import copy
import unittest
from plan_region_import import allocate


class AllocationTests(unittest.TestCase):
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
