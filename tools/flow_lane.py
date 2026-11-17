"""One durable push lane. The live state lock is used only for queue/receipt mutations.

Candidates are independent local clones with private flow snapshots. No branch, stash or
worktree is removed, including failed candidates. A run drains one frozen queue snapshot.
"""
from contextlib import contextmanager
import fcntl
from functools import wraps
import hashlib
import json
import math
import os
from pathlib import Path
import shutil
import shlex
import socket
import signal
import subprocess
import sys
import time
import uuid

import flow as f
import flow_heartbeat
import agent_advice
import resource_admission
import check as check_gate
from check_source_digest import source_digest_cache
from flow_state import state_lock, atomic_write, require_complete


def require_base_tools(root, branch='main', base=None):
    """Refuse stale running code, independently of the checkout supplying candidate inputs."""
    if base is None:
        f.git(root, 'fetch', '--quiet', 'origin', branch)
        base = f.git(root, 'rev-parse', f'origin/{branch}')
    expected = {}
    for row in f.git(root, 'ls-tree', '-r', base, '--', 'tools').splitlines():
        metadata, path = row.split('\t', 1)
        if Path(path).suffix in ('.py', '.json'):
            expected[path] = metadata.split()[2]
    live = f.lane_tool_sources()
    paths = set(expected) | set(live) | set(f.LANE_TOOL_SOURCES)
    different = sorted(p for p in paths if expected.get(p) != live.get(p)
                       or expected.get(p) != f.LANE_TOOL_SOURCES.get(p))
    f.refuse(not different,
             f'lane tooling differs from integration base {base}: ' + ', '.join(different[:8])
             + '. Run tools/flow.py from a clean checkout of this origin base, with --root '
             'pointing to the input checkout and --flow-dir to the shared flow directory '
             '(including tooling-push of changes to these tools). See docs/orchestrator-runbook.md.')
    return base


def read(path, *, details=True):
    value = json.loads(path.read_text())
    if details and isinstance(value, dict) and value.get('details_sidecar'):
        ref = value['details_sidecar']
        # Immutable content-addressed sidecars survive receipt replacement and snapshot copies.
        name = ref['sha256']
        f.refuse(len(name) == 64 and all(c in '0123456789abcdef' for c in name), 'invalid receipt sidecar digest')
        blob = (path.parent / 'details' / (name + '.json')).read_bytes()
        f.refuse(hashlib.sha256(blob).hexdigest() == name, 'receipt sidecar digest differs')
        detail = json.loads(blob)
        files = detail.pop('integrated_files', None)
        value.update(detail)
        if files is not None:
            value['integrated']['files'] = files
            value['integrated'].pop('files_count', None)
        for key in ('tooling', 'tooling_accepted'):
            for item in value.get(key, []):
                if item.pop('integrated_files_from_run', False):
                    item['integrated']['files'] = files
        value.pop('details_sidecar')
    return value


def compact_receipt(path, receipt):
    """Serialize large immutable member detail outside the scheduling lock; keep readers compatible."""
    detail = {key: receipt[key] for key in ('origins', 'bindings', 'input_pins', 'gate_dependencies') if key in receipt}
    if (receipt.get('integrated') or {}).get('files'):
        detail['integrated_files'] = receipt['integrated']['files']
    blob = json.dumps(detail, separators=(',', ':'), sort_keys=True).encode()
    if len(blob) < 65536:
        return receipt
    digest = hashlib.sha256(blob).hexdigest()
    sidecar = path.parent / 'details' / (digest + '.json')
    if not sidecar.is_file():
        atomic_write(sidecar, blob)
    compact = {key: value for key, value in receipt.items() if key not in detail}
    if 'integrated_files' in detail:
        compact['integrated'] = dict(receipt['integrated'], files_count=len(detail['integrated_files']))
        compact['integrated'].pop('files')
    compact['details_sidecar'] = dict(sha256=digest, bytes=len(blob), fields=sorted(detail))
    compact_tooling(compact, receipt)
    return compact


def compact_tooling(compact, receipt):
    """Keep current mutable request state, referencing the run's shared file list only."""
    if compact is receipt or not compact.get('details_sidecar'):
        return
    files = (receipt.get('integrated') or {}).get('files')
    if not files:
        return
    for key in ('tooling', 'tooling_accepted'):
        if key not in receipt:
            continue
        items = []
        for original in receipt[key]:
            item = dict(original)
            integrated = item.get('integrated') or {}
            listed = integrated.get('files')
            if listed is files or listed == files:
                item['integrated'] = {k: v for k, v in integrated.items() if k != 'files'}
                item['integrated_files_from_run'] = True
            items.append(item)
        compact[key] = items


def write(path, value):
    atomic_write(path, (json.dumps(value, indent=2, sort_keys=True) + '\n').encode())


def guard_cleanup(fdir, root):
    # Called under the state lock by every enqueue/resume path.
    for reservation in (Path(fdir) / 'cleanup/claims').glob('*.json'):
        claim = read(reservation)
        f.refuse(not claim.get('active') or not Path(root).resolve().is_relative_to(Path(claim['tree'])),
                 'input checkout is being cleaned; retry after cleanup with a retained input')


def enqueue(root, fdir, kind, selection, branch='main', approved_only=False, with_metadata=False, name=None):
    root, fdir = Path(root).resolve(), Path(fdir).resolve()
    payload = dict(root=str(root), kind=kind, selection=selection, branch=branch,
                   approved_only=approved_only, with_metadata=with_metadata, name=name)
    key = hashlib.sha256(json.dumps(payload, sort_keys=True).encode()).hexdigest()[:24]
    path = fdir / 'lane/queue' / (key + '.json')
    with state_lock(fdir):
        require_complete(fdir)
        guard_cleanup(fdir, root)
        if path.exists():
            previous = read(path)
            if previous['state'] in ('queued', 'running'):
                return previous
        item = dict(payload, id=key, state='queued', queued_at=f.utc())
        write(path, item)
    return item


@contextmanager
def lane_lock(fdir, wait=False, purpose=None):
    path = Path(fdir) / 'lane/.lane.lock'
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open('a') as handle:
        if wait:
            print('Waiting for the lane lock is not a stall; keep waiting with --wait-lane. Queued items are kept.', file=sys.stderr, flush=True)
        try:
            fcntl.flock(handle, fcntl.LOCK_EX | (0 if wait else fcntl.LOCK_NB))
        except BlockingIOError:
            raise f.FlowError('push lane already running; queued work stays for the next run') from None
        owner_path = Path(fdir) / 'lane/owner.json'
        if purpose:
            with state_lock(fdir):
                write(owner_path, dict(process_owner(), kind=purpose, active=True))
        lease = LaneLease(handle, fdir, owner_path, purpose)
        try:
            yield lease
        finally:
            lease.release()


class LaneLease:
    """A build/push lease that may finish before the enclosing CI watch."""
    def __init__(self, handle, fdir, owner_path, purpose):
        self.handle, self.fdir, self.owner_path, self.purpose = handle, fdir, owner_path, purpose
        self.started, self.seconds, self.released = time.monotonic(), None, False

    def release(self):
        if self.released:
            return
        self.seconds = time.monotonic() - self.started
        if self.purpose:
            with state_lock(self.fdir):
                owner = read(self.owner_path)
                owner.update(active=False, hold_seconds=self.seconds)
                write(self.owner_path, owner)
        fcntl.flock(self.handle, fcntl.LOCK_UN)
        self.seconds = time.monotonic() - self.started
        self.released = True


def running(fdir):
    """Probe the permanent lane lock without waiting for a runner."""
    try:
        with lane_lock(fdir):
            return False
    except f.FlowError:
        return True


def snapshot(fdir, destination):
    """Only small scheduling files, never integration trees or analysis inputs."""
    with state_lock(fdir):
        require_complete(fdir)
        destination.mkdir(parents=True, exist_ok=True)
        for name in ('groups', 'imports', 'combined', 'lane/queue', 'lane/runs', 'lane/tooling', 'lane/tooling-queue'):
            if (fdir / name).exists():
                shutil.copytree(fdir / name, destination / name)
        for name in ('versions.json', 'events.jsonl', 'lane/owner.json'):
            if (fdir / name).is_file():
                (destination / name).parent.mkdir(parents=True, exist_ok=True)
                shutil.copyfile(fdir / name, destination / name)
    # The publication clone must enforce the owner's ignored policy too. Never track this file.
    policy = Path(fdir).parent / 'publication-forbidden.json'
    if policy.is_file():
        shutil.copyfile(policy, destination.parent / policy.name)
    write(destination / 'lane-live.json', dict(running=running(fdir), flow_dir=str(Path(fdir).resolve()),
                                               service_enabled=(Path(fdir) / 'lane/service/config.json').is_file()))


def process_owner():
    return dict(pid=os.getpid(), host=socket.gethostname(), birth=process_birth(os.getpid()))


def process_birth(pid):
    result = subprocess.run(['ps', '-p', str(int(pid)), '-o', 'lstart='], capture_output=True, text=True)
    return result.stdout.strip() if result.returncode == 0 else None


def owner_alive(owner):
    if not owner:
        return False
    if owner.get('host') != socket.gethostname():
        return True  # An unknown host is not evidence of a dead process.
    birth = process_birth(owner['pid'])
    return bool(birth and birth == owner.get('birth'))


@contextmanager
def service_authority(fdir):
    """Private service role bound to this process and a held permanent lock, not a CLI flag."""
    private = Path(fdir) / 'lane/service'
    private.mkdir(parents=True, exist_ok=True)
    with (private / '.worker.lock').open('a') as handle:
        fcntl.flock(handle, fcntl.LOCK_EX | fcntl.LOCK_NB)
        owner = dict(process_owner(), active=True)
        f.refuse(owner.get('birth'), 'cannot establish lane service process birth; no control authority granted')
        write(private / 'worker-owner.json', owner)
        try:
            yield
        finally:
            write(private / 'worker-owner.json', dict(owner, active=False))


def service_caller(fdir):
    private = Path(fdir) / 'lane/service'
    marker = private / 'worker-owner.json'
    if not marker.is_file():
        return False
    owner = read(marker)
    current = process_owner()
    if not current.get('birth') or not owner.get('active') or {k: owner.get(k) for k in ('pid', 'host', 'birth')} != current:
        return False
    with (private / '.worker.lock').open('a') as handle:
        try:
            fcntl.flock(handle, fcntl.LOCK_EX | fcntl.LOCK_NB)
        except BlockingIOError:
            return True
    return False


