"""Dead publishers' receipts are closed from origin or resumed, never left `running`."""
import json
from pathlib import Path
import socket
import sys
import unittest
from unittest.mock import patch

sys.path[:0] = [str(Path(__file__).resolve().parents[1] / 'tools')]
import flow
import flow_lane as lane
import flow_reconcile
from test_flow import git
from test_flow_lane import PushLane as _PushLane

lane_setup, enqueue, run_lane = _PushLane.setUp, _PushLane.enqueue, _PushLane.lane
del _PushLane

DEAD = dict(pid=999999, host=socket.gethostname(), birth='never started')


class Reconcile(unittest.TestCase):
    setUp, enqueue, lane = lane_setup, enqueue, run_lane

    def events(self, name):
        return [e for e in flow.events(self.fdir) if e['event'] == name]

    def orphan(self, *, publish):
        """A built, checked lane run whose owner died; optionally after its push reached origin."""
        self.enqueue()
        receipt = self.lane(push=False)
        tree = Path(receipt['tree'])
        if publish:
            git(tree, 'push', '-q', 'origin', receipt['integrated']['commit'] + ':refs/heads/main')
            receipt['integrated']['pushed'] = '2000-01-01T00:00:00Z'
        receipt.update(state='running', owner=DEAD)
        lane.save_run(self.fdir, receipt)
        return receipt

    def test_dead_owner_whose_commit_is_on_origin_is_retired_with_the_origin_outcome(self):
        receipt = self.orphan(publish=True)
        with patch.object(flow_reconcile, 'origin_ci', return_value='green'):
            report = flow_reconcile.reconcile(self.root, self.fdir)
        self.assertEqual(report['runs_retired'], [receipt['id']])
        stored = lane.read(self.fdir / 'lane/runs' / (receipt['id'] + '.json'))
        self.assertEqual(stored['state'], 'retired')
        self.assertEqual(stored['origin_outcome'], dict(on_origin=True, ci='green', commit=receipt['integrated']['commit']))
        self.assertEqual(stored['integrated']['ci'], 'green')
        self.assertIsNone(stored['owner'])
        self.assertTrue(all(i['state'] == 'done' for i in stored['items']))
        self.assertEqual(len(self.events('lane-reconciled')), 1)
        self.assertEqual(len(self.events('integrated')), 1)
        # Idempotent: the retired receipt is not looked at again.
        flow_reconcile.reconcile(self.root, self.fdir)
        self.assertEqual(len(self.events('lane-reconciled')), 1)

    def test_live_owner_is_never_touched(self):
        receipt = self.orphan(publish=True)
        stored = lane.read(self.fdir / 'lane/runs' / (receipt['id'] + '.json'))
        stored['owner'] = lane.process_owner()
        lane.save_run(self.fdir, stored)
        report = flow_reconcile.reconcile(self.root, self.fdir)
        self.assertEqual(report['runs_retired'], [])
        self.assertEqual(lane.read(self.fdir / 'lane/runs' / (receipt['id'] + '.json'))['state'], 'running')

    def test_dead_owner_whose_commit_is_not_on_origin_resumes_in_the_next_lane_run(self):
        receipt = self.orphan(publish=False)
        before = git(self.root, 'rev-parse', 'origin/main')
        report = flow_reconcile.reconcile(self.root, self.fdir)
        self.assertEqual(report['resume'], [receipt['id']])
        result = self.lane()
        self.assertEqual(result['id'], receipt['id'])
        self.assertEqual(result['state'], 'done')
        self.assertNotEqual(git(self.root, 'rev-parse', 'origin/main'), before)

    def request(self, revision, name):
        item = dict(id=name, root=str(self.root), revision=revision, base=revision, branch='main',
                    paths=['tools/x.py'], state='running', run='unknown-run', queued_at='2000-01-01T00:00:00Z')
        lane.write(self.fdir / 'lane/tooling-queue' / (name + '.json'), item)
        lane.write(self.fdir / 'lane/tooling' / (name + '.json'), dict(item, kind='tooling-request', owner=DEAD))
        return item

    def test_tooling_request_without_outcome_is_closed_or_requeued_from_origin(self):
        landed = git(self.root, 'rev-parse', 'origin/main')
        self.request(landed, 'landedrequest')
        (self.root / 'unpushed.txt').write_text('x\n')
        git(self.root, 'add', 'unpushed.txt')
        git(self.root, 'commit', '-q', '-m', 'local only')
        self.request(git(self.root, 'rev-parse', 'HEAD'), 'localrequest')
        report = flow_reconcile.reconcile(self.root, self.fdir)
        self.assertEqual((report['tooling_retired'], report['tooling_requeued']), (['landedrequest'], ['localrequest']))
        done = lane.read(self.fdir / 'lane/tooling' / 'landedrequest.json')
        self.assertEqual((done['state'], done['integrated']['commit']), ('done', landed))
        outcome = self.events('tooling-outcome')
        self.assertEqual([(e['request'], e['result']) for e in outcome], [('landedrequest', 'done')])
        queued = lane.read(self.fdir / 'lane/tooling-queue' / 'localrequest.json')
        self.assertEqual(queued['state'], 'queued')
        self.assertFalse((self.fdir / 'lane/tooling/localrequest.json').exists())

    def test_next_actions_reconciles_first(self):
        receipt = self.orphan(publish=True)
        with patch.object(flow_reconcile, 'origin_ci', return_value='green'):
            flow.next_actions(self.root, self.fdir, online=True)
        self.assertEqual(lane.read(self.fdir / 'lane/runs' / (receipt['id'] + '.json'))['state'], 'retired')


if __name__ == '__main__':
    unittest.main()
