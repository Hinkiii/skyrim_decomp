"""Which reconstruction units need recompiling after a change.

A unit is stale when a file of its include closure changed (the .cpp plus every header it reaches,
taken from both the current tree and the closure the record was accepted with, so a removed
include still selects it) or when its own record, recipe or symbol map changed. Anything that can
alter every compile (tools, the pinned toolchain, the target build, recipes) selects all units,
because a narrower claim would let a changed compiler or comparator ride on old evidence.
Selection only saves work; it never decides a match.
"""
import subprocess

import source_digest

# Compiler, comparator, pins, target identity and recipes are shared by every unit.
GLOBAL_PREFIXES = ('tools/', 'builds/', 'reconstruction/recipes/')
GLOBAL_FILES = ('.gitattributes',)


def git(root, *args):
    return subprocess.check_output(['git', *args], cwd=root, stderr=subprocess.PIPE)


def changed_paths(root, base):
    """Tracked differences of the working tree from `base`, plus untracked files (no rename inference)."""
    tracked = git(root, 'diff', '--no-renames', '--name-only', '-z', base).split(b'\0')
    fresh = git(root, 'ls-files', '--others', '--exclude-standard', '-z').split(b'\0')
    return sorted({p.decode() for p in tracked + fresh if p})


def full_rebuild_reason(paths):
    for path in paths:
        if path.startswith(GLOBAL_PREFIXES) or path in GLOBAL_FILES:
            return f'full rebuild: {path} can change every unit'
    return None


def select(root, base, units):
    """(selected compiled files, reason). `units` maps compiled file -> extra dependency paths
    (the record's translation unit, recipe and symbol map)."""
    if not base:
        return set(units), 'full rebuild: no comparison base'
    try:
        paths = changed_paths(root, base)
    except (subprocess.CalledProcessError, OSError, UnicodeError):
        return set(units), 'full rebuild: comparison unavailable'
    reason = full_rebuild_reason(paths)
    if reason:
        return set(units), reason
    changed = set(paths)
    chosen = set()
    for compiled, extra in units.items():
        deps = set(extra) | {compiled}
        try:
            deps.update(source_digest.include_closure(root, compiled))
        except (ValueError, OSError):
            chosen.add(compiled)  # an unreadable closure cannot be shown unchanged
            continue
        if deps & changed:
            chosen.add(compiled)
    return chosen, f'{len(chosen)} of {len(units)} units have a changed include closure, record or recipe'