def require_control(fdir, receipt, action):
    current = process_owner()
    f.refuse((bool(current.get('birth')) and receipt.get('owner') == current) or service_caller(fdir),
             f'cannot {action} lane {receipt["id"]}: only the lane service or its recorded owner may do that; '
             "never kill a lane process you didn't start")


def stop_run(root, fdir, run_id, reason):
    requested = read(receipt_path(fdir, run_id))
    if requested.get('kind') == 'tooling-request':
        run_id = requested['run']
    with state_lock(fdir):
        path = receipt_path(fdir, run_id)
        receipt = read(path)
        require_control(fdir, receipt, 'stop')
        owner = receipt.get('owner') or {}
        f.refuse(owner.get('host') == socket.gethostname() and owner_alive(owner),
                 'lane owner is not a live local process; no signal sent')
        receipt.update(stop_requested_at=f.utc(), stop_reason=reason, stop_requested_by=process_owner(),
                       error='Publication intentionally stopped: ' + reason)
        write(path, flow_heartbeat.preserve(path, receipt))
        # Signal only the birth-checked recorded PID; never a shared process group.
        os.kill(owner['pid'], signal.SIGTERM)
        return receipt


def run_status(root, fdir, receipt, active=None):
    value = dict(receipt)
    live = Path(fdir) / 'lane-live.json'
    info = read(live) if live.is_file() else {}
    actual = info.get('flow_dir', str(fdir))
    prefix = ('python3 ' + shlex.quote(str(f.TOOLS / 'flow.py')) + ' --root ' + shlex.quote(str(receipt.get('root') or root))
              + ' --flow-dir ' + shlex.quote(actual))
    if receipt.get('state') in ('running', 'stale', 'failed', 'conflict'):
        active = (info['running'] if 'running' in info else running(fdir)) if active is None else active
        alive = owner_alive(receipt.get('owner'))
        # Legacy receipts have no PID. A held lane lock protects those old runners.
        owner_path = Path(fdir) / 'lane/owner.json'
        holder = read(owner_path) if owner_path.is_file() else {}
        other_owner = (holder.get('active') and owner_alive(holder)
                       and holder.get('kind') != (receipt.get('kind') or 'integration'))
        stale = not alive and (bool(receipt.get('owner')) or not active or other_owner)
        value['status'] = 'stale' if stale else 'running' if active else 'paused'
        value['lane_busy'] = active
        command = 'tooling-push' if receipt.get('kind') == 'tooling' else 'lane-run'
        resume_id = receipt.get('run') if receipt.get('kind') == 'tooling-request' else receipt['id']
        value['resume_command'] = prefix + ' ' + command + ' --resume ' + shlex.quote(resume_id) + ' --branch ' + shlex.quote(receipt['branch'])
        if not (receipt.get('integrated') or {}).get('pushed'):
            value['retire_command'] = prefix + ' lane-retire ' + resume_id + " --reason 'owner stopped; selection superseded'"
    return value


def receipt_path(fdir, run_id):
    """Resolve an explicit run id across acceptance and tooling receipts, never guess."""
    f.refuse(run_id and Path(run_id).name == run_id and run_id not in ('.', '..'), 'invalid run id')
    paths = [Path(fdir) / 'lane' / kind / (run_id + '.json') for kind in ('runs', 'tooling')]
    found = [p for p in paths if p.is_file()]
    f.refuse(len(found) == 1, 'no unique lane or tooling receipt for ' + run_id)
    return found[0]


def retire(root, fdir, run_id, reason, *, requeue_stale=False):
    requested = read(receipt_path(fdir, run_id))
    if requested.get('kind') == 'tooling-request':
        run_id = requested['run']
        requested = read(receipt_path(fdir, run_id))
    require_control(fdir, requested, 'retire')
    with lane_lock(fdir), state_lock(fdir):
        path = receipt_path(fdir, run_id)
        receipt = read(path)
        require_control(fdir, receipt, 'retire')
        f.refuse(receipt['state'] in ('running', 'stale', 'failed', 'conflict'), 'only an unfinished or failed run can be retired')
        f.refuse(not (receipt.get('integrated') or {}).get('pushed'), 'pushed run must resume CI; cannot retire it')
        if requeue_stale:
            owner = receipt.get('owner') or {}
            f.refuse(owner.get('host') == socket.gethostname() and not owner_alive(owner),
                     'automatic recovery requires a dead local owner')
            f.refuse(not receipt.get('error') and receipt['state'] in ('running', 'stale'),
                     'recorded failures require triage, not automatic recovery')
            f.refuse(path.parent.name == 'runs', 'legacy tooling requires explicit resume')
            recovered = []
            for directory, key in (('queue', 'items'), ('tooling-queue', 'tooling')):
                for original in receipt.get(key, []):
                    queued = Path(fdir) / 'lane' / directory / (original['id'] + '.json')
                    # Another run or a worker may already have replaced this selection.
                    current = read(queued) if queued.is_file() else original
                    if current.get('run') != run_id or current.get('state') not in ('running', 'stale'):
                        continue
                    current.update(state='queued', recovered_from=run_id, queued_at=f.utc())
                    for field in ('run', 'error', 'integrated'):
                        current.pop(field, None)
                    write(queued, current)
                    recovered.append(current['id'])
            receipt['requeued_items'] = recovered
        receipt.update(state='retired', retired_at=f.utc(), retirement_reason=reason)
        for item in receipt.get('items', []):
            item['state'] = 'retired'
        if path.parent.name == 'tooling' or requeue_stale:
            write(path, compact_receipt(path, receipt))
        else:
            save_run(fdir, receipt)
        f.log_event(Path(fdir), 'lane-retired', lane=run_id, kind=receipt.get('kind', 'integration'), reason=reason)
        return receipt



SUPERSEDED_ADVISORY = 'red-advisory-superseded'
ADVISORY_WORKFLOW = 'Advisory target hashes'
ADVISORY_STEP = 'target-hashes / Compile 50 accepted references twice against published target hashes'


def ci_evidence(root, commit, python):
    """Read exact-SHA CI evidence without interpreting an advisory failure as success."""
    result = subprocess.run([python, str(f.TOOLS / 'ci_status.py'), '--commit', commit, '--json'],
                            cwd=root, capture_output=True, text=True)
    f.refuse(result.returncode in (0, 1), 'CI evidence unavailable or pending for ' + commit)
    try:
        evidence = json.loads(result.stdout)
    except ValueError:
        raise f.FlowError('invalid CI evidence for ' + commit) from None
    f.refuse(evidence.get('commit') == commit, 'CI evidence names another commit')
    f.refuse(evidence.get('result') == {0: 'green', 1: 'red'}[result.returncode], 'CI evidence result mismatch')
    return evidence


def validate_supersession_ci(original, replacement):
    required = {'Infrastructure checks', 'Publish research report', ADVISORY_WORKFLOW}
    for evidence in (original, replacement):
        runs = evidence.get('runs', [])
        f.refuse(required <= {r.get('workflow') for r in runs}, 'missing required CI workflow')
        f.refuse(all(r.get('status') == 'completed' for r in runs), 'CI still pending')
    f.refuse(original.get('result') == 'red', 'original CI must remain red')
    failures = [r for r in original['runs'] if r.get('conclusion') != 'success']
    f.refuse(failures and all(r.get('workflow') == ADVISORY_WORKFLOW
                             and r.get('conclusion') == 'failure'
                             and r.get('failed_steps') == [ADVISORY_STEP] for r in failures),
             'CI failure is not exclusively the supported advisory hash step')
    f.refuse(replacement.get('result') == 'green'
             and all(r.get('conclusion') == 'success' for r in replacement['runs']),
             'superseding CI must pass every workflow')


def finish_superseded(root, fdir, run_id, superseding_commit, reason, python=sys.executable):
    """Finish a dead pushed checkpoint after an explicitly authorized advisory repair."""
    root, fdir = Path(root).resolve(), Path(fdir).resolve()
    f.refuse(reason.strip(), 'a supersession reason is required')
    import ci_status
    f.refuse(ci_status.FULL_SHA.fullmatch(superseding_commit), 'superseding commit must be a full SHA')
    lock = fdir / 'lane/ci-locks' / (run_id + '.lock')
    # Resolve before constructing the observer lock so run ids cannot escape its directory.
    path = receipt_path(fdir, run_id)
    lock.parent.mkdir(parents=True, exist_ok=True)
    with lock.open('a') as handle:
        try:
            fcntl.flock(handle, fcntl.LOCK_EX | fcntl.LOCK_NB)
        except BlockingIOError:
            raise f.FlowError('CI observer is active; cannot supersede receipt') from None
        with state_lock(fdir):
            require_complete(fdir)
            receipt = read(path)
            integ = receipt.get('integrated') or {}
            f.refuse(not owner_alive(receipt.get('owner')), 'receipt owner is still live')
            f.refuse(receipt.get('state') in ('running', 'stale', 'failed', 'conflict', 'done'),
                     'receipt cannot be finished by supersession')
            f.refuse(integ.get('pushed'), 'only an already pushed receipt can be superseded')
            if receipt.get('state') == 'done':
                f.refuse(integ.get('ci') == SUPERSEDED_ADVISORY
                         and integ.get('superseded_by') == superseding_commit,
                         'terminal receipt has another outcome')
                return dict(id=run_id, state='done', commit=integ['commit'], ci=integ['ci'],
                            superseded_by=superseding_commit)
            commit, branch = integ['commit'], receipt['branch']
        f.refuse(commit != superseding_commit, 'superseding commit must be a descendant')
        f.git(root, 'fetch', '--quiet', 'origin', branch)
        f.refuse(f.git(root, 'merge-base', commit, superseding_commit, check=False) == commit,
                 'pushed commit is not an ancestor of superseding commit')
        f.refuse(f.git(root, 'merge-base', superseding_commit, 'origin/' + branch, check=False) == superseding_commit,
                 'superseding commit is not published on the receipt branch')
        original = ci_evidence(root, commit, python)
        replacement = ci_evidence(root, superseding_commit, python)
        validate_supersession_ci(original, replacement)
        with state_lock(fdir):
            # The per-run observer lease excludes concurrent pushed-receipt writers.
            integ.update(ci=SUPERSEDED_ADVISORY, superseded_by=superseding_commit,
                         ci_supersession=dict(reason=reason, recorded_at=f.utc(),
                                              original=original, superseding=replacement))
            receipt.update(state='done', finished_owner=receipt.get('owner'), owner=None)
            receipt.pop('error', None)
            accepted_items = {r['item'] for r in receipt.get('accepted', [])}
            for item in receipt.get('items', []):
                item['state'] = 'done' if item['id'] in accepted_items else 'dropped'
            if path.parent.name == 'runs':
                save_run(fdir, receipt)
            else:
                save_tooling(fdir, receipt)
            f.log_event(fdir, 'ci-superseded', lane=run_id, commit=commit,
                        result=SUPERSEDED_ADVISORY, superseded_by=superseding_commit, reason=reason)
    import flow_cleanup
    flow_cleanup.automatic_retention(root, fdir)
    return dict(id=run_id, state='done', commit=commit, ci=SUPERSEDED_ADVISORY,
                superseded_by=superseding_commit, accepted=integ.get('accepted'))

