"""Every metatile a Hoenn/Johto layout uses must have graphics in its tileset.

Guards against the batch-1 regression where the Hoenn General tileset only carried the metatiles of the
first pilot maps, so later maps drew empty (grey/black) squares that the ROM walk tests cannot see.
A metatile whose eight entries are all zero is empty unless the source tileset has it empty too, which
only happens for metatiles no map should use.
"""
import json
from pathlib import Path
import re
import struct
import unittest

GAME = Path(__file__).resolve().parents[1] / 'games/super-fire-red'


def tileset_files():
    headers = (GAME / 'src/data/tilesets/headers.h').read_text(encoding='utf-8')
    metatiles = (GAME / 'src/data/tilesets/metatiles.h').read_text(encoding='utf-8')
    files = {}
    for name, body in re.findall(r'const struct Tileset (\w+) =\s*\{(.*?)\};', headers, re.S):
        symbol = re.search(r'\.metatiles = (\w+)', body)[1]
        path = re.search(symbol + r'\[\] = INCBIN_U16\("([^"]+)"\)', metatiles)
        if path:
            files[name] = GAME / path[1]
    return files


class RegionMetatiles(unittest.TestCase):
    def test_used_metatiles_have_graphics(self):
        files = tileset_files()
        layouts = json.loads((GAME / 'data/layouts/layouts.json').read_text(encoding='utf-8'))['layouts']
        empty = []
        for layout in layouts:
            if not str(layout.get('id', '')).startswith(('LAYOUT_HOENN_', 'LAYOUT_JOHTO_')):
                continue
            tables = {}
            for key, offset in (('primary_tileset', 0), ('secondary_tileset', 640)):
                raw = files[layout[key]].read_bytes()
                tables[offset] = [raw[i:i + 16] for i in range(0, len(raw), 16)]
            used = {v & 0x3FF for (v,) in struct.iter_unpack('<H', (GAME / layout['blockdata_filepath']).read_bytes())}
            for tile in sorted(used):
                offset = 0 if tile < 640 else 640
                table = tables[offset]
                index = tile - offset
                if index >= len(table) or not any(table[index]):
                    empty.append((layout['id'], hex(tile)))
        self.assertEqual(empty, [], 'layouts use metatiles with no graphics')


if __name__ == '__main__':
    unittest.main()
