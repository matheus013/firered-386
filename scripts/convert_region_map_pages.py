"""Convert the Hoenn (pokeemerald) and Johto (HNS_modern) region maps into FireRed region map pages.

The sources draw the map on an 8bpp affine background: a 64x64 one-byte tilemap over an 8bpp tile
sheet, with grid cell (c, r) on tile (c + 1, r + 2). FireRed's region map is a 4bpp text background
whose grid cell (c, r) sits on screen tile (c + 4, r + 4), 21 map columns wide plus the button column.

Each page becomes:
  graphics/region_map/<region>_page_<n>.png  indexed 4bpp sheets, one per 15-colour palette
                                             (the tiles of sheet 1 follow those of sheet 0)
  graphics/region_map/<region>_page.bin      tilemap, PAGE_ROWS rows of (width + 9) columns
  src/data/region_map/region_map_layout_<region>.h  section grid [MAP_HEIGHT][width]
Tile numbers in the tilemap start at TILE_BASE (BG0 tiles 576+, after the open animation's tiles in
char block 1) and palette numbers at the page's first palette slot.

Tiles are deduplicated (including horizontal/vertical flips). Every source pixel keeps its exact
colour: a tile is placed in the first palette whose colour set it fits.
"""
import argparse
import json
from pathlib import Path
import re
import struct

from PIL import Image

TILE_BASE = 576
PAGE_ROWS = 20
LEFT_MARGIN = 4     # FireRed screen tile of grid column 0
RIGHT_EXTRA = 5     # button column and frame after the last grid column
GRID_ROWS = 15
OCEAN = (0, 21)    # a plain sea tile in both sources (bottom-left corner)
# region: (repo, tileset png, tilemap bin, layout header, layout array, grid width, first palette slot,
#          palette slots available). BG palettes 5-11 are free on FireRed's region map.
PAGES = {
    'hoenn': ('pokeemerald', 'graphics/pokenav/region_map/map.png', 'graphics/pokenav/region_map/map.bin',
              'src/data/region_map/region_map_layout.h', 'sRegionMap_MapSectionLayout', 28, 5, 4),
    'johto': ('HNS_modern', 'graphics/pokenav/region_map/johtomap.png', 'graphics/pokenav/region_map/johtomap.bin',
              'src/data/region_map/region_map_layout_johto.h', 'sRegionMap_MapSectionLayout_Johto', 25, 9, 3),
}