def lane_python(requested):
    """The worker interpreter is a transport/build prerequisite, never a match predicate."""
    configured = os.environ.get('SKYRIM_LANE_PYTHON')
    candidate = configured or (requested if sys.version_info[:2] == (3, 13) else shutil.which('python3.13'))
    f.refuse(candidate, 'lane requires python3.13 with pyexpat; set SKYRIM_LANE_PYTHON')
    if Path(candidate).resolve() == Path(sys.executable).resolve() and sys.version_info[:2] == (3, 13):
        try:
            import pyexpat
        except ImportError:
            raise f.FlowError('lane requires python3.13 with working pyexpat') from None
        return str(candidate)
    result = subprocess.run([str(candidate), '-c',
                             'import sys,pyexpat; sys.exit(0 if sys.version_info[:2] == (3,13) else 1)'],
                            capture_output=True, text=True)
    f.refuse(result.returncode == 0, 'lane requires python3.13 with working pyexpat; interpreter refused: ' + str(candidate))
    return str(candidate)


def recover_stale(fdir):
    """Skip dead local unpublished owners under the build lease; retain every input and candidate."""
    with state_lock(fdir):
        for directory in ('runs', 'tooling'):
            for path in (Path(fdir) / 'lane' / directory).glob('*.json'):
                receipt = read(path)
                owner = receipt.get('owner') or {}
                if (receipt.get('kind') == 'tooling-request' or receipt.get('state') != 'running' or not owner or owner.get('host') != socket.gethostname()
                        or owner_alive(owner) or (receipt.get('integrated') or {}).get('pushed')):
                    continue
                receipt.update(state=('conflict' if 'git rebase' in receipt.get('error', '') else 'stale'), stale_at=f.utc(),
                               recovery_reason='Dead local publisher skipped; inputs and candidate retained')
                for item in receipt.get('items', []):
                    item['state'] = 'stale'
                if directory == 'runs':
                    save_run(fdir, receipt)
                else:
                    write(path, receipt)
                f.log_event(Path(fdir), 'lane-stale-skipped', lane=receipt['id'])


def wait_for_owner(root, fdir, directory, resume, wait):
    """A live other publisher is a queue wait; own/stale work needs an explicit resume."""
    announced = False
    while True:
        with state_lock(fdir):
            unfinished = [read(p) for p in (Path(fdir) / 'lane' / directory).glob('*.json')
                          if read(p)['state'] == 'running' and read(p).get('kind') != 'tooling-request'
                          and not (read(p).get('integrated') or {}).get('pushed')]
        blockers = [r for r in unfinished if r['id'] != resume]
        blockers = [r for r in blockers if not (r.get('root') == str(root)
                    and (r.get('owner') or {}).get('pid') == os.getpid() and not running(fdir))]
        if not blockers:
            return
        blockers = [r for r in blockers if owner_alive(r.get('owner'))
                    or (not r.get('owner') and running(fdir))]
        if not blockers:
            return
        for r in blockers:
            command = run_status(root, fdir, r)['resume_command']
            f.refuse(owner_alive(r.get('owner')) or (not r.get('owner') and running(fdir)),
                     'unfinished ' + ('tooling publication' if directory == 'tooling' else 'lane run')
                     + '; resume own or stale receipt: ' + command)
        f.refuse(wait, 'live publisher busy; retry with --wait-lane')
        if not announced:
            print('Waiting for the live publisher is not a stall; keep waiting with --wait-lane. Queued items are kept.', file=sys.stderr, flush=True)
            announced = True
        time.sleep(0.25)


def require_green(root, commit, python, ci_wait):
    flow_heartbeat.phase('origin exact-SHA CI')
    result = subprocess.run([python, 'tools/ci_status.py', '--without-advisory', '--commit', commit, '--wait', str(ci_wait)],
                            cwd=root, capture_output=True, text=True)
    f.refuse(result.returncode == 0, 'origin CI is not green: ' + result.stdout.strip())


def publish_tooling(root, fdir, branch='main', ci_wait=600, python=sys.executable, resume=None, wait_lane=False,
                    soft_free_gb=None, allow_low_disk=False, repair_red=None):
    """Register new tooling requests before taking the shared batching lane."""
    root, fdir = Path(root).resolve(), Path(fdir).resolve()
    require_base_tools(root, branch)
    import flow_cleanup
    if fdir.is_dir():
        flow_cleanup.automatic_retention(root, fdir)
    python = lane_python(python)
    if repair_red:
        import flow_repair
        flow_repair.validate_sha(repair_red)
        return _publish_legacy_tooling(root, fdir, branch, ci_wait, python, resume, wait_lane,
                                       soft_free_gb, allow_low_disk, repair_red)
    if not resume:
        import flow_tooling
        return flow_tooling.publish(root, fdir, branch, ci_wait, python, soft_free_gb, allow_low_disk, wait_lane)
    requested = read(receipt_path(fdir, resume))
    if requested.get('kind') == 'tooling-request':
        resume = requested['run']
    if receipt_path(fdir, resume).parent.name == 'runs':
        return run_lane(root, fdir, branch, True, ci_wait, python, soft_free_gb=soft_free_gb,
                        allow_low_disk=allow_low_disk, resume=resume, wait_lane=wait_lane)
    return _publish_legacy_tooling(root, fdir, branch, ci_wait, python, resume, wait_lane,
                                  soft_free_gb, allow_low_disk)


@flow_heartbeat.publisher
def _publish_legacy_tooling(root, fdir, branch='main', ci_wait=600, python=sys.executable, resume=None, wait_lane=False,
                    soft_free_gb=None, allow_low_disk=False, repair_red=None):
    """Check and publish committed tooling under the same lane lock as acceptance work."""
    root, fdir = Path(root).resolve(), Path(fdir).resolve()
    require_base_tools(root, branch)
    python = lane_python(python)
    if resume:
        with state_lock(fdir):
            previous = read(receipt_path(fdir, resume))
        if (previous.get('integrated') or {}).get('pushed'):
            f.refuse(previous['root'] == str(root) and previous['branch'] == branch,
                     'resume tooling publication with its original root and branch')
            if previous.get('repair_red') and not previous.get('repair_dispatched'):
                with lane_lock(fdir, wait=wait_lane, purpose='tooling-repair-dispatch'):
                    f.git(root, 'fetch', '--quiet', 'origin', branch)
                    f.refuse(f.git(root, 'rev-parse', 'origin/' + branch) == previous['integrated']['commit'],
                             'repair head moved before workflow dispatch; inspect before resuming')
                    import flow_repair
                    previous['repair_dispatched'] = flow_repair.dispatch(root, branch, previous['repair'])
                    save_tooling(fdir, previous)
            return finish_tooling(root, fdir, previous, python, ci_wait)
    if repair_red:
        wait_for_owner(root, fdir, 'runs', None, wait_lane)
    wait_for_owner(root, fdir, 'tooling', resume, wait_lane)
    with lane_lock(fdir, wait=wait_lane, purpose='tooling') as lease:
        require_base_tools(root, branch)
        recover_stale(fdir)
        with state_lock(fdir):
            pending = [read(p) for p in (fdir / 'lane/tooling').glob('*.json') if read(p)['state'] == 'running' and read(p).get('kind') != 'tooling-request'
                       and not (read(p).get('integrated') or {}).get('pushed')]
            if resume:
                selected = read(receipt_path(fdir, resume))
                f.refuse(selected['state'] in ('running', 'failed', 'conflict', 'stale'), 'receipt cannot be resumed')
                pending = [selected]
            f.refuse(len(pending) <= 1, 'multiple unfinished tooling publications')
            if pending:
                receipt = pending[0]
                f.refuse(resume == receipt['id'], 'unfinished tooling publication; use tooling-push --resume ' + receipt['id'])
                f.refuse(receipt['root'] == str(root) and receipt['branch'] == branch,
                         'resume tooling publication with its original root and branch')
            else:
                f.refuse(not resume, 'no unfinished tooling publication with that id')
                receipt = dict(id=uuid.uuid4().hex[:16], kind='tooling', root=str(root), branch=branch,
                               state='running', integrated=None)
            guard_cleanup(fdir, root)
            if receipt.get('tree'):
                guard_cleanup(fdir, receipt['tree'])
            if repair_red and receipt.get('repair_red'):
                f.refuse(repair_red == receipt['repair_red'], 'repair SHA differs from saved receipt')
            repair_red = repair_red or receipt.get('repair_red')
            if repair_red:
                receipt['repair_red'] = repair_red
            receipt['owner'] = process_owner()
            receipt.update(state='running', python=python)
            path = fdir / 'lane/tooling' / (receipt['id'] + '.json')
            save_tooling(fdir, receipt)

        def save():
            save_tooling(fdir, receipt)

        try:
            integ = receipt.get('integrated') or {}
            if not integ.get('pushed'):
                f.refuse(not f.git(root, 'status', '--porcelain', '--untracked-files=all'),
                         'tooling-push requires a clean, committed checkout')
                for attempt in range(3):
                    f.git(root, 'fetch', '--quiet', 'origin', branch)
                    base = f.git(root, 'rev-parse', f'origin/{branch}')
                    require_base_tools(root, branch, base)
                    checked = receipt.get('integrated') or {}
                    if (checked.get('checks') and checked.get('commit') == f.git(root, 'rev-parse', 'HEAD')
                            and f.git(root, 'merge-base', checked['commit'], base) == checked['commit']):
                        # Recover death after the server accepted the push but before its receipt write.
                        integ = checked
                        integ['pushed'] = f.utc()
                        save()
                        break
                    f.refuse(f.git(root, 'rev-parse', 'HEAD') != base, 'no tooling commit to push')
                    if f.git(root, 'merge-base', base, 'HEAD') != base:
                        f.git(root, 'rebase', f'origin/{branch}')
                    commit = f.git(root, 'rev-parse', 'HEAD')
                    if repair_red:
                        import flow_repair
                        receipt['repair'] = flow_repair.approve(root, base, commit, repair_red, python, ci_wait)
                        save()
                    checked = receipt.get('integrated') or {}
                    if not (checked.get('checks') and checked.get('commit') == commit):
                        resource_admission.admit(root, 'tooling validation', soft_gb=soft_free_gb, allow_low_disk=allow_low_disk)
                        started = time.monotonic()
                        logs = fdir / 'lane/tooling' / receipt['id'] / str(len(receipt.get('attempts', [])))
                        receipt['checks_log_dir'] = str(logs)
                        receipt.setdefault('attempts', []).append(dict(commit=commit, base=base, logs=str(logs)))
                        save()
                        f.pre_push_checks(root, commit, python, candidate=True, log_dir=logs)
                        receipt['integrated'] = integ = dict(commit=commit, base_revision=base, checks=f.utc(),
                                                            pushed=None, ci=None, wall_seconds={'checks': time.monotonic()-started})
                        save()
                    else:
                        integ = checked
                        integ['base_revision'] = base
                    # A red main must be resolved before unrelated publication.
                    if repair_red:
                        receipt['repair'] = flow_repair.approve(root, base, commit, repair_red, python, ci_wait)
                        save()
                    else:
                        require_green(root, base, python, ci_wait)
                    f.git(root, 'fetch', '--quiet', 'origin', branch)
                    if f.git(root, 'rev-parse', f'origin/{branch}') != base:
                        continue
                    require_base_tools(root, branch, base)
                    f.refuse(f.git(root, 'rev-parse', 'HEAD') == commit and
                             not f.git(root, 'status', '--porcelain', '--untracked-files=all'), 'checked tooling tree changed')
                    pushed = subprocess.run(['git', 'push', 'origin', f'{commit}:refs/heads/{branch}'],
                                            cwd=root, capture_output=True, text=True)
                    if pushed.returncode:
                        f.git(root, 'fetch', '--quiet', 'origin', branch)
                        f.refuse(f.git(root, 'rev-parse', f'origin/{branch}') != base,
                                 'tooling push failed: ' + pushed.stderr.strip())
                        continue
                    integ['pushed'] = f.utc()
                    save()
                    break
                f.refuse(integ.get('pushed'), 'origin kept moving; resume tooling-push after coordinating publishers')
            if repair_red and not receipt.get('repair_dispatched'):
                import flow_repair
                receipt['repair_dispatched'] = flow_repair.dispatch(root, branch, receipt['repair'])
                save()
            lease.release()
            receipt['lane_hold_seconds'] = lease.seconds
            save()
            return finish_tooling(root, fdir, receipt, python, ci_wait)

        except BaseException as error:
            if (receipt.get('integrated') or {}).get('pushed'):
                raise
            receipt['error'] = str(error)
            if 'git rebase' in str(error) and not (receipt.get('integrated') or {}).get('pushed'):
                receipt['state'] = 'conflict'
                receipt['resume_command'] = run_status(root, fdir, receipt).get('resume_command')
            save()
            raise


