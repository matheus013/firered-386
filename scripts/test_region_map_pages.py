"""The converted Hoenn/Johto region map pages redraw the source art exactly (at GBA colour precision)."""
import json
from pathlib import Path
import struct
import unittest

from PIL import Image

import convert_region_map_pages as conv

ROOT = Path(__file__).resolve().parents[1]
GAME = ROOT / 'games/super-fire-red'
REFS = ROOT.parent / 'map-references'


def page_image(region, first_pal):
    tiles = []
    i = 0
    while (GAME / f'graphics/region_map/{region}_page_{i}.png').exists():
        sheet = Image.open(GAME / f'graphics/region_map/{region}_page_{i}.png')
        assert sheet.size[0] == 8, 'sheets are one tile wide so tile numbers stay contiguous'
        pal = sheet.getpalette()[:48]
        for k in range(sheet.size[1] // 8):
            tiles.append((i, [tuple(v >> 3 for v in pal[c * 3:c * 3 + 3]) for c in sheet.crop((0, k * 8, 8, k * 8 + 8)).getdata()]))
        i += 1
    data = (GAME / f'graphics/region_map/{region}_page.bin').read_bytes()
    words = struct.unpack(f'<{len(data) // 2}H', data)
    width = len(words) // conv.PAGE_ROWS
    out = {}
    for n, v in enumerate(words):
        tile, flip, pal = (v & 0x3FF) - conv.TILE_BASE, (v >> 10) & 3, (v >> 12) - first_pal
        sheet, px = tiles[tile]
        assert sheet == pal, (region, n, 'tile drawn with another palette')
        for k, colour in enumerate(px):
            x, y = k % 8, k // 8
            if flip & 1:
                x = 7 - x
            if flip & 2:
                y = 7 - y
            out[((n % width) * 8 + x, (n // width) * 8 + y)] = colour
    return width, out


@unittest.skipUnless(REFS.exists(), 'map-references checkout not available')
class RegionMapPages(unittest.TestCase):
    def test_pages_match_source_art(self):
        for region, (repo, png, tilemap, *_rest) in conv.PAGES.items():
            first_pal = conv.PAGES[region][6]
            width, drawn = page_image(region, first_pal)
            tile_at = conv.source_tiles(REFS / repo / png, REFS / repo / tilemap)
            for tr in range(conv.PAGE_ROWS):
                for tc in range(width):
                    want = [conv.gba_colour(p) for p in tile_at(tc - conv.LEFT_MARGIN + 1, tr - conv.LEFT_MARGIN + 2)]
                    got = [drawn[(tc * 8 + k % 8, tr * 8 + k // 8)] for k in range(64)]
                    self.assertEqual(got, want, (region, tc, tr))

    def test_tiles_fit_bg0_and_free_palettes(self):
        report = json.loads((ROOT / 'docs/map-audits/region-map-pages.json').read_text(encoding='utf-8'))
        thumbs = report.pop('switch_map_thumbnails')
        slots = {thumbs['palette_slot']}
        self.assertIn(thumbs['palette_slot'], range(5, 12), 'switch menu thumbnails use a free BG palette')
        for region, info in report.items():
            self.assertLessEqual(info['first_tile'] + sum(info['tiles']), 1024, region)
            used = set(range(info['first_palette'], info['first_palette'] + info['palettes']))
            self.assertTrue(used <= set(range(5, 12)), (region, 'BG palettes 5-11 are the free ones'))
            self.assertFalse(used & slots, (region, 'palette slots overlap'))
            slots |= used

    def test_section_grids(self):
        for region, info in conv.PAGES.items():
            text = (GAME / f'src/data/region_map/region_map_layout_{region}.h').read_text(encoding='utf-8')
            rows = [r for r in text.splitlines() if r.startswith('    {')]
            self.assertEqual(len(rows), conv.GRID_ROWS)
            self.assertTrue(all(r.count('MAPSEC_') == info[5] for r in rows), region)
        hoenn = (GAME / 'src/data/region_map/region_map_layout_hoenn.h').read_text(encoding='utf-8')
        johto = (GAME / 'src/data/region_map/region_map_layout_johto.h').read_text(encoding='utf-8')
        self.assertIn('MAPSEC_LITTLEROOT_TOWN', hoenn)
        self.assertIn('MAPSEC_NEW_BARK_TOWN', johto)
        self.assertIn('MAPSEC_ROUTE_29', johto)


if __name__ == '__main__':
    unittest.main()
