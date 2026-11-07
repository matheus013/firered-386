import copy
import unittest
import json
import tempfile
from pathlib import Path
from plan_region_import import allocate, plan


class AllocationTests(unittest.TestCase):
    def test_excluded_custom_hub_remains_a_boundary(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            folder = root / 'data/maps'
            folder.mkdir(parents=True)
            (folder / 'map_groups.json').write_text(json.dumps({'group_order': []}))
            for name, kind, destination in [('Town', 'MAP_TYPE_TOWN', 'House'),
                                             ('House', 'MAP_TYPE_INDOOR', 'Hub'),
                                             ('Hub', 'MAP_TYPE_INDOOR', 'Town')]:
                (folder / name).mkdir()
                (folder / name / 'map.json').write_text(json.dumps({
                    'id': 'MAP_' + name, 'map_type': kind,
                    'warp_events': [{'dest_map': 'MAP_' + destination, 'dest_warp_id': 0}]}))
            result = plan(root, root, ['Town'], 'Johto', ['Hub'])
            self.assertEqual({x['source_name'] for x in result['allocation']}, {'Town', 'House'})
            self.assertEqual(result['boundary_links_requiring_resolution'],
                             [{'map': 'House', 'kind': 'warp', 'destination': 'MAP_Hub'}])
            with self.assertRaises(ValueError):
                plan(root, root, ['Town'], 'Johto', ['Town'])

    def test_append_only_deterministic(self):
        groups = {'group_order': ['old'], 'old': ['PalletTown']}
        original = copy.deepcopy(groups)
        result = allocate(groups, ['Route29', 'NewBarkTown'], 'Johto')
        self.assertEqual(groups, original)
        self.assertEqual(result[0], {'source_name': 'NewBarkTown', 'target_name': 'Johto_NewBarkTown',
                                    'group': 1, 'number': 0})
        self.assertEqual(result, allocate(groups, ['NewBarkTown', 'Route29'], 'Johto'))

    def test_storage_limits_and_collisions(self):
        for groups, names in [({'group_order': list(range(256))}, []),
                              ({'group_order': []}, list(map(str, range(257)))),
                              ({'group_order': ['old'], 'old': ['Johto_Test']}, ['Test'])]:
            with self.assertRaises(ValueError):
                allocate(groups, names, 'Johto')


if __name__ == '__main__':
    unittest.main()