def ci_serialized(directory):
    """Only one observer updates a pushed receipt; this lock never covers building or pushing."""
    def decorate(function):
        @wraps(function)
        def finish(root, fdir, receipt, python, ci_wait, **kwargs):
            path = Path(fdir) / 'lane' / directory / (receipt['id'] + '.json')
            lock = Path(fdir) / 'lane/ci-locks' / (receipt['id'] + '.lock')
            lock.parent.mkdir(parents=True, exist_ok=True)
            with lock.open('a') as handle:
                fcntl.flock(handle, fcntl.LOCK_EX)
                with state_lock(fdir):
                    current = read(path)
                integrated = current.get('integrated') or {}
                terminal_ci = (integrated.get('ci') == 'green'
                               or (integrated.get('ci') == SUPERSEDED_ADVISORY
                                   and integrated.get('superseded_by') and integrated.get('ci_supersession')))
                if current.get('state') == 'done' and terminal_ci:
                    return current
                try:
                    return function(root, fdir, current, python, ci_wait, **kwargs)
                except BaseException as error:
                    current['error'] = str(error)
                    if directory == 'runs':
                        save_run(fdir, current)
                    else:
                        save_tooling(fdir, current)
                    raise
        return finish
    return decorate


def save_tooling(fdir, receipt):
    with state_lock(fdir):
        path = Path(fdir) / 'lane/tooling' / (receipt['id'] + '.json')
        flow_heartbeat.watch(fdir, path, receipt.get('owner'))
        write(path, flow_heartbeat.preserve(path, receipt))


@flow_heartbeat.publisher
@ci_serialized('tooling')
def finish_tooling(root, fdir, receipt, python, ci_wait):
    integ = receipt['integrated']
    receipt['owner'] = process_owner()
    save_tooling(fdir, receipt)
    flow_heartbeat.phase('exact-SHA CI')
    args = [python, 'tools/ci_status.py']
    if receipt.get('repair'):
        for workflow in ('Infrastructure checks', 'Publish research report', *receipt['repair']['workflows']):
            args += ['--require-workflow', workflow]
    else:
        args += ['--without-advisory']
    result = subprocess.run(args + ['--commit', integ['commit'], '--wait', str(ci_wait)],
                            cwd=root, capture_output=True, text=True)
    integ['ci'] = {0: 'green', 1: 'red', 2: 'pending'}.get(result.returncode, 'unavailable')
    ci_log = fdir / 'lane/tooling' / receipt['id'] / 'ci.log'
    with state_lock(fdir):
        atomic_write(ci_log, (result.stdout + result.stderr).encode())
    receipt['ci_log'] = str(ci_log)
    save_tooling(fdir, receipt)
    f.refuse(integ['ci'] == 'green', 'tooling CI ' + integ['ci'] + ': ' + result.stdout.strip())
    receipt.update(state='done', finished_owner=receipt.get('owner'), owner=None)
    receipt.pop('error', None)
    save_tooling(fdir, receipt)
    import flow_cleanup
    flow_cleanup.automatic_retention(root, fdir)
    return receipt



def signature(fdir, group):
    # Integration receipts may change without changing any gate evidence.
    value = dict(f.load_group(fdir, group))
    value.pop('integrated', None)
    return value


def gate_inputs(root, frozen, requests):
    """Pin the complete evidence consulted for these members, not other workers' gates."""
    data = f.inventory(root)
    by_id = {fn['id']: fn for fn in data['functions']}
    selected = {(r['build_id'], rva) for r in requests for rva in r['functions']}
    members = {fid for fid, fn in by_id.items() if (fn['build_id'], f.hexrva(fn['rva'])) in selected}
    records = {}

    def record(fid):
        if fid not in records:
            records[fid] = f.fr.read(root, by_id[fid]['report'])
        return records[fid]

    families, batches = f.family_manifests(root), f.family_batches(root)
    paths, relevant_families = set(), set()
    for request in requests:
        if request['kind'] == 'family':
            paths.add(request['path'])
            batch = batches[request['path']]
            members.update(batch['population'])
            relevant_families.add(batch['family_path'])
    # Individual reviews also consult family holds and the samples of bound batches.
    for path, family in families.items():
        if any(record(fid).get('origin') == family['origin']
               and f.fr.binding(by_id[fid], record(fid)) == family['binding'] for fid in members):
            relevant_families.add(path)
    sample_members = set()
    for path, batch in batches.items():
        if path in paths or any(fid in batch['population']
                                and f.fr.member_digest(by_id[fid], record(fid)) == batch['members'][fid]
                                for fid in members):
            paths.add(path)
            relevant_families.add(batch['family_path'])
            sample_members.update(batch['sample'] or [])
    members.update(sample_members)
    for path in relevant_families:
        family = families[path]
        paths.add(path)
        members.update(family['first_members'])
        if family.get('review'):
            paths.add(f.REVIEWS + '/' + family['review']['review_task_id'] + '.family.json')
        for field in ('template_source', 'parameter_source'):
            paths.add(family[field]['path'])
    referenced = {(by_id[fid]['build_id'], f.hexrva(by_id[fid]['rva'])) for fid in members}
    all_groups = f.groups(frozen)
    names = {r['group'] for r in requests if r.get('group')}
    names.update(name for name, group in all_groups.items()
                 if group.get('family_review_path') in relevant_families
                 or any((group['build_id'], rva) in referenced for rva in group['functions']))
    names.update(all_groups[name]['pair'] for name in list(names) if all_groups[name].get('pair'))
    bindings = {name: signature(frozen, name) for name in names}
    for group in bindings.values():
        paths.add(str(f.review_path(root, group).resolve()))
        if group.get('family_review_path') in relevant_families:
            paths.add(f.REVIEWS + '/' + group['task_id'] + '.family.json')
    for fid in members:
        paths.add(by_id[fid]['report'])
        paths.add(str(Path(by_id[fid]['report']).with_suffix('.review.json')))
    pins = {}
    for rel in paths:
        path = root / rel
        pins[str(path)] = hashlib.sha256(path.read_bytes()).hexdigest() if path.is_file() else None
    return bindings, pins


def request_key(request):
    return (request['item'], request['kind'], request.get('group'), request.get('path'))


def requeue_changed_gates(root, fdir, receipt):
    """Retain changed selections for a fresh gate pass; rebuild the rest normally."""
    if 'gate_dependencies' not in receipt:
        # Old receipts lack scoped dependencies. Reproduce acceptance from live gates
        # instead of publishing their old candidate without its original guard.
        receipt['attempts'].append(receipt['integrated'])
        receipt['integrated'] = None
        save_run(fdir, receipt)
        return True
    affected = []
    for dependency in receipt.get('gate_dependencies', []):
        try:
            groups_changed = any(signature(fdir, name) != pinned
                                 for name, pinned in dependency['bindings'].items())
            files_changed = any((hashlib.sha256(Path(path).read_bytes()).hexdigest()
                                 if Path(path).is_file() else None) != digest
                                for path, digest in dependency['pins'].items())
        except (f.FlowError, OSError, ValueError, KeyError, TypeError):
            groups_changed, files_changed = True, True
        if groups_changed or files_changed:
            affected.append(dependency['request'])
    if not affected:
        return False
    with state_lock(fdir):
        for request in affected:
            item = next(i for i in receipt['items'] if i['id'] == request['item'])
            selection = [request['group']] if request.get('group') else item['selection']
            retry = enqueue(Path(item['root']), fdir, item['kind'], selection, item['branch'],
                            item['approved_only'], item['with_metadata'],
                            name='gate-changed-' + receipt['id'] + '-' + hashlib.sha256(
                                json.dumps(selection).encode()).hexdigest()[:12])
            receipt.setdefault('gate_dropped', []).append(dict(request=request, reason='gate-changed: requeue',
                                                               requeued=retry['id']))
        receipt['attempts'].append(receipt['integrated'])
        receipt.update(integrated=None, accepted=[])
    save_run(fdir, receipt)
    return True


