"""Stage static General/Petalburg graphics for the two Hoenn pilot exteriors.

Only audited metatiles are populated. Output is NOT a complete/installable tileset.
"""
import argparse
import hashlib
import json
from pathlib import Path
import struct
from PIL import Image
from normalize_region_layers import tile_pixels
from region_tilesets import convert_entries, words


def palette(path):
    lines = path.read_text().splitlines()
    if lines[:3] != ['JASC-PAL', '0100', '16']:
        raise ValueError(f'Unsupported palette: {path}')
    colors = [tuple(map(int, line.split())) for line in lines[3:] if line.strip()]
    if len(colors) != 16 or any(len(c) != 3 or any(v < 0 or v > 255 for v in c) for c in colors):
        raise ValueError('Invalid palette colors')
    return colors


def rendered_entry(value, primary, secondary, split, palettes):
    index = value & 1023
    sheet = primary if index < split else secondary
    pixels = tile_pixels(sheet, index if index < split else index - split)
    colors = palettes[value >> 12]
    return tuple(None if pixels[(7-y if value & 0x800 else y)*8 + (7-x if value & 0x400 else x)] == 0
                 else colors[pixels[(7-y if value & 0x800 else y)*8 + (7-x if value & 0x400 else x)]]
                 for y in range(8) for x in range(8))


def pad_sheet(image, count):
    if image.mode != 'P' or image.width != 128 or image.height > count // 16 * 8:
        raise ValueError('Unexpected tilesheet dimensions/mode')
    if max(image.get_flattened_data()) > 15:
        raise ValueError('Source is not 4bpp indexed')
    out = Image.new('P', (128, count // 16 * 8), 0)
    out.putpalette(image.getpalette())
    out.paste(image, (0, 0))
    return out


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source', type=Path, required=True)
    p.add_argument('--audit', type=Path, required=True)
    p.add_argument('--normalized', type=Path, required=True)
    p.add_argument('--normalization-report', type=Path, required=True)
    p.add_argument('--out', type=Path, required=True)
    p.add_argument('--report', type=Path, required=True)
    a = p.parse_args()
    source = a.source / 'data/tilesets'
    normalized = a.normalized.read_bytes()
    evidence = json.loads(a.normalization_report.read_text())
    original = (source / 'secondary/petalburg/metatiles.bin').read_bytes()
    if hashlib.sha256(normalized).hexdigest() != evidence['result_sha256'] or hashlib.sha256(original).hexdigest() != evidence['source_sha256']:
        raise ValueError('Normalization evidence does not match these files')
    used = {'gTileset_General': set(), 'gTileset_Petalburg': set()}
    audit = json.loads(a.audit.read_text())
    if {m['map'] for m in audit} != {'LittlerootTown', 'Route101'}:
        raise ValueError('This converter is scoped to the two pilot exteriors')
    for m in audit:
        for s in m['tilesets']:
            used[s['symbol']].update(s['used_indices'])
    primary = Image.open(source / 'primary/general/tiles.png')
    secondary = Image.open(source / 'secondary/petalburg/tiles.png')
    dest_primary, dest_secondary = pad_sheet(primary, 640), pad_sheet(secondary, 384)
    tile_map = {i: i if i < 512 else i + 128 for i in range(512 + secondary.width*secondary.height//64)}
    palette_map = {i: i if i < 6 else i + 1 for i in range(12)}
    old_palettes = {i: palette(source / ('primary/general' if i < 6 else 'secondary/petalburg') / f'palettes/{i:02}.pal') for i in range(12)}
    new_palettes = {palette_map[i]: colors for i, colors in old_palettes.items()}
    new_palettes[6] = [(0, 0, 0)] * 16
    outputs, compared = {}, 0
    for symbol, raw, slots, folder in [
        ('gTileset_General', (source / 'primary/general/metatiles.bin').read_bytes(), 640, 'primary'),
        ('gTileset_Petalburg', normalized, len(normalized)//16, 'secondary')]:
        dest = bytearray(slots * 16)
        for index in sorted(used[symbol]):
            part = raw[index*16:index*16+16]
            if len(part) != 16:
                raise ValueError('Metatile outside source array')
            converted = convert_entries(part, tile_map, palette_map)
            for before, after in zip(words(part), words(converted)):
                if rendered_entry(before, primary, secondary, 512, old_palettes) != rendered_entry(after, dest_primary, dest_secondary, 640, new_palettes):
                    raise ValueError('Pixel mismatch after remapping')
                compared += 64
            dest[index*16:index*16+16] = converted
        outputs[folder] = bytes(dest)
    a.out.mkdir(parents=True, exist_ok=False)
    for folder, sheet in [('primary', dest_primary), ('secondary', dest_secondary)]:
        directory = a.out / folder
        (directory / 'palettes').mkdir(parents=True)
        sheet.save(directory / 'tiles.png')
        (directory / 'metatiles.bin').write_bytes(outputs[folder])
        for i in range(13):
            colors = new_palettes[i]
            (directory / 'palettes' / f'{i:02}.pal').write_text('JASC-PAL\n0100\n16\n' + '\n'.join(' '.join(map(str, c)) for c in colors) + '\n')
    report = {'status': 'STATIC_PILOT_GRAPHICS_ONLY_NOT_INSTALLABLE',
              'populated_metatiles': {k: len(v) for k, v in used.items()},
              'rgba_pixel_comparisons': compared, 'pixel_mismatches': 0,
              'tile_partition': '512 -> 640', 'palette_partition': '6 -> 7',
              'pending': ['attributes', 'animations', 'door graphics', 'unused metatiles', 'runtime validation'],
              'limits': 'Comparison against normalized static input, not original engine output. Zeroed unused entries are not suitable for other maps.'}
    a.report.write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps(report))


if __name__ == '__main__':
    main()
