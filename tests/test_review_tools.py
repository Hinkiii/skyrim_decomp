"""review-check, review-group --select and detach-function. All inputs are synthetic."""
import contextlib
import io
from pathlib import Path
import shutil
import sys
import tempfile
import unittest
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'tools'))
import claims as github_claims  # noqa: E402
import flow  # noqa: E402
from test_flow import Repo, FUNCS, BUILD  # noqa: E402


class ReviewTools(unittest.TestCase):
    def setUp(self):
        for target, value in (('claims.sync_safe', None), ('claims.cached', (github_claims.ClaimIndex(), None))):
            p = patch(target, return_value=value)
            p.start()
            self.addCleanup(p.stop)
        self.tmp = Path(tempfile.mkdtemp()).resolve()
        self.repo = Repo(self.tmp)
        self.root, self.fdir = self.repo.root, self.repo.fdir
        self.addCleanup(shutil.rmtree, self.tmp)

    def group(self):
        name = flow.review_group(self.root, self.fdir, max_size=10)[0]
        return name, flow.load_group(self.fdir, name)

    def blocks(self, verdicts):
        return [(r, size, self.repo.digest(r), verdicts.get(r, 'approve'), 'none') for r, size, _ in FUNCS]

    def cli(self, *argv):
        out = io.StringIO()
        with contextlib.redirect_stdout(out), contextlib.redirect_stderr(out):
            code = flow.main(['--root', str(self.root), '--flow-dir', str(self.fdir), *argv])
        return code, out.getvalue()

    # review-check
    def test_check_ok_matches_read_review(self):
        _, g = self.group()
        path = self.repo.save_review(g['task_id'], self.blocks({}))
        code, out = self.cli('review-check', str(path))
        self.assertEqual((code, out.strip()), (0, 'OK'))
        self.assertEqual(flow.read_review(self.root, g, flow.inventory(self.root)['functions'])[0], 'ok')

    def test_check_reports_line_and_fix_for_each_problem(self):
        _, g = self.group()
        path = self.repo.save_review(g['task_id'], self.blocks({}))
        lines = path.read_text().splitlines()
        first = next(i for i, l in enumerate(lines) if l.startswith('verdict:'))
        lines[first] = 'verdict: Approve'
        later = next(i for i in range(first + 1, len(lines)) if lines[i].startswith('verdict:'))
        lines[later] = 'bogus_key: x'
        path.write_text('\n'.join(lines) + '\n')
        code, out = self.cli('review-check', str(path))
        self.assertEqual(code, 1)
        self.assertIn(f'line {first + 1}: verdict must be one of approve, reject, escalate', out)
        self.assertIn(f'line {later + 1}: unknown field', out)
        self.assertEqual(out.count('fix:'), 3)  # the dropped verdict is a third problem
        # The saved-review reader refuses the same file.
        status = flow.read_review(self.root, g, flow.inventory(self.root)['functions'])[0]
        self.assertTrue(status.startswith('malformed'))

    def test_check_flags_bad_heading_and_missing_block_and_group_fields(self):
        _, g = self.group()
        text = self.repo.save_review(g['task_id'], self.blocks({})).read_text()
        bad = self.tmp / 'bad.md'
        bad.write_text(text.replace(f'## {BUILD} 0x1010 8', f'## {BUILD} 0x1010 9'))
        code, out = self.cli('review-check', str(bad))
        self.assertEqual(code, 1)
        self.assertIn('heading "## synthetic-build 0x1010 9"', out)
        short = self.tmp / 'short.md'
        head, _, rest = text.partition(f'## {BUILD} 0x2000')
        short.write_text(head)
        code, out = self.cli('review-check', str(short))
        self.assertIn('no block for synthetic-build 0x2000', out)
        wrong = self.tmp / 'wrong.md'
        wrong.write_text(text.replace('fresh_context: yes', 'fresh_context: no'))
        code, out = self.cli('review-check', str(wrong))
        self.assertIn('line 4: fresh_context is not yes', out)

    def test_check_missing_file_and_title(self):
        code, out = self.cli('review-check', str(self.tmp / 'none.md'))
        self.assertEqual(code, 1)
        self.assertIn('does not exist', out)
        odd = self.tmp / 'odd.md'
        odd.write_text('hello\n')
        code, out = self.cli('review-check', str(odd))
        self.assertEqual(code, 1)
        self.assertIn('no "# Review <task-id>" title', out)

    def test_briefs_tell_the_reviewer_to_run_the_check(self):
        name, g = self.group()
        for brief in (g['brief'], flow.load_group(self.fdir, g['pair'])['brief']):
            self.assertIn(f'flow.py review-check .local/reviews/', (self.root / brief).read_text())
        self.assertIn('review-check', (self.root / f'.local/briefs/rerun-{name}.md').read_text())
        redo = flow.redo_review(self.root, self.fdir, name)
        self.assertIn(f'review-check .local/reviews/{redo["task_id"]}.md', (self.root / redo['brief']).read_text())

    # review-group --select
    def test_select_groups_exactly_the_named_functions(self):
        code, out = self.cli('review-group', '--select', '0x1010', f'{BUILD}-0x2000')
        self.assertEqual(code, 0, out)
        made = [g for g in flow.groups(self.fdir).values() if g['kind'] == 'review']
        self.assertEqual(sorted(sum((g['functions'] for g in made), [])), ['0x1010', '0x2000'])
        states = flow.function_states(self.root, self.fdir, online=False)
        self.assertEqual(states['0x1000']['state'], 'confirmed')
        self.assertFalse(states['0x1000'].get('group'))

    def test_select_refuses_unknown_or_ineligible_without_grouping(self):
        code, out = self.cli('review-group', '--select', '0x9999')
        self.assertEqual(code, 1)
        self.assertIn('names no function', out)
        self.cli('review-group', '--select', '0x1000')
        code, out = self.cli('review-group', '--select', '0x1000', '0x2000')
        self.assertEqual(code, 1)
        self.assertRegex(out, '0x1000 is .* in group')
        self.assertEqual([g['functions'] for g in flow.groups(self.fdir).values() if g['kind'] == 'review'],
                         [['0x1000']])

    # detach-function
    def malformed_group(self):
        name, g = self.group()
        path = self.repo.save_review(g['task_id'], self.blocks({}))
        path.write_text(path.read_text().replace('verdict: approve', 'verdict: Approve', 1))
        data = flow.inventory(self.root)['functions']
        self.assertTrue(flow.read_review(self.root, g, data)[0].startswith('malformed'))
        return name, g, path

    def test_detach_from_malformed_group_keeps_reviews_and_regroups(self):
        name, g, path = self.malformed_group()
        before = path.read_bytes()
        with self.assertRaisesRegex(flow.FlowError, 'detach-function'):
            flow.split_group(self.root, self.fdir, name, ['0x2000'])
        code, out = self.cli('detach-function', name, '--functions', '0x2000', '--reason', 'needs a fresh review')
        self.assertEqual(code, 0, out)
        self.assertEqual(path.read_bytes(), before)
        parent = flow.load_group(self.fdir, name)
        self.assertEqual(parent['functions'], ['0x1000', '0x1010'])
        self.assertNotIn('0x2000', parent['sampled'])
        self.assertNotIn('0x2000', parent['components'])
        self.assertEqual(parent['detached'][0]['function'], '0x2000')
        pair = flow.load_group(self.fdir, parent['pair'])
        self.assertNotIn('0x2000', pair['functions'])
        self.assertEqual(flow.events(self.fdir)[-1]['event'], 'detach-function')
        self.assertEqual(flow.function_states(self.root, self.fdir, online=False)['0x2000']['state'], 'confirmed')
        again = flow.review_group(self.root, self.fdir, select={'synthetic-build-0x2000'}, strict=True)
        self.assertEqual(flow.load_group(self.fdir, again[0])['functions'], ['0x2000'])

    def test_detach_everything_supersedes_the_group(self):
        name, _, path = self.malformed_group()
        flow.detach_functions(self.root, self.fdir, name, ['0x1000', '0x1010', '0x2000'], 'start over')
        self.assertTrue(flow.load_group(self.fdir, name)['superseded_by'].startswith('detach:'))
        self.assertTrue(path.exists())

    def test_detach_refuses_foreign_function_integrated_group_and_bad_reason(self):
        name, g, _ = self.malformed_group()
        with self.assertRaisesRegex(flow.FlowError, 'functions of the group'):
            flow.detach_functions(self.root, self.fdir, name, ['0x3000'], 'x')
        with self.assertRaisesRegex(flow.FlowError, 'short public prose'):
            flow.detach_functions(self.root, self.fdir, name, ['0x2000'], '  ')
        with self.assertRaisesRegex(flow.FlowError, 'primary review group'):
            flow.detach_functions(self.root, self.fdir, g['pair'], ['0x2000'], 'x')
        g['integrated'] = 'abc'
        flow.save_group(self.fdir, g)
        with self.assertRaisesRegex(flow.FlowError, 'integrated'):
            flow.detach_functions(self.root, self.fdir, name, ['0x2000'], 'x')


if __name__ == '__main__':
    unittest.main()