def prepare(item, frozen):
    root = Path(item['root'])
    data = f.inventory(root)
    requests, bindings = [], {}
    if item['kind'] == 'family':
        path = item['selection'][0]
        batch = f.fr.read(root, path)
        family = f.fr.read(root, batch['family_path'])
        # The unchanged acceptance function repeats every family and sampling check on the final tree.
        f.fr.validate_family(family)
        f.fr.validate_batch(batch, family)
        f.fr.private_approval(root, family, f.session_profile(frozen))
        f.fr.ordering(root, path, batch)
        by_id = {fn['id']: fn for fn in data['functions']}
        required = set(batch['sample'])
        for fid in batch['population']:
            fn = by_id[fid]
            record = f.fr.read(root, fn['report'])
            if fn['status'] == 'matched' or not f.fr.in_ranges(record.get('family_parameters'), family['parameter_ranges']):
                required.add(fid)
        functions = [f.hexrva(by_id[fid]['rva']) for fid in batch['population'] if fid not in required]
        requests.append(dict(kind='family', path=path, functions=functions,
                             build_id=family['binding']['build_id'], profile=f.session_profile(frozen)))
    elif item['kind'] == 'label':
        ids = f.af.resolve_selection(root, data, item['selection'])
        selected = [fn for fn in data['functions'] if fn['id'] in ids]
        f.refuse(len({fn['build_id'] for fn in selected}) == 1, 'label request must name one build')
        requests.append(dict(kind='label', functions=[f.hexrva(fn['rva']) for fn in selected],
                             build_id=selected[0]['build_id']))
    else:
        for name in item['selection']:
            group = f.load_group(frozen, name)
            f.refuse(group['kind'] == 'review', 'integrate requires an individual review group')
            status, blocks = f.read_review(root, group, data['functions'])
            f.refuse(status == 'ok', f'{name}: saved review {status}')
            esc = [r for r, b in blocks.items() if b.get('verdict') == 'escalate'
                   and b.get('escalation_category') in f.BLOCKING_ESCALATIONS]
            f.refuse(item['approved_only'] or not esc, f'{name}: blocking escalation; group held')
            approved = sorted(r for r, b in blocks.items() if b.get('verdict') == 'approve'
                              and (not group.get('subset_of') or r in group['functions']))
            f.refuse(approved, f'{name}: no approvals')
            ok, why = f.sampling_gate(root, frozen, group, data, approved)
            f.refuse(ok, f'{name}: sampling gate: {why}')
            if item['kind'] == 'groups':
                f.refuse(set(approved) == set(group['functions']), f'{name}: every member needs approval')
            f.family_group_hold(root, frozen, [fn['id'] for fn in data['functions']
                                              if fn['build_id'] == group['build_id'] and f.hexrva(fn['rva']) in approved])
            bindings[name] = signature(frozen, name)
            if group.get('pair'):
                bindings[group['pair']] = signature(frozen, group['pair'])
            requests.append(dict(kind='review', group=name, review=str(f.review_path(root, group).resolve()),
                                 functions=approved, build_id=group['build_id'],
                                 profile=group.get('profile', f.profiles.DEFAULT), harvests=group.get('harvests'),
                                 record_refresh=group.get('record_refresh', []),
                                 **({'contributor': group['contributor']} if group.get('contributor') else {})))
    return requests, bindings


def accept(tree, requests, python):
    request_file, result_file = tree / '.local/lane-requests.json', tree / '.local/lane-result.json'
    write(request_file, requests)
    f.run([python, str(f.TOOLS / 'lane_accept.py'), '--root', str(tree), '--requests', str(request_file),
           '--result', str(result_file)], tree, 'lane acceptance',
          env=dict(os.environ, SKYRIM_FLOW_DIR=str(tree / '.local/flow')))
    return read(result_file)


def save_run(fdir, receipt):
    for field in ('dropped', 'gate_dropped', 'tooling_dropped'):
        for item in receipt.get(field, []):
            if item.get('reason'):
                item['next'] = agent_advice.next_action(item['reason'])
    path = fdir / 'lane/runs' / (receipt['id'] + '.json')
    flow_heartbeat.watch(fdir, path, receipt.get('owner'))
    compact = compact_receipt(path, receipt)
    with state_lock(fdir):
        for item in receipt['items']:
            write(fdir / 'lane/queue' / (item['id'] + '.json'), item)
        import flow_tooling
        flow_tooling.save_items(fdir, receipt, receipt.get('state') == 'done')
        compact_tooling(compact, receipt)
        write(path, flow_heartbeat.preserve(path, compact))
        integ = receipt.get('integrated')
        if integ:
            group_members = {}
            for request in receipt.get('accepted', []):
                if request.get('group'):
                    group_members.setdefault(request['group'], set()).update(request['functions'])
            for name, members in group_members.items():
                group = f.load_group(fdir, name)
                group['integrated'] = dict(integ, accepted=len(members), lane=receipt['id'])
                f.save_group(fdir, group)


def _source_files(root, request):
    by_rva = {f.hexrva(fn['rva']): fn for fn in f.inventory(root)['functions']
              if fn['build_id'] == request['build_id']}
    paths, headers, compiled = set(), set(), set()
    for rva in request['functions']:
        fn = by_rva[rva]
        paths.add(fn['report'])
        if fn.get('status') == 'matched':
            review = Path(fn['report']).with_suffix('.review.json').as_posix()
            if (root / review).is_file():
                paths.add(review)
        record = f.fr.read(root, fn['report'])
        paths.update(record['source']['translation_unit'])
        headers.update(record['source']['translation_unit'])
        compiled.add(record['source']['compiled'])
        if record.get('symbol_map', {}).get('path'):
            name = record['symbol_map']['path']
            paths.add(name)
            if (root / f.history_name(name)).exists():
                paths.add(f.history_name(name))
    paths.update(refreshed_records(root, request, by_rva))
    return by_rva, paths, headers - compiled


def refreshed_records(root, request, by_rva):
    if not request.get('record_refresh'):
        return []
    allowed = {f.fr.read(root, by_rva[rv]['report'])['source']['compiled'] for rv in request['functions']}
    selected = {fn['id']: fn for fn in by_rva.values()}
    _, target = f.af.load(root)
    records = []
    for fid in request['record_refresh']:
        fn = selected[fid]
        record = f.fr.read(root, fn['report'])
        if record['source']['compiled'] not in allowed:
            continue
        f.refuse(fn['status'] != 'matched', 'matched companion needs its own review selection')
        f.af.validate_match(dict(fn, status='matched'), target, root)
        records.append(fn['report'])
    return records


def carry_request(source, tree, request, by_rva):
    companions = refreshed_records(source, request, by_rva)
    files = f.carry_into_tree(source, tree, request['functions'], by_rva, request.get('harvests'),
                              request.get('enrollments'))
    for name in companions:
        destination = tree / name
        destination.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(source / name, destination)
    return sorted(set(files) | set(companions))


def copy_private_review(source, tree, review, name=None):
    """Preserve review bytes and containment without inheriting immutable input modes."""
    review = f.review_path(source, dict(saved_review=str(review)))
    relative = review.resolve().relative_to((Path(source) / f.REVIEWS).resolve())
    destination = Path(tree) / f.REVIEWS / (name if name is not None else relative)
    destination = f.review_path(tree, dict(saved_review=str(destination)))
    destination.parent.mkdir(parents=True, exist_ok=True)
    if destination.exists():
        f.refuse(destination.read_bytes() == review.read_bytes(),
                 'conflicting saved review: ' + str(relative))
    else:
        shutil.copyfile(review, destination)
    return destination


def stale_sibling_members(tree, baseline, requests):
    """Attribute stale existing units to carried members; the whole-tree guard is unchanged.

    Inspect each distinct matched unit/digest once, not once per incoming member. Both old and
    current include closures matter when a carried header changes another unit's dependencies.
    Family selections stay atomic because their committed population/draw cannot be pruned.
    """
    changed = set(f.git(tree, 'diff', '--name-only', 'HEAD', '--', 'reconstruction').splitlines())
    changed.update(f.git(tree, 'ls-files', '--others', '--exclude-standard', '--', 'reconstruction').splitlines())
    if not changed:
        return requests, []
    current = f.inventory(tree)
    by_id = {fn['id']: fn for fn in current['functions']}
    by_address = {(fn['build_id'], f.hexrva(fn['rva'])): fn for fn in current['functions']}
    records, checked, affected = {}, set(), {}
    def source(fn):
        path = fn.get('report')
        if path not in records:
            records[path] = (f.fr.read(tree, path).get('source') or {}) if path else {}
        return records[path]
    with source_digest_cache():
        for original in baseline['functions']:
            if original['status'] != 'matched':
                continue
            fn = by_id[original['id']]
            src = source(fn)
            unit = src.get('translation_unit')
            if not isinstance(unit, list) or not all(isinstance(p, str) for p in unit):
                continue  # malformed records remain the ordinary validator's responsibility
            key = (src.get('compiled'), src.get('digest'), tuple(unit))
            if key in checked:
                continue
            checked.add(key)
            if f.af.acceptance_staleness(fn, tree) != check_gate.STALE_UNIT:
                continue
            dependencies = set(unit)
            try:
                dependencies.update(check_gate.include_closure(tree, src['compiled']))
            except (ValueError, OSError):
                pass  # retain the recorded dependencies; the final guard still checks the closure
            for name in dependencies & changed:
                affected.setdefault(name, set()).add(src['compiled'])
    if not affected:
        return requests, []
    accepted, dropped = [], []
    for request in requests:
        if not request['functions']:
            accepted.append(request)
            continue
        good, bad = [], []
        for rva in request['functions']:
            src = source(by_address[request['build_id'], rva])
            touched = set(src.get('translation_unit') or ()) & affected.keys()
            if touched:
                bad.append(dict(request=dict(request, functions=[rva]), reason='stale-sibling: needs re-confirm',
                                units=[src['compiled']], changed_paths=sorted(touched),
                                stale_sibling_units=sorted({u for name in touched for u in affected[name]})))
            else:
                good.append(rva)
        if bad and request['kind'] == 'family':
            dropped.append(dict(request=request, reason='stale-sibling: needs re-confirm',
                                units=sorted({u for d in bad for u in d['units']}),
                                changed_paths=sorted({p for d in bad for p in d['changed_paths']}),
                                stale_sibling_units=sorted({u for d in bad for u in d['stale_sibling_units']})))
        else:
            dropped.extend(bad)
            if good:
                accepted.append(dict(request, functions=good))
    return accepted, dropped


