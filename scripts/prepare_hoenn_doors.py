"""Stage Littleroot door animations for FireRed's 1x2 door frame layout."""
import argparse
import hashlib
import json
from pathlib import Path
from PIL import Image


def encode_4bpp(image, banked=False):
    if image.mode != 'P' or image.width % 8 or image.height % 8:
        raise ValueError('Expected indexed sheet aligned to 8x8 tiles')
    output = bytearray()
    for ty in range(0, image.height, 8):
        for tx in range(0, image.width, 8):
            for y in range(8):
                for x in range(0, 8, 2):
                    lo, hi = image.getpixel((tx+x, ty+y)), image.getpixel((tx+x+1, ty+y))
                    if banked:
                        # Matches pokeemerald tools/gbagfx/gfx.c ConvertFromTiles:
                        # 8-bit indexed input to 4bpp keeps each index's low nibble.
                        lo, hi = lo & 15, hi & 15
                    if not 0 <= lo < 16 or not 0 <= hi < 16:
                        raise ValueError('Pixel outside 4bpp palette')
                    output.append(lo | hi << 4)
    return bytes(output)


def decode_4bpp(data, width, height):
    if width <= 0 or height <= 0 or width % 8 or height % 8 or len(data) != width*height//2:
        raise ValueError('Invalid 4bpp shape/length')
    image = Image.new('P', (width, height))
    cursor = 0
    for ty in range(0, height, 8):
        for tx in range(0, width, 8):
            for y in range(8):
                for x in range(0, 8, 2):
                    value = data[cursor]
                    cursor += 1
                    image.putpixel((tx+x, ty+y), value & 15)
                    image.putpixel((tx+x+1, ty+y), value >> 4)
    return image


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source', type=Path, required=True)
    p.add_argument('--out', type=Path, required=True)
    p.add_argument('--report', type=Path, required=True)
    a = p.parse_args()
    definitions = [('littleroot', 'Littleroot', 0x248, [10, 10, 6, 6, 6, 6, 6, 6]),
                   ('birchs_lab', 'BirchsLab', 0x249, [8]*8)]
    reports, outputs, declarations, rows = [], {}, [], []
    for name, symbol, metatile, palettes in definitions:
        path = a.source / 'graphics/door_anims' / (name + '.png')
        with Image.open(path) as image:
            if image.size != (16, 96):
                raise ValueError('Expected three 16x32 door frames')
            binary = encode_4bpp(image, banked=True)
            decoded = decode_4bpp(binary, 16, 96)
            if decoded.tobytes() != bytes(v & 15 for v in image.tobytes()):
                raise ValueError('Door image roundtrip differs')
        mapped = [v if v < 6 else v + 1 for v in palettes]
        if any(v >= 13 for v in mapped):
            raise ValueError('Unmapped reserved palette')
        outputs[name + '.4bpp'] = binary
        declarations.extend([
            f'static const u8 sHoennDoorTiles_{symbol}[] = INCBIN_U8("graphics/door_anims/hoenn/{name}.4bpp");',
            f'static const u8 sHoennDoorPalettes_{symbol}[] = {{{", ".join(map(str, mapped))}}};'])
        rows.append(f'    {{0x{metatile+128:03X}, DOOR_SOUND_NORMAL, DOOR_SIZE_1x2, sHoennDoorTiles_{symbol}, sHoennDoorPalettes_{symbol}}},')
        reports.append({'door': name, 'source_metatile': metatile, 'target_metatile': metatile+128,
                        'palettes': mapped, 'bytes': len(binary), 'frames': 3,
                        'frame_offsets': [0, 256, 512], 'frame_bytes': 256,
                        'pixel_roundtrip_mismatches': 0,
                        'index_conversion': 'low nibble, matching upstream gbagfx 8-bit indexed -> 4bpp',
                        'sha256': hashlib.sha256(binary).hexdigest()})
    a.out.mkdir(parents=True, exist_ok=False)
    for name, data in outputs.items():
        (a.out / name).write_bytes(data)
    # Snippet only: must be selected by tileset/region; IDs overlap existing FR doors.
    snippet = '// STAGING ONLY: select exclusively for the imported Hoenn tileset.\n'
    snippet += '\n'.join(declarations) + '\nstatic const struct DoorGraphics sHoennPilotDoors[] = {\n'
    snippet += '\n'.join(rows) + '\n    {}\n};\n'
    (a.out / 'hoenn_doors.inc').write_text(snippet)
    a.report.write_text(json.dumps({'status': 'DOOR_ASSETS_STAGED_NOT_INTEGRATED', 'doors': reports,
                                   'pending': ['tileset-scoped door table selection', 'closed-door redraw', 'warp scripts', 'runtime tests'],
                                   'limits': 'Pixel roundtrip and frame offsets only; no animation played in engine.'}, indent=2) + '\n')
    print('2 door assets staged, 6 frames, 3072 indexed pixels roundtripped')


if __name__ == '__main__':
    main()
