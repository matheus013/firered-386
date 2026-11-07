"""Remove invalid static lower quadrants only when an opaque upper tile covers them.

Diagnostic staging only: animations and runtime writes must be reviewed separately.
"""
import argparse
import hashlib
import json
from pathlib import Path
import struct
from PIL import Image
from region_tilesets import words


def tile_pixels(image, index):
    if image.mode != 'P' or image.width % 8 or image.height % 8:
        raise ValueError('Expected indexed tilesheet with 8-pixel alignment')
    if not 0 <= index < image.width * image.height // 64:
        raise ValueError('Tile outside static sheet')
    x = index % (image.width // 8) * 8
    y = index // (image.width // 8) * 8
    return tuple(image.crop((x, y, x + 8, y + 8)).get_flattened_data())


def normalize(data, attributes, selected, primary, secondary, split):
    entries, attrs = words(data), words(attributes)
    if len(entries) != len(attrs) * 8:
        raise ValueError('Metatile/attribute lengths differ')
    output, proof = entries.copy(), []
    for index in sorted(set(selected)):
        if not 0 <= index < len(attrs):
            raise ValueError('Invalid metatile index')
        for quadrant in range(8):
            offset = index * 8 + quadrant
            value = entries[offset]
            tile, palette = value & 1023, value >> 12
            image = primary if tile < split else secondary
            local = tile if tile < split else tile - split
            invalid_tile = local >= image.width * image.height // 64
            if palette < 13 and not invalid_tile:
                continue
            if quadrant >= 4 or attrs[index] >> 12 != 0:
                raise ValueError('Invalid visible tile/palette; cannot normalize')
            upper = entries[offset + 4]
            upper_id = upper & 1023
            upper_image = primary if upper_id < split else secondary
            pixels = tile_pixels(upper_image, upper_id if upper_id < split else upper_id - split)
            if upper >> 12 >= 13 or 0 in pixels:
                raise ValueError('Invalid lower tile is not covered by valid opaque upper tile')
            output[offset] = 0
            proof.append({'metatile': index, 'quadrant': quadrant, 'before': value,
                          'after': 0, 'upper_entry': upper, 'opaque_pixels': len(pixels)})
    return struct.pack('<' + 'H' * len(output), *output), proof


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source', type=Path, required=True)
    p.add_argument('--audit', type=Path, required=True)
    p.add_argument('--out', type=Path, required=True)
    p.add_argument('--report', type=Path, required=True)
    a = p.parse_args()
    # Explicit first target: Emerald General + Petalburg, not an inferred global conversion.
    base = a.source / 'data/tilesets'
    metatiles = base / 'secondary/petalburg/metatiles.bin'
    raw = metatiles.read_bytes()
    selected = set()
    for m in json.loads(a.audit.read_text()):
        for s in m['tilesets']:
            if s['symbol'] == 'gTileset_Petalburg':
                selected.update(s['used_indices'])
    if not selected:
        raise ValueError('Audit contains no Petalburg metatiles')
    with Image.open(base / 'primary/general/tiles.png') as primary, Image.open(base / 'secondary/petalburg/tiles.png') as secondary:
        result, proof = normalize(raw, (base / 'secondary/petalburg/metatile_attributes.bin').read_bytes(), selected, primary, secondary, 512)
    a.out.parent.mkdir(parents=True, exist_ok=True)
    with a.out.open('xb') as f:
        f.write(result)
    report = {'status': 'STATIC_STAGING_ONLY_RUNTIME_UNVALIDATED',
              'source_sha256': hashlib.sha256(raw).hexdigest(),
              'result_sha256': hashlib.sha256(result).hexdigest(), 'changes': proof,
              'pending': ['animation review', 'tileset conversion', 'runtime rendering test']}
    a.report.write_text(json.dumps(report, indent=2) + '\n')
    print(f'{len(proof)} hidden quadrants normalized; no build or engine installation')


if __name__ == '__main__':
    main()
