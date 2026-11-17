import unittest
from pathlib import Path

WORKFLOWS = Path(__file__).resolve().parents[1] / '.github/workflows'


class CheckWorkflowTriggers(unittest.TestCase):
    def test_candidate_branches_do_not_run_the_full_suite(self):
        text = (WORKFLOWS / 'check.yml').read_text()
        head = text.split('permissions:')[0]
        self.assertIn("branches-ignore: ['ci-confirm/**']", head)
        self.assertNotIn('\n    branches:', head)

    def test_main_pushes_still_trigger_the_checks_that_gates_read(self):
        head = (WORKFLOWS / 'check.yml').read_text().split('permissions:')[0]
        self.assertIn('\n  push:', head)
        self.assertIn('\n  pull_request:', head)

    def test_no_tool_waits_for_checks_on_a_confirm_branch(self):
        tools = Path(__file__).resolve().parents[1] / 'tools'
        pusher = (tools / 'verifier/ci_confirm.py').read_text()
        self.assertIn("refs/heads/ci-confirm/", pusher)
        self.assertNotIn('Infrastructure', pusher)
        self.assertNotIn('ci_status', pusher)


if __name__ == '__main__':
    unittest.main()
