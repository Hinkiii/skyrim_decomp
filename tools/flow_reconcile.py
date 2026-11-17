"""Close lane receipts and tooling requests whose publisher died, using origin as the authority.

A publisher that dies after its commit reached origin leaves a `running` receipt nobody
finishes, and the next publisher has to sort it out by hand. Reconciliation runs at the start
of every lane run and next-actions: a dead owner's work either resumes (its commit is not on
origin) or is retired with the outcome origin shows. Anything undeterminable is left alone.
"""
import json
from pathlib import Path
import socket
import subprocess
import sys

import flow as f
import flow_lane as lane
from flow_state import state_lock


def on_origin(roots, commit, branch):
    """True/False when some checkout knows the commit, None when none does (fail closed)."""
    if not isinstance(commit, str) or not commit:
        return None
    for root in roots:
        root = Path(root)
        if not root.is_dir() or subprocess.run(['git', 'cat-file', '-e', commit + '^{commit}'],
                                               cwd=root, capture_output=True).returncode:
            continue
        reached = subprocess.run(['git', 'merge-base', '--is-ancestor', commit, 'origin/' + branch],
                                 cwd=root, capture_output=True)
        if reached.returncode in (0, 1):
            return reached.returncode == 0
    return None


def origin_ci(root, commit, python):
    """The exact-SHA CI verdict as origin reports it right now; never waits."""
    try:
        result = subprocess.run([python, str(f.TOOLS / 'ci_status.py'), '--without-advisory', '--commit', commit],
                                cwd=root, capture_output=True, text=True, timeout=120)
    except (OSError, subprocess.SubprocessError):
        return 'unavailable'
    return {0: 'green', 1: 'red', 2: 'pending'}.get(result.returncode, 'unavailable')


def dead_local(owner):
    """Only a recorded owner on this host whose process is gone counts as dead."""
    return bool(owner) and owner.get('host') == socket.gethostname() and not lane.owner_alive(owner)


def reconcile_requests(root, fdir, branch, report):
    events = {e.get('request') for e in f.events(fdir) if e.get('event') == 'tooling-outcome'}
    for path in sorted((fdir / 'lane/tooling').glob('*.json')):
        request = lane.read(path)
        if request.get('kind') != 'tooling-request' or request.get('state') != 'running':
            continue
        if request['id'] in events or request.get('branch', branch) != branch:
            continue
        run = None
        if request.get('run'):
            try:
                run = lane.read(lane.receipt_path(fdir, request['run']))
            except (f.FlowError, OSError, ValueError):
                run = None
        if not dead_local(request.get('owner')) or (run and lane.owner_alive(run.get('owner'))):
            continue
        landed = on_origin([root, request.get('root')], request.get('revision'), branch)
        if landed is None:
            report['skipped'].append(request['id'])
        elif landed:
            request.update(state='done', reconciled_at=f.utc(), owner=None, error=None,
                           integrated=dict((request.get('integrated') or {}), commit=request['revision'],
                                           pushed=f.utc(), reconciled=True))
            queued = fdir / 'lane/tooling-queue' / (request['id'] + '.json')
            lane.write(queued, {k: v for k, v in request.items() if k not in ('kind', 'owner')})
            lane.write(path, request)
            f.log_event(fdir, 'tooling-outcome', request=request['id'], lane=request.get('run'),
                        revision=request['revision'], result='done', reconciled=True,
                        commit=request['revision'], reason='revision is on origin; owner died before recording it')
            report['tooling_retired'].append(request['id'])
        else:
            queued = fdir / 'lane/tooling-queue' / (request['id'] + '.json')
            item = lane.read(queued) if queued.is_file() else {k: v for k, v in request.items() if k != 'kind'}
            item.update(state='queued', recovered_from=request.get('run'), queued_at=f.utc())
            for field in ('run', 'error', 'integrated', 'owner'):
                item.pop(field, None)
            lane.write(queued, item)
            path.unlink()
            f.log_event(fdir, 'tooling-requeued', request=request['id'], lane=request.get('run'),
                        revision=request['revision'], reason='owner died; revision is not on origin')
            report['tooling_requeued'].append(request['id'])


def reconcile_runs(root, fdir, branch, ci_for, report):
    logged = {e.get('lane') for e in f.events(fdir) if e.get('event') == 'integrated'}
    for path in sorted((fdir / 'lane/runs').glob('*.json')):
        receipt = lane.read(path)
        if receipt.get('state') != 'running' or receipt.get('kind') == 'tooling-request' or receipt.get('branch') != branch:
            continue
        if not dead_local(receipt.get('owner')):
            continue
        integ = receipt.get('integrated') or {}
        landed = on_origin([root, receipt.get('tree'), receipt.get('root')], integ.get('commit'), branch) if integ.get('commit') else False
        if landed is None:
            report['skipped'].append(receipt['id'])
        elif landed:
            ci = ci_for(integ['commit'])
            integ.update(pushed=integ.get('pushed') or f.utc(), ci=ci if integ.get('ci') != 'green' else 'green')
            accepted_items = {r['item'] for r in receipt.get('accepted', [])}
            for item in receipt.get('items', []):
                item['state'] = 'done' if item['id'] in accepted_items else 'dropped'
            receipt.update(state='retired', retired_at=f.utc(), finished_owner=receipt.get('owner'), owner=None,
                           retirement_reason='reconciled: owner died; the commit is already on origin',
                           origin_outcome=dict(on_origin=True, ci=ci, commit=integ['commit']))
            receipt.pop('error', None)
            receipt['integrated'] = integ
            lane.save_run(fdir, receipt)
            f.log_event(fdir, 'lane-reconciled', lane=receipt['id'], commit=integ['commit'], ci=ci, outcome='on-origin')
            if receipt['id'] not in logged and ci == 'green':
                f.log_event(fdir, 'integrated', lane=receipt['id'], accepted=integ.get('accepted'),
                            commit=integ['commit'], reconciled=True)
            report['runs_retired'].append(receipt['id'])
        elif receipt.get('root') == str(root):
            report['resume'].append(receipt['id'])
        else:
            report['skipped'].append(receipt['id'])


def reconcile(root, fdir, branch='main', python=sys.executable, fetch=True):
    """Returns what was closed and which run (if any) the caller should resume."""
    root, fdir = Path(root).resolve(), Path(fdir).resolve()
    report = dict(runs_retired=[], tooling_retired=[], tooling_requeued=[], resume=[], skipped=[])
    if not (fdir / 'lane').is_dir():
        return report
    if fetch:
        try:
            f.git(root, 'fetch', '--quiet', 'origin', branch)
        except f.FlowError:
            report['skipped'].append('origin-unreachable')
            return report
    # The CI query is a network call; make it before taking the scheduling lock.
    verdicts = {}
    for path in (fdir / 'lane/runs').glob('*.json'):
        receipt = lane.read(path, details=False)
        commit = (receipt.get('integrated') or {}).get('commit')
        if (receipt.get('state') == 'running' and dead_local(receipt.get('owner')) and commit
                and on_origin([root, receipt.get('tree'), receipt.get('root')], commit, branch)):
            verdicts[commit] = origin_ci(root, commit, python)
    with state_lock(fdir):
        reconcile_requests(root, fdir, branch, report)
        reconcile_runs(root, fdir, branch, lambda commit: verdicts.get(commit, 'unavailable'), report)
    return report
