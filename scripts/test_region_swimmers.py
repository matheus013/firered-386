"""Prevent aquatic NPCs from becoming unreachable on a different elevation."""
import json
from pathlib import Path
import re
import struct
import unittest


class SwimmerElevationTests(unittest.TestCase):
    def test_aquatic_objects_match_their_water_tile(self):
        game = Path(__file__).resolve().parents[1] / 'games/super-fire-red'
        layouts = {l['id']: l for l in json.loads((game / 'data/layouts/layouts.json').read_text())['layouts'] if 'id' in l}
        count = 0
        for folder in sorted((game / 'data/maps').iterdir()):
            if not folder.name.startswith(('Hoenn_', 'Johto_')):
                continue
            header = json.loads((folder / 'map.json').read_text())
            layout = layouts[header['layout']]
            data = (game / layout['blockdata_filepath']).read_bytes()
            for obj in header.get('object_events') or []:
                if not re.search('SWIMMER|_WATER', obj['graphics_id']):
                    continue
                count += 1
                with self.subTest(map=folder.name, script=obj['script']):
                    offset = 2 * (obj['y'] * layout['width'] + obj['x'])
                    elevation = struct.unpack_from('<H', data, offset)[0] >> 12
                    self.assertIn(int(obj['elevation']), (0, elevation))
        self.assertGreater(count, 0, 'No aquatic NPCs examined')


if __name__ == '__main__':
    unittest.main()