def admit_enrollments(tree, source, enrollments):
    """Refuse an item whose new inventory rows cite evidence the published tree will not hold.

    The whole-inventory validator fails on any row with missing evidence, so one bad row from
    one queued item would otherwise reject every other item in the run. Evidence is never
    carried by the lane: it has to be committed content of the tree being published, and an
    untracked file in the input checkout does not count.
    """
    for fn in enrollments:
        evidence = fn.get('evidence')
        parts = Path(evidence).parts if isinstance(evidence, str) and evidence else ()
        f.refuse(parts and not Path(evidence).is_absolute() and '..' not in parts
                 and parts[0] in ('findings', 'reports'),
                 f'{fn["id"]}: enrollment evidence {evidence!r} is not a public findings/reports path')
        tracked = subprocess.run(['git', 'cat-file', '-e', 'HEAD:' + evidence], cwd=tree,
                                 capture_output=True).returncode == 0
        f.refuse(tracked, f'{fn["id"]}: Evidence {evidence} is not committed content of the '
                          'integration tree (untracked or missing in the input checkout); commit and '
                          'publish it, or pass --with-metadata, then re-queue')
        f.refuse(fn.get('report') and (Path(source) / fn['report']).is_file(),
                 f'{fn["id"]}: match record {fn.get("report")} is missing from the input checkout')


def merge_metadata(tree, source, revision):
    # integration_base validated the metadata-only history before this transport step.
    f.git(tree, 'fetch', '--quiet', str(source), revision)
    import flow_derived
    flow_derived.merge(tree, revision, no_ff=False)


def contributor_history(tree, source, contribution):
    """Validate what a contributor publication retains, without checking out PR code.

    Only accepted requests reach this path. The Engineer's AI screen and fresh reviews are
    recorded in the pinned group; this repeats the mechanical scope/resource checks for the
    exact parent being published. The acceptance tree still contains only carried inputs.
    """
    import re
    from contrib_publication import SHA
    fields = {'head', 'base', 'authors', 'paths'}
    f.refuse(isinstance(contribution, dict) and set(contribution) in (fields, fields | {'merge_method'}),
             'invalid contributor ancestry metadata')
    method = contribution.get('merge_method', 'merge')  # Frozen legacy groups retain their history guard.
    f.refuse(method in ('merge', 'squash'), 'unsupported contributor merge method')
    head, base = contribution['head'], contribution['base']
    f.refuse(isinstance(head, str) and SHA.fullmatch(head) and isinstance(base, str) and SHA.fullmatch(base),
             'invalid contributor ancestry revisions')
    f.git(tree, 'fetch', '--quiet', str(source), head)
    from contrib_publication import PublicationRejected, screen_publication
    try:
        publication = screen_publication(tree, base, head, merge_method=method,
            git=lambda *args: f.drafts.git(tree, *args))
    except PublicationRejected as error:
        raise f.FlowError(str(error)) from None
    paths = [item['path'] for item in publication['files']]
    f.refuse(sorted(paths) == sorted(contribution['paths']),
             'contributor ancestry changes files outside its screened scope')
    authors = sorted(set(f.git(tree, 'log', '--format=%an <%ae>', f'{base}..{head}').splitlines()))
    f.refuse(authors and authors == contribution['authors'] and all(
        re.fullmatch(r'[^<>\r\n]{1,100} <[^<>\s]+@[^<>\s]+>', a) for a in authors),
        'contributor attribution differs from original commits')
    return head, authors


def commit_acceptance(tree, requests, items, message, commit_date):
    """Squash outside tip snapshots with credit; retain history only for explicit merge groups.

    A squash retains the contributor identity but no contributor parent. Legacy merges retain
    exact screened commits. Neither checks out PR trees or runs their hooks. This also supports
    partially accepted PRs. Source acceptance and the staged guard have already passed unchanged.
    """
    contributions = {}
    for request in requests:
        if request.get('contributor'):
            value = request['contributor']
            source = Path(next(i['root'] for i in items if i['id'] == request['item']))
            head, authors = contributor_history(tree, source, value)
            contributions[head] = dict(authors=authors, method=value.get('merge_method', 'merge'))
    env = dict(os.environ, GIT_COMMITTER_DATE=commit_date)
    if not contributions:
        f.run(['git', 'commit', '--quiet', '-m', message], tree, 'git commit', env=env)
        return
    previous = f.git(tree, 'rev-parse', 'HEAD')
    parents = [previous] + [h for h in sorted(contributions)
                            if h != previous and contributions[h]['method'] == 'merge']
    trailers = sorted({a for value in contributions.values() for a in value['authors']})
    message += '\n\n' + '\n'.join('Co-authored-by: ' + author for author in trailers) + '\n'
    command = ['git', 'commit-tree', f.git(tree, 'write-tree')]
    for parent in parents:
        command += ['-p', parent]
    result = subprocess.run(command, cwd=tree, env=env, input=message, capture_output=True, text=True)
    f.refuse(result.returncode == 0, 'contributor merge commit failed')
    f.git(tree, 'update-ref', 'HEAD', result.stdout.strip(), previous)


def nothing_published(receipt):
    """True for a run that consumed queued work and published none of it."""
    return (isinstance(receipt, dict) and receipt.get('state') == 'done' and bool(receipt.get('items'))
            and not (receipt.get('integrated') or {}).get('pushed'))


def log_all_dropped(fdir, receipt):
    f.log_event(Path(fdir), 'lane-all-dropped', lane=receipt['id'],
                dropped=len(receipt.get('dropped', [])),
                reasons=sorted({str(d.get('reason', ''))[:200] for d in receipt.get('dropped', [])})[:5])


