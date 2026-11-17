import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools'))
import unit_selection as us


def git(root, *args):
    subprocess.run(['git', '-C', str(root), *args], check=True, capture_output=True)


class SelectionTest(unittest.TestCase):
    def setUp(self):
        self._tmp = tempfile.TemporaryDirectory()
        self.root = Path(self._tmp.name)
        self.addCleanup(self._tmp.cleanup)
        git(self.root, 'init', '-q')
        git(self.root, 'config', 'user.email', 't@example.com')
        git(self.root, 'config', 'user.name', 'T')
        self.write('reconstruction/a/A.cpp', '#include "A.h"\n')
        self.write('reconstruction/a/A.h', '#include "Common.h"\n')
        self.write('reconstruction/a/B.cpp', '#include "B.h"\n')
        self.write('reconstruction/a/B.h', '\n')
        self.write('reconstruction/a/Common.h', '\n')
        self.write('tools/x.py', 'x\n')
        git(self.root, 'add', '-A')
        git(self.root, 'commit', '-qm', 'base')
        self.units = {'reconstruction/a/A.cpp': set(), 'reconstruction/a/B.cpp': set()}

    def write(self, name, text):
        path = self.root / name
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(text)

    def select(self):
        return us.select(self.root, 'HEAD', self.units)

    def test_nothing_changed_selects_nothing(self):
        self.assertEqual(self.select()[0], set())

    def test_header_reached_transitively_selects_only_its_includers(self):
        self.write('reconstruction/a/Common.h', 'changed\n')
        self.assertEqual(self.select()[0], {'reconstruction/a/A.cpp'})

    def test_unrelated_header_selects_only_its_unit(self):
        self.write('reconstruction/a/B.h', 'changed\n')
        self.assertEqual(self.select()[0], {'reconstruction/a/B.cpp'})

    def test_new_untracked_include_target_selects_includer(self):
        self.write('reconstruction/a/B.cpp', '#include "B.h"\n#include "New.h"\n')
        self.write('reconstruction/a/New.h', '\n')
        self.assertEqual(self.select()[0], {'reconstruction/a/B.cpp'})

    def test_recorded_dependency_that_was_removed_still_selects(self):
        self.units['reconstruction/a/B.cpp'] = {'reconstruction/a/Gone.h'}
        git(self.root, 'rm', '-q', '--cached', 'reconstruction/a/B.h')
        self.write('reconstruction/a/Gone.h', 'x\n')
        self.assertEqual(self.select()[0], {'reconstruction/a/B.cpp'})

    def test_record_or_recipe_change_selects_its_unit(self):
        self.units['reconstruction/a/A.cpp'] = {'reports/x/0x1.json'}
        self.write('reports/x/0x1.json', '{}\n')
        self.assertEqual(self.select()[0], {'reconstruction/a/A.cpp'})

    def test_tools_and_pins_force_full_rebuild(self):
        for name in ('tools/x.py', 'tools/verifier/pinned.sha256', 'builds/target.json',
                     'reconstruction/recipes/r.json', '.gitattributes'):
            with self.subTest(name):
                self.write(name, 'changed\n')
                chosen, why = self.select()
                self.assertEqual(chosen, set(self.units))
                self.assertIn('full rebuild', why)

    def test_unreadable_closure_fails_closed(self):
        self.write('reconstruction/a/A.cpp', '#include "Missing.h"\n')
        self.assertIn('reconstruction/a/A.cpp', self.select()[0])

    def test_no_base_or_unknown_base_rebuilds_everything(self):
        self.assertEqual(us.select(self.root, None, self.units)[0], set(self.units))
        self.assertEqual(us.select(self.root, 'nosuchrev', self.units)[0], set(self.units))


if __name__ == '__main__':
    unittest.main()
