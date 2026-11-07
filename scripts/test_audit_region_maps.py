import json
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch
from audit_region_maps import audit


class InventoryTests(unittest.TestCase):
    def test_detects_missing_assets_and_dangling_destinations(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            (root / 'data/layouts').mkdir(parents=True)
            (root / 'data/maps/Test').mkdir(parents=True)
            layout = {'id': 'LAYOUT_TEST', 'width': 2, 'height': 2,
                      'border_filepath': 'border.bin', 'blockdata_filepath': 'map.bin'}
            (root / 'data/layouts/layouts.json').write_text(json.dumps({'layouts': [layout]}))
            (root / 'map.bin').write_bytes(b'\0\0')
            data = {'id': 'MAP_TEST', 'layout': 'LAYOUT_TEST', 'connections': None,
                    'warp_events': [{'dest_map': 'MAP_DYNAMIC'}, {'dest_map': 'MAP_MISSING'}]}
            (root / 'data/maps/Test/map.json').write_text(json.dumps(data))
            with patch('audit_region_maps.subprocess.check_output', return_value='testpin\n'):
                report = audit(root, root)
            self.assertEqual(report['conflicting_ids'], 1)
            self.assertEqual(report['maps_with_issues'], 1)
            self.assertEqual(report['problems'][0]['issues'], [
                'missing border.bin', 'block size 2 != 8', 'unresolved warp: MAP_MISSING'])
            self.assertEqual(len(report['maps'][0]['links']), 2)


if __name__ == '__main__':
    unittest.main()