def build(root, fdir, receipt, python, branch, soft_free_gb=None, allow_low_disk=False, hard_free_gb=None):
    try:
        resource_admission.admit(fdir, 'lane build', hard_gb=hard_free_gb,
                                 soft_gb=soft_free_gb, allow_low_disk=allow_low_disk)
    except ValueError as error:
        raise f.FlowError(str(error)) from None
    started = time.monotonic()
    stages = receipt['build_wall_seconds'] = {}
    receipt.setdefault('attempts', [])
    attempt = receipt.get('next_tree', 0)
    tree = fdir / 'lane/trees' / receipt['id'] / str(attempt)
    while tree.exists():
        attempt += 1
        tree = tree.with_name(str(attempt))
    receipt['next_tree'] = attempt + 1
    receipt.setdefault('candidates', []).append(str(tree))
    save_run(fdir, receipt)
    tree.parent.mkdir(parents=True, exist_ok=True)
    f.git(root, 'clone', '--quiet', '--shared', '--no-checkout', str(root), str(tree))
    # Local clone's origin points at the source checkout; use the public remote for base/push identity.
    f.git(tree, 'remote', 'set-url', 'origin', f.git(root, 'remote', 'get-url', 'origin'))
    f.git(tree, 'fetch', '--quiet', 'origin', branch)
    base = f.git(tree, 'rev-parse', f'origin/{branch}')
    require_base_tools(tree, branch, base)
    f.git(tree, 'checkout', '--quiet', '--detach', base)
    # A candidate is disposable; successful exact-content memos belong to the lane. Git's
    # local config carries this path to every check/site subprocess without changing inputs.
    f.git(tree, 'config', '--local', 'skyrim.revisionCacheDirectory', str((fdir / 'revision-cache').resolve()))
    stages['clone_fetch_checkout'] = time.monotonic() - started
    for key in ('user.name', 'user.email'):
        value = f.git(root, 'config', key, check=False)
        if value:
            f.git(tree, 'config', key, value)
    import flow_tooling
    began = time.monotonic()
    active_tools, tool_dropped = flow_tooling.merge(tree, receipt.get('tooling', []))
    stages['tooling_merge'] = time.monotonic() - began
    receipt['tooling_accepted'], receipt['tooling_dropped'] = active_tools, tool_dropped
    if active_tools:
        import flow_derived
        began = time.monotonic()
        flow_derived.regenerate(tree, python)
        stages['derived_views'] = time.monotonic() - began
    integrate_started = time.monotonic()
    baseline = f.inventory(tree) if receipt['items'] else dict(functions=[])
    public_ids = {fn['id'] for fn in json.loads(f.git(tree, 'show', base + ':data/progress.json'))['functions']}
    carry_base = f.git(tree, 'rev-parse', 'HEAD')
    frozen = tree / '.local/flow'
    snapshot(fdir, frozen)
    requests, bindings, origins, carried, pins = [], {}, [], set(), {}
    input_pins, review_copies, gate_dependencies = {}, {}, []
    dropped = list(receipt.get('gate_dropped', []))
    excluded = {request_key(d['request']) for d in dropped}
    parts = [dict(item, selection=[name]) if item['kind'] == 'groups' else item
             for item in receipt['items'] for name in (item['selection'] if item['kind'] == 'groups' else [None])]
    for item in parts:
        source = Path(item['root'])
        before_item = f.git(tree, 'rev-parse', 'HEAD')
        try:
            if item['kind'] in ('family', 'label'):
                key = (item['id'], item['kind'], None, item['selection'][0] if item['kind'] == 'family' else None)
                if key in excluded:
                    continue
            elif all((item['id'], 'review', name, None) in excluded for name in item['selection']):
                continue
            selected, _ = prepare(item, frozen)
            selected = [r for r in selected if request_key(dict(r, item=item['id'])) not in excluded]
            if not selected:
                continue
            for review in sorted((source / f.REVIEWS).rglob('*')):
                if not review.is_file():
                    continue
                f.review_path(source, dict(saved_review=str(review)))
                dest = copy_private_review(source, tree, review)
                review_copies[str(review.resolve())] = dest.relative_to(tree).as_posix()

            f.git(source, 'fetch', '--quiet', 'origin', branch)
            metadata_origin = f.git(source, 'merge-base', 'HEAD', f'origin/{branch}') if item['with_metadata'] else None
            pinned_tooling = any(Path(t['root']).resolve() == source.resolve()
                                 and t['revision'] == f.git(source, 'rev-parse', 'HEAD') for t in active_tools)
            # This exact committed tip is already present through an explicit tooling input.
            # Other unpublished commits still go through the unchanged metadata-only policy.
            metadata_base = (f'origin/{branch}' if pinned_tooling else
                             f.integration_base(source, branch, item['with_metadata'], remote=metadata_origin))
            if item['with_metadata'] and metadata_base != f'origin/{branch}':
                merge_metadata(tree, source, metadata_base)
            for request in selected:
                dependencies, evidence = gate_inputs(source, frozen, [request])
                bound = dependencies
                request_pins = dict(evidence)
                input_pins.update(evidence)
                by_rva, paths, headers = _source_files(source, request)
                request['enrollments'] = [dict({k: v for k, v in by_rva[r].items() if k != 'named'}, status='mapped')
                                          for r in request['functions'] if by_rva[r]['id'] not in public_ids]
                for enrollment in request['enrollments']:
                    f.validate_enrollment(enrollment)
                admit_enrollments(tree, source, request['enrollments'])
                f._integration_origin_unchanged(source, f.git(source, 'rev-parse', 'HEAD'), base,
                                               sorted(paths), request['build_id'], request['functions'], headers=headers,
                                               enrollments=request['enrollments'])
                origins.append(dict(root=str(source), base=base, paths=sorted(paths),
                                    build_id=request['build_id'], functions=request['functions'],
                                    entries={r: by_rva[r] for r in request['functions']}))
                # Never silently let one checkout overwrite a conflicting closure from another.
                for rel in paths:
                    digest = hashlib.sha256((source / rel).read_bytes()).hexdigest()
                    f.refuse(rel not in pins or pins[rel] == digest, 'incompatible queued inputs: ' + rel)
                    pins[rel] = digest
                    input_pins[str(source / rel)] = digest
                    request_pins[str(source / rel)] = digest
                carried.update(carry_request(source, tree, request, by_rva))
                request['item'] = item['id']
                if request.get('review'):
                    src = Path(request['review'])
                    dest = copy_private_review(source, tree, src, item['id'] + '-' + src.name)
                    input_pins[str(src)] = hashlib.sha256(src.read_bytes()).hexdigest()
                    request_pins[str(src)] = input_pins[str(src)]
                    request['review'] = str(dest)
                # The validated recursive copy above already preserves family approval filenames.
                requests.append(request)
                bindings.update(bound)
                gate_dependencies.append(dict(request=request, bindings=bound, pins=request_pins))

        except (f.FlowError, f.af.Refused, ValueError, KeyError, TypeError, OSError) as error:
            if (tree / '.git/MERGE_HEAD').exists():
                f.git(tree, 'merge', '--abort', check=False)
            if f.git(tree, 'rev-parse', 'HEAD') != before_item:
                f.git(tree, 'reset', '--hard', before_item)
            dropped.append(dict(item=item['id'], selection=item['selection'], reason=str(error)))
    # Imported groups may name absolute paths in an input's private reviews. The frozen
    # groups must instead refer to their validated copies, including groups not selected
    # for acceptance: post-CI claims/status reads the entire frozen flow state.
    for group in f.groups(frozen).values():
        saved = group.get('saved_review')
        if saved and Path(saved).is_absolute():
            copied = review_copies.get(str(Path(saved).resolve()))
            if copied:
                group['saved_review'] = copied
                f.save_group(frozen, group)
    if not requests and not active_tools and not receipt.get('baseline_check'):
        receipt.update(dropped=dropped, state='done')
        for item in receipt['items']:
            item['state'] = 'dropped'
        save_run(fdir, receipt)
        log_all_dropped(fdir, receipt)
        return None
    carry_base = f.git(tree, 'rev-parse', 'HEAD')

    def recarry(selected):
        f.git(tree, 'reset', '--hard', carry_base)
        for rel in carried:
            if f.git(tree, 'ls-files', '--', rel) == '':
                (tree / rel).unlink(missing_ok=True)
        for request in selected:
            source = Path(next(i['root'] for i in receipt['items'] if i['id'] == request['item']))
            by_rva, _, _ = _source_files(source, request)
            carry_request(source, tree, request, by_rva)

    # Remove any partial carry of a request that failed before acceptance.
    if dropped:
        recarry(requests)
    while requests:
        remaining, stale = stale_sibling_members(tree, baseline, requests)
        if not stale:
            break
        dropped.extend(stale)
        requests = remaining
        recarry(requests)
    result = accept(tree, requests, python) if requests else dict(accepted=[], dropped=[], changed=[])
    receipt['accept_wall_seconds'] = result.get('wall_seconds', {})
    if not result['accepted'] and not active_tools and not receipt.get('baseline_check'):
        receipt.update(dropped=dropped + result['dropped'], state='done')
        for item in receipt['items']:
            item['state'] = 'dropped'
        save_run(fdir, receipt)
        log_all_dropped(fdir, receipt)
        return None
    if result['dropped']:
        recarry(result['accepted'])
        failures = result['dropped']
        result = accept(tree, result['accepted'], python)
        receipt['accept_recarry_wall_seconds'] = result.get('wall_seconds', {})
        f.refuse(not result['dropped'], 'accepted member changed during re-carry')
        result['dropped'] = failures
    f.refuse(result['accepted'] or active_tools or receipt.get('baseline_check'), 'no member passed acceptance')
    accepted_by_key = {}
    for request in result['accepted']:
        key = request_key(request)
        if key not in accepted_by_key:
            accepted_by_key[key] = dict(request, functions=[])
        accepted_by_key[key]['functions'].extend(request['functions'])
    gate_dependencies = [dict(d, request=accepted_by_key[request_key(d['request'])]) for d in gate_dependencies
                         if request_key(d['request']) in accepted_by_key]
    bindings = {name: value for d in gate_dependencies for name, value in d['bindings'].items()}
    input_pins = {path: value for d in gate_dependencies for path, value in d['pins'].items()}
    receipt.update(accepted=result['accepted'], dropped=dropped + result['dropped'], bindings=bindings,
                   gate_dependencies=gate_dependencies,
                   origins=origins, input_pins=input_pins, tree=str(tree))
    if not result['accepted']:
        stages['integrate'] = time.monotonic() - integrate_started
        receipt['integrated'] = dict(commit=carry_base, accepted=0, files=[], checks=None, pushed=None, ci=None,
                                    wall_seconds={'build': time.monotonic()-started}, base_revision=base)
        if carry_base == base and active_tools and not receipt.get('baseline_check'):
            # Already-published no-op: no pre-push checks follow, so retain its site check here.
            f.run([python, 'tools/build_site.py'], tree, 'build_site.py')
            receipt['integrated'].update(checks=f.utc(), pushed=f.utc())
        save_run(fdir, receipt)
        return tree
    f.run([python, 'tools/build_progress_svg.py'], tree, 'build_progress_svg.py')
    if (tree / 'tools/build_leaderboard.py').is_file():
        f.run([python, 'tools/build_leaderboard.py'], tree, 'build_leaderboard.py')
    paths = carried | set(result['changed']) | {'data/progress.json', 'reconstruction/INDEX.md',
                                              'docs/progress.svg', 'docs/progress-map.svg',
                                              'LEADERBOARD.md', 'data/contributors.json'}
    files = f.changed(tree, paths)
    f.refuse(files, 'nothing changed to commit')
    f.git(tree, 'add', '--', *files)
    commit_date = f.dt.datetime.now(f.dt.timezone.utc).replace(microsecond=0).isoformat()
    f.run([python, 'tools/build_model_trend.py', '--pending-date', commit_date], tree, 'build_model_trend.py')
    trend = ['README.md', 'docs/model-trend.svg', 'docs/progress-trend.svg']
    f.git(tree, 'add', '--', *trend)
    stages['integrate'] = time.monotonic() - integrate_started
    began = time.monotonic()
    f.run([python, 'tools/check.py', '--staged'], tree, 'check.py --staged')
    stages['staged_guard'] = time.monotonic() - began
    began = time.monotonic()
    commit_acceptance(tree, result['accepted'], receipt['items'],
                      f'Accept queued work in push lane {receipt["id"]}', commit_date)
    stages['commit'] = time.monotonic() - began
    integ = dict(commit=f.git(tree, 'rev-parse', 'HEAD'), accepted=sum(len(r['functions']) for r in result['accepted']),
                 files=sorted(set(files) | set(trend)), checks=None, pushed=None, ci=None,
                 wall_seconds={'build': time.monotonic() - started}, base_revision=base)
    receipt['integrated'] = integ
    save_run(fdir, receipt)
    return tree


def reparent_tree_neutral(tree, receipt, integ, destination):
    """Move a checked commit onto a new origin head whose tree equals the old base's.

    Contributor history merges (-s ours) move origin without touching a file, so the staged
    checks stay valid; rebuilding would only repeat them. The new commit keeps the same tree,
    message, author and extra parents and is still pushed and CI-checked as its own SHA.
    """
    base, commit = integ['base_revision'], integ['commit']
    if f.git(tree, 'rev-parse', destination + '^{tree}') != f.git(tree, 'rev-parse', base + '^{tree}'):
        return False
    if subprocess.run(['git', 'merge-base', '--is-ancestor', base, destination], cwd=tree).returncode != 0:
        return False
    if f.git(tree, 'rev-parse', commit + '^') != base:
        return False
    parents = f.git(tree, 'rev-list', '--parents', '-n', '1', commit).split()[1:]
    fields = f.git(tree, 'log', '-1', '--format=%an%x00%ae%x00%aI', commit).split('\0')
    env = dict(os.environ, GIT_AUTHOR_NAME=fields[0], GIT_AUTHOR_EMAIL=fields[1], GIT_AUTHOR_DATE=fields[2])
    message = subprocess.run(['git', 'log', '-1', '--format=%B', commit], cwd=tree, capture_output=True, text=True).stdout
    command = ['git', 'commit-tree', commit + '^{tree}', '-p', destination]
    for parent in parents[1:]:
        command += ['-p', parent]
    result = subprocess.run(command, cwd=tree, env=env, input=message, capture_output=True, text=True)
    f.refuse(result.returncode == 0, 'could not re-parent the checked commit: ' + result.stderr.strip())
    new = result.stdout.strip()
    f.git(tree, 'update-ref', 'HEAD', new, commit)
    receipt.setdefault('attempts', []).append(dict(integ, superseded_by_reparent=new))
    integ.update(commit=new, base_revision=destination, reparented_from=commit)
    return True


def disk_preflight(fdir, min_free_gb=None):
    """Refuse expensive lane work below the configured free-space threshold (decimal GB)."""
    setting = min_free_gb if min_free_gb is not None else os.environ.get('SKYRIM_LANE_MIN_FREE_GB', '3')
    try:
        threshold = float(setting)
    except (TypeError, ValueError):
        raise f.FlowError('minimum free disk space must be a finite nonnegative number of GB') from None
    f.refuse(math.isfinite(threshold) and threshold >= 0,
             'minimum free disk space must be a finite nonnegative number of GB')
    path = Path(fdir)
    while not path.exists():
        path = path.parent
    free = shutil.disk_usage(path).free / 1_000_000_000
    f.refuse(free >= threshold,
             f'insufficient free disk space for push lane: {free:.2f} GB free; need {threshold:g} GB. '
             'Free disk space, then retry; keep machine-specific paths in private configuration.')