def source_tiles(png, tilemap):
    sheet = Image.open(png)
    assert sheet.mode == 'P', png
    rgb = sheet.getpalette()
    width = sheet.size[0] // 8
    pixels = sheet.load()
    raw = Path(tilemap).read_bytes()
    assert len(raw) == 64 * 64, (tilemap, len(raw))

    def tile_at(col, row):
        if col < 0 or row < 0:
            col, row = OCEAN  # outside the source art: open sea
        t = raw[row * 64 + col]
        tx, ty = (t % width) * 8, (t // width) * 8
        return tuple(tuple(rgb[pixels[tx + x, ty + y] * 3:pixels[tx + x, ty + y] * 3 + 3]) for y in range(8) for x in range(8))
    return tile_at


def gba_colour(c):
    return tuple(v >> 3 for v in c)


def convert(references, game, region, report):
    repo, png, tilemap, layout_h, array, width, first_pal, max_pals = PAGES[region]
    src = references / repo
    tile_at = source_tiles(src / png, src / tilemap)
    cols = width + LEFT_MARGIN + RIGHT_EXTRA
    # Page tile (tc, tr) shows source tile (tc - LEFT_MARGIN + 1, tr - LEFT_MARGIN + 2).
    cells = [[tile_at(tc - LEFT_MARGIN + 1, tr - LEFT_MARGIN + 2) for tc in range(cols)] for tr in range(PAGE_ROWS)]
    # Colours are compared at GBA precision (5 bits per channel).
    cells = [[tuple(gba_colour(p) for p in t) for t in row] for row in cells]
    # Index 0 is transparent on a GBA background, so it is never used: every colour is opaque.
    unused = (31, 0, 31)
    palettes = []   # colour lists; entry 0 is the unused transparent slot
    order = sorted({t for row in cells for t in row}, key=lambda t: -len(set(t)))
    placement = {}
    for t in order:
        need = set(t)
        for i, pal in enumerate(palettes):
            if len(set(pal[1:]) | need) <= 15:
                pal.extend(sorted(need - set(pal)))
                placement[t] = i
                break
        else:
            palettes.append([unused] + sorted(need))
            placement[t] = len(palettes) - 1
            assert len(palettes[-1]) <= 16, (region, 'a tile needs more than 15 colours')
    assert len(palettes) <= max_pals, (region, 'needs', len(palettes), 'palettes')
    sheets = [[] for _ in palettes]
    index = {}
    entries = []

    def indexed(t, pal):
        return tuple(pal.index(p, 1) for p in t)

    def flips(px):
        rows = [px[i * 8:i * 8 + 8] for i in range(8)]
        h = tuple(v for r in rows for v in reversed(r))
        v = tuple(v for r in reversed(rows) for v in r)
        hv = tuple(v for r in reversed(rows) for v in reversed(r))
        return {px: 0, h: 1, v: 2, hv: 3}

    for row in cells:
        for t in row:
            p = placement[t]
            px = indexed(t, palettes[p])
            for form, flip in flips(px).items():
                if (p, form) in index:
                    number = index[(p, form)]
                    break
            else:
                sheets[p].append(px)
                number = None
                flip = 0
            if number is None:
                local = len(sheets[p]) - 1
                index[(p, px)] = (p, local)
                number = (p, local)
            entries.append((number, flip, p))
    counts = [len(s) for s in sheets]
    starts = [TILE_BASE + sum(counts[:i]) for i in range(len(counts))]
    assert starts[-1] + counts[-1] <= 1024, (region, 'tiles do not fit in BG0', starts, counts)
    out = game / 'graphics/region_map'
    for i, (pal, sheet) in enumerate(zip(palettes, sheets)):
        tiles_per_row = 1  # one tile per row: no padding tiles, so tile numbers stay contiguous
        img = Image.new('P', (tiles_per_row * 8, ((len(sheet) + tiles_per_row - 1) // tiles_per_row) * 8))
        img.putpalette([v << 3 for c in pal + [(0, 0, 0)] * (16 - len(pal)) for v in c])
        px = img.load()
        for n, t in enumerate(sheet):
            ox, oy = (n % tiles_per_row) * 8, (n // tiles_per_row) * 8
            for k, v in enumerate(t):
                px[ox + k % 8, oy + k // 8] = v
        img.save(out / f'{region}_page_{i}.png')
        # gbagfx reads JASC palettes with LF line endings only.
        (out / f'{region}_page_{i}.pal').write_bytes(
            ('JASC-PAL\n0100\n16\n' + ''.join(f'{r << 3} {g << 3} {b << 3}\n' for r, g, b in pal + [(0, 0, 0)] * (16 - len(pal)))).encode('ascii'))
    words = []
    for (p, local), flip, pal_index in entries:
        words.append((starts[p] + local) | (flip << 10) | ((first_pal + pal_index) << 12))
    (out / f'{region}_page.bin').write_bytes(struct.pack(f'<{len(words)}H', *words))
    # Section grid: the source layout's first GRID_ROWS rows and `width` columns.
    text = (src / layout_h).read_text(encoding='utf-8')
    body = text.split(array)[1]
    rows = [[c.strip() for c in r.split(',') if c.strip()] for r in re.findall(r'\{(MAPSEC_[^{}]*)\}', body)][:GRID_ROWS]
    # Only the region's own IDs: Hoenn's come before MAPSEC_PALLET_TOWN (KANTO_MAPSEC_START), Johto's after
    # MAPSEC_SPECIAL_AREA. HNS reuses some Hoenn names (MAPSEC_SAFARI_ZONE) for Johto places.
    ids = [e['id'] for e in json.loads((game / 'src/data/region_map/region_map_sections.json').read_text(encoding='utf-8'))['map_sections']]
    known = set(ids[:ids.index('MAPSEC_PALLET_TOWN')] if region == 'hoenn' else ids[ids.index('MAPSEC_SPECIAL_AREA') + 1:])
    grid = [[c if c in known else 'MAPSEC_NONE' for c in r[:width]] for r in rows]
    dropped = sorted({c for r in rows for c in r[:width]} - known - {'MAPSEC_NONE'})
    name = f'sRegionMapSections_{region.title()}'
    lines = [f'// Generated by scripts/convert_region_map_pages.py from {repo} {layout_h}.',
             f'// Sections missing from FireRed are MAPSEC_NONE: {", ".join(dropped) or "none"}.',
             f'#define REGIONMAP_{region.upper()}_WIDTH {width}',
             f'static const u8 {name}[MAP_HEIGHT][REGIONMAP_{region.upper()}_WIDTH] = {{']
    lines += ['    {' + ', '.join(r) + '},' for r in grid]
    lines.append('};')
    (game / f'src/data/region_map/region_map_layout_{region}.h').write_text('\n'.join(lines) + '\n', encoding='utf-8', newline='\n')
    report[region] = {'source': f'{repo} {png}, {tilemap}, {layout_h}', 'grid_width': width, 'page_columns': cols,
                      'tiles': counts, 'first_tile': TILE_BASE, 'palettes': len(palettes), 'first_palette': first_pal,
                      'colours': [len(p) - 1 for p in palettes], 'sections_dropped': dropped}
    return grid


def section_boxes(grid):
    """Bounding box (x, y, width, height) of every section on a page grid."""
    boxes = {}
    for y, row in enumerate(grid):
        for x, mapsec in enumerate(row):
            if mapsec == 'MAPSEC_NONE':
                continue
            x0, y0, x1, y1 = boxes.get(mapsec, (x, y, x, y))
            boxes[mapsec] = (min(x0, x), min(y0, y), max(x1, x), max(y1, y))
    return {m: (x0, y0, x1 - x0 + 1, y1 - y0 + 1) for m, (x0, y0, x1, y1) in boxes.items()}


def update_section_positions(game, grids):
    """Write each page section's position into region_map_sections.json, where the player icon reads it.
    Hoenn IDs come before KANTO_MAPSEC_START and use their own table (hoenn_x/...); Johto ones use x/y."""
    path = game / 'src/data/region_map/region_map_sections.json'
    data = json.loads(path.read_text(encoding='utf-8'))
    for region, grid in grids.items():
        prefix = 'hoenn_' if region == 'hoenn' else ''
        for entry in data['map_sections']:
            box = section_boxes(grid).get(entry['id'])
            if box:
                entry.update({prefix + k: v for k, v in zip(('x', 'y', 'width', 'height'), box)})
    path.write_text(json.dumps(data, indent=2, ensure_ascii=False) + '\n', encoding='utf-8', newline='\n')


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--references', type=Path, required=True)
    p.add_argument('--game', type=Path, required=True)
    p.add_argument('--report', type=Path, required=True)
    a = p.parse_args()
    report = {}
    grids = {region: convert(a.references, a.game, region, report) for region in PAGES}
    update_section_positions(a.game, grids)
    a.report.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(json.dumps(report))


if __name__ == '__main__':
    main()
