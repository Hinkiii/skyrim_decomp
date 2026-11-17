"""Fast checks before registering a tooling publication."""
from pathlib import Path
import subprocess
import sys

import ci_status

# Paths whose change can break the Linux CI run (shell flags, /proc, path handling) while the
# macOS and Cygwin machines that develop them stay green.
TOOLING_PREFIXES = ('tools/', 'tests/', '.github/')
LINUX_WORKFLOW = ci_status.CHECKS


def linux_check(root, base, git):
    """(problem, warning) for a tooling push: a green Linux run must exist for the exact HEAD."""
    changed = git(root, 'diff', '--name-only', f'{base}...HEAD')
    if changed.returncode:
        return 'cannot list changed paths: ' + changed.stderr.strip(), None
    if not any(p.startswith(TOOLING_PREFIXES) for p in changed.stdout.splitlines()):
        return None, None
    head = git(root, 'rev-parse', 'HEAD')
    if head.returncode:
        return 'cannot resolve HEAD: ' + head.stderr.strip(), None
    sha = head.stdout.strip()
    try:
        runs = ci_status.gh('run', 'list', '--workflow', 'check.yml', '--commit', sha, '--limit', '20',
                            '--json', 'databaseId,headSha,status,conclusion')
    except ci_status.GhError as error:
        # Offline or unauthenticated: the lane's exact-SHA CI still decides after the push.
        return None, f'linux check skipped, gh unavailable: {error}'
    mine = [r for r in runs if r.get('headSha') == sha]
    if any(r.get('status') == 'completed' and r.get('conclusion') == 'success' for r in mine):
        return None, None
    branch = git(root, 'branch', '--show-current').stdout.strip() or 'preflight/' + sha[:12]
    state = 'is red' if any(r.get('status') == 'completed' for r in mine) else (
        'is still running' if mine else 'has not run')
    return (f'no green Linux check.yml run for {sha[:12]} (it {state}); run: git push origin '
            f'{sha}:refs/heads/{branch} && {sys.executable} tools/ci_status.py --require-workflow '
            f"'{LINUX_WORKFLOW}' --wait 900 --commit {sha}"), None


def preflight(root, main_checkout, rebase=False):
    root, main_checkout = Path(root).resolve(), Path(main_checkout).resolve()
    rebased = False
    warnings = []

    def result(problem=None):
        out = dict(ok=problem is None, problem=problem, rebased=rebased)
        if warnings:
            out['warnings'] = warnings
        return out

    def git(where, *args):
        return subprocess.run(['git', '-C', str(where), *args], capture_output=True, text=True)

    fetched = git(main_checkout, 'fetch', 'origin', 'main')
    if fetched.returncode:
        return result('fetch failed: ' + (fetched.stderr.strip() or fetched.stdout.strip()))
    sha = git(main_checkout, 'rev-parse', 'origin/main')
    if sha.returncode:
        return result('cannot resolve origin/main: ' + sha.stderr.strip())
    ci = subprocess.run([sys.executable, 'tools/ci_status.py', '--without-advisory',
                         '--commit', sha.stdout.strip()], cwd=main_checkout, capture_output=True, text=True)
    if ci.returncode:
        lines = (ci.stdout or ci.stderr).splitlines()
        return result('main is not green: ' + (lines[0] if lines else 'no output'))
    ahead = git(main_checkout, 'rev-list', '--count', 'origin/main..HEAD')
    changes = git(main_checkout, 'status', '--porcelain', '--', 'tools', 'tests')
    if ahead.returncode or changes.returncode or ahead.stdout.strip() != '0' or changes.stdout.strip():
        return result('main checkout has unpublished changes')
    fetched_input = git(root, 'fetch', 'origin', 'main')
    if fetched_input.returncode:
        return result('input fetch failed: ' + fetched_input.stderr.strip())
    ancestor = git(root, 'merge-base', '--is-ancestor', sha.stdout.strip(), 'HEAD')
    if ancestor.returncode:
        if not rebase:
            return result('input is based on an old main; rerun with --rebase')
        if root == main_checkout:
            return result('cannot rebase the main checkout')
        attempt = git(root, 'rebase', sha.stdout.strip())
        if attempt.returncode:
            files = git(root, 'diff', '--name-only', '--diff-filter=U').stdout.strip().splitlines()
            git(root, 'rebase', '--abort')
            return result('rebase conflict: ' + (', '.join(files) or attempt.stderr.strip()))
        rebased = True
    problem, warning = linux_check(root, sha.stdout.strip(), git)
    if warning:
        warnings.append(warning)
        print('preflight: ' + warning, file=sys.stderr)
    return result(problem)