@flow_heartbeat.publisher
def run_lane(root, fdir, branch='main', push=True, ci_wait=600, python=sys.executable, min_free_gb=None,
             soft_free_gb=None, allow_low_disk=False, resume=None, wait_lane=False, sync_claims=True):
    root, fdir = Path(root).resolve(), Path(fdir).resolve()
    require_base_tools(root, branch)
    import flow_cleanup
    if fdir.is_dir():
        flow_cleanup.automatic_retention(root, fdir)
    python = lane_python(python)
    if not resume and push and fdir.is_dir():
        import flow_reconcile
        recovered = flow_reconcile.reconcile(root, fdir, branch, python)['resume']
        resume = recovered[0] if recovered else None
    if resume and receipt_path(fdir, resume).parent.name == 'tooling':
        f.refuse(push, 'tooling resume requires push/CI mode; use tooling-push --resume ' + resume)
        return publish_tooling(root, fdir, branch, ci_wait, python, resume=resume,
                               soft_free_gb=soft_free_gb, allow_low_disk=allow_low_disk)
    if resume:
        with state_lock(fdir):
            previous = read(receipt_path(fdir, resume))
        if (previous.get('integrated') or {}).get('pushed'):
            return finish_run(root, fdir, previous, python, ci_wait, sync_claims=sync_claims)
    disk_preflight(fdir, min_free_gb)
    wait_for_owner(root, fdir, 'runs', resume, wait_lane)
    wait_for_owner(root, fdir, 'tooling', None, wait_lane)
    with lane_lock(fdir, wait=wait_lane, purpose='integration') as lease:
        require_base_tools(root, branch)
        recover_stale(fdir)
        with state_lock(fdir):
            require_complete(fdir)
            unfinished = [read(p) for p in sorted((fdir / 'lane/runs').glob('*.json')) if read(p)['state'] == 'running' and read(p).get('kind') != 'tooling-request'
                       and not (read(p).get('integrated') or {}).get('pushed')]
            unfinished = [r for r in unfinished if r.get('root') == str(root)]
            if resume:
                selected = read(receipt_path(fdir, resume))
                f.refuse(selected['state'] in ('running', 'failed', 'conflict', 'stale'), 'receipt cannot be resumed')
                unfinished = [selected]
            f.refuse(len(unfinished) <= 1, 'multiple unfinished lane runs')
            if unfinished:
                receipt = unfinished[0]
                f.refuse(not resume or receipt['id'] == resume, 'resume id is not the unfinished lane run')
                f.refuse(receipt['branch'] == branch, 'resume lane with the same branch')
            else:
                f.refuse(not resume, 'no unfinished lane run with that id')
                items = [read(p) for p in sorted((fdir / 'lane/queue').glob('*.json')) if read(p)['state'] == 'queued']
                import flow_tooling
                tooling = [read(p) for p in sorted((fdir / 'lane/tooling-queue').glob('*.json'))
                           if read(p)['state'] == 'queued' and read(p)['branch'] == branch]
                tooling.sort(key=lambda i: (i['queued_at'], i['id']))
                if not items and not tooling:
                    return dict(state='empty')
                f.refuse(all(i['branch'] == branch for i in items), 'queued requests name different branches')
                receipt = dict(id=uuid.uuid4().hex[:16], root=str(root), branch=branch, items=items, tooling=tooling, state='running', integrated=None)
                for item in items:
                    item['state'], item['run'] = 'running', receipt['id']
                save_run(fdir, receipt)
            guard_cleanup(fdir, root)
            if receipt.get('tree'):
                guard_cleanup(fdir, receipt['tree'])
            receipt['owner'] = process_owner()
            receipt.update(state='running', python=python)
            receipt.pop('error', None)
            save_run(fdir, receipt)
        try:
            while True:
                if not receipt.get('integrated'):
                    try:
                        tree = build(root, fdir, receipt, python, branch, soft_free_gb, allow_low_disk, min_free_gb)
                    except f.FlowError as error:
                        if not receipt.get('tooling_accepted'):
                            raise
                        import flow_tooling
                        flow_tooling.isolate_failure(root, fdir, receipt, python, branch, error,
                                                    soft_free_gb, allow_low_disk, min_free_gb)
                        tree = receipt.get('tree')
                    if tree is None or receipt['state'] == 'done':
                        return receipt
                tree, integ = Path(receipt['tree']), receipt['integrated']
                # Avoid checking an obsolete candidate when resuming after a failed check.
                if not integ['pushed']:
                    f.git(tree, 'fetch', '--quiet', 'origin', branch)
                    destination = f.git(tree, 'rev-parse', f'origin/{branch}')
                    require_base_tools(tree, branch, destination)
                    if destination != integ['base_revision']:
                        if reparent_tree_neutral(tree, receipt, integ, destination):
                            save_run(fdir, receipt)
                        else:
                            for origin in receipt['origins']:
                                f._integration_origin_unchanged(tree, origin['base'], destination, origin['paths'],
                                                              origin['build_id'], origin['functions'])
                            receipt['attempts'].append(integ)
                            receipt['integrated'] = None
                            save_run(fdir, receipt)
                            continue
                if not integ['checks']:
                    started = time.monotonic()
                    logs = fdir / 'lane/logs' / receipt['id'] / str(len(receipt.get('attempts', [])))
                    receipt['checks_log_dir'] = str(logs)
                    save_run(fdir, receipt)
                    try:
                        f.pre_push_checks(tree, integ['commit'], python, candidate=True, log_dir=logs,
                                          base_revision=integ['base_revision'])
                    except f.FlowError as error:
                        if not receipt.get('tooling_accepted'):
                            raise
                        import flow_tooling
                        flow_tooling.isolate_failure(root, fdir, receipt, python, branch, error,
                                                    soft_free_gb, allow_low_disk, min_free_gb)
                        if receipt['state'] == 'done':
                            return receipt
                        tree, integ = Path(receipt['tree']), receipt['integrated']
                    integ['checks'] = f.utc()
                    integ['wall_seconds']['checks'] = time.monotonic() - started
                    save_run(fdir, receipt)
                if not push:
                    return receipt
                if not integ['pushed']:
                    require_green(tree, integ['base_revision'], python, ci_wait)
                    # Changed evidence drops/requeues only its dependants. The fresh build
                    # revalidates every retained member through the unchanged current gates.
                    if requeue_changed_gates(root, fdir, receipt):
                        continue
                    for origin in receipt['origins']:
                        current = {f.hexrva(fn['rva']): fn for fn in f.inventory(Path(origin['root']))['functions']
                                   if fn['build_id'] == origin['build_id']}
                        f.refuse(all(current.get(r) == row for r, row in origin['entries'].items()),
                                 'selected inventory changed during lane run')
                    f.git(tree, 'fetch', '--quiet', 'origin', branch)
                    destination = f.git(tree, 'rev-parse', f'origin/{branch}')
                    require_base_tools(tree, branch, destination)
                    if destination != integ['base_revision']:
                        if reparent_tree_neutral(tree, receipt, integ, destination):
                            save_run(fdir, receipt)
                        else:
                            for origin in receipt['origins']:
                                f._integration_origin_unchanged(tree, origin['base'], destination, origin['paths'],
                                                              origin['build_id'], origin['functions'])
                            receipt['attempts'].append(integ)
                            receipt['integrated'] = None
                            save_run(fdir, receipt)
                            continue
                    f.refuse(f.git(tree, 'rev-parse', 'HEAD') == integ['commit']
                             and not f.git(tree, 'status', '--porcelain', '--untracked-files=no'),
                             'candidate changed after checks; refusing push')
                    try:
                        started = time.monotonic()
                        f.run(['git', 'push', 'origin', f'{integ["commit"]}:refs/heads/{branch}'], tree, 'git push')
                    except f.FlowError as error:
                        if 'non-fast-forward' not in str(error) and 'fetch first' not in str(error):
                            raise
                        # The next iteration observes the winner and rebuilds through unchanged gates.
                        continue
                    integ['pushed'] = f.utc()
                    integ['wall_seconds']['push'] = time.monotonic() - started
                    save_run(fdir, receipt)
                lease.release()
                receipt['lane_hold_seconds'] = lease.seconds
                save_run(fdir, receipt)
                return finish_run(root, fdir, receipt, python, ci_wait, sync_claims=sync_claims)

        except BaseException as error:
            if (receipt.get('integrated') or {}).get('pushed'):
                raise
            receipt['error'] = str(error)
            save_run(fdir, receipt)
            raise


@flow_heartbeat.publisher
@ci_serialized('runs')
def finish_run(root, fdir, receipt, python, ci_wait, *, sync_claims=True):
    tree, integ = Path(receipt['tree']), receipt['integrated']
    integ.setdefault('accepted', sum(len(r.get('functions', [])) for r in receipt.get('accepted', [])))
    branch = receipt['branch']
    receipt['owner'] = process_owner()
    save_run(fdir, receipt)
    if integ['ci'] != 'green':
        flow_heartbeat.phase('exact-SHA CI')
        started = time.monotonic()
        result = subprocess.run([python, 'tools/ci_status.py', '--without-advisory', '--commit', integ['commit'], '--wait', str(ci_wait)],
                                cwd=tree, capture_output=True, text=True)
        log = fdir / 'lane/logs' / receipt['id'] / 'ci.log'
        with state_lock(fdir):
            atomic_write(log, (result.stdout + result.stderr).encode())
        receipt['ci_log'] = str(log)
        integ['ci'] = {0: 'green', 1: 'red', 2: 'pending'}.get(result.returncode, 'unavailable')
        integ['wall_seconds']['ci'] = time.monotonic() - started
        save_run(fdir, receipt)
        f.refuse(integ['ci'] == 'green', f'CI {integ["ci"].upper()} for {integ["commit"]}: {result.stdout.strip()}')
    # The shared checkout needs the new remote ref so next-actions offers sync.
    f.git(root, 'fetch', '--quiet', 'origin', branch)
    receipt.pop('error', None)
    receipt['state'] = 'done'
    accepted_items = {r['item'] for r in receipt['accepted']}
    for item in receipt['items']:
        item['state'] = 'done' if item['id'] in accepted_items else 'dropped'
    save_run(fdir, receipt)
    f.log_event(fdir, 'integrated', lane=receipt['id'], accepted=integ['accepted'], commit=integ['commit'])
    f.log_event(fdir, 'ci', lane=receipt['id'], result=integ['ci'], commit=integ['commit'])
    if sync_claims and receipt.get('accepted'):
        import claims
        claims.sync_safe(tree)
    receipt.update(finished_owner=receipt.get('owner'), owner=None)
    save_run(fdir, receipt)
    import flow_cleanup
    flow_cleanup.automatic_retention(root, fdir)
    return receipt
