import subprocess
import sys
from pathlib import Path
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools'))
import flow_preflight


def git(path, *args):
    return subprocess.run(['git', '-C', str(path), *args], check=True, capture_output=True, text=True).stdout.strip()


def setup_repos(tmp_path):
    remote = tmp_path / 'remote.git'
    subprocess.run(['git', 'init', '--bare', '-q', str(remote)], check=True)
    main = tmp_path / 'main'
    subprocess.run(['git', 'clone', '-q', str(remote), str(main)], check=True, capture_output=True)
    git(main, 'config', 'user.email', 'test@example.com')
    git(main, 'config', 'user.name', 'Test')
    git(main, 'checkout', '-b', 'main')
    (main / 'file').write_text('base\n')
    git(main, 'add', 'file')
    git(main, 'commit', '-qm', 'base')
    git(main, 'push', '-qu', 'origin', 'main')
    root = tmp_path / 'input'
    subprocess.run(['git', 'clone', '-q', '-b', 'main', str(remote), str(root)], check=True)
    git(root, 'config', 'user.email', 'test@example.com')
    git(root, 'config', 'user.name', 'Test')
    return main, root


def commit(path, name, text):
    (path / name).write_text(text)
    git(path, 'add', name)
    git(path, 'commit', '-qm', 'change')


def check(main, root, rebase=False, code=0):
    real = subprocess.run
    def run(args, **kwargs):
        if str(args[0]) == sys.executable:
            return subprocess.CompletedProcess(args, code, 'red reason\n' if code else 'green\n', '')
        return real(args, **kwargs)
    with patch.object(flow_preflight.subprocess, 'run', side_effect=run):
        return flow_preflight.preflight(root, main, rebase)


def advance(main):
    commit(main, 'file', 'upstream\n')
    git(main, 'push', '-q', 'origin', 'main')


def test_green(tmp_path):
    main, root = setup_repos(tmp_path)
    assert check(main, root) == dict(ok=True, problem=None, rebased=False)


def test_red(tmp_path):
    main, root = setup_repos(tmp_path)
    assert check(main, root, code=1)['problem'] == 'main is not green: red reason'


def test_ahead(tmp_path):
    main, root = setup_repos(tmp_path)
    commit(main, 'other', 'local')
    assert check(main, root)['problem'] == 'main checkout has unpublished changes'


def test_stale(tmp_path):
    main, root = setup_repos(tmp_path)
    advance(main)
    assert check(main, root)['problem'] == 'input is based on an old main; rerun with --rebase'


def test_clean_rebase(tmp_path):
    main, root = setup_repos(tmp_path)
    commit(root, 'other', 'local')
    advance(main)
    assert check(main, root, True) == dict(ok=True, problem=None, rebased=True)
    git(root, 'merge-base', '--is-ancestor', 'origin/main', 'HEAD')


def test_conflict_aborts(tmp_path):
    main, root = setup_repos(tmp_path)
    commit(root, 'file', 'local\n')
    original = git(root, 'rev-parse', 'HEAD')
    advance(main)
    assert check(main, root, True)['problem'] == 'rebase conflict: file'
    assert git(root, 'rev-parse', 'HEAD') == original
    assert not (root / '.git/rebase-merge').exists()


def tooling_change(root):
    (root / 'tools').mkdir(exist_ok=True)
    commit(root, 'tools/x.py', 'x\n')
    return git(root, 'rev-parse', 'HEAD')


def linux(main, root, runs=None, error=None):
    def gh(*args):
        if error:
            raise flow_preflight.ci_status.GhError(error)
        return runs
    with patch.object(flow_preflight.ci_status, 'gh', side_effect=gh) as mocked:
        out = check(main, root)
    return out, mocked


def test_linux_green_run_for_exact_sha(tmp_path):
    main, root = setup_repos(tmp_path)
    sha = tooling_change(root)
    out, mocked = linux(main, root, [dict(headSha=sha, status='completed', conclusion='success')])
    assert out == dict(ok=True, problem=None, rebased=False)
    assert '--commit' in mocked.call_args.args and sha in mocked.call_args.args


def test_linux_run_for_other_sha_does_not_count(tmp_path):
    main, root = setup_repos(tmp_path)
    sha = tooling_change(root)
    out, _ = linux(main, root, [dict(headSha='0' * 40, status='completed', conclusion='success')])
    assert not out['ok']
    assert f'git push origin {sha}:refs/heads/' in out['problem'] and 'has not run' in out['problem']
    assert 'ci_status.py' in out['problem'] and '--wait' in out['problem']


def test_linux_red_and_running_runs_block(tmp_path):
    main, root = setup_repos(tmp_path)
    sha = tooling_change(root)
    red, _ = linux(main, root, [dict(headSha=sha, status='completed', conclusion='failure')])
    assert 'is red' in red['problem']
    running, _ = linux(main, root, [dict(headSha=sha, status='in_progress', conclusion=None)])
    assert 'still running' in running['problem']


def test_linux_gh_unavailable_warns_without_blocking(tmp_path):
    main, root = setup_repos(tmp_path)
    tooling_change(root)
    out, _ = linux(main, root, error='gh: not found')
    assert out['ok'] and 'gh unavailable' in out['warnings'][0]


def test_linux_skipped_for_non_tooling_change(tmp_path):
    main, root = setup_repos(tmp_path)
    commit(root, 'reconstruction.cpp', 'x\n')
    out, mocked = linux(main, root, error='must not be asked')
    assert out['ok'] and not mocked.called
