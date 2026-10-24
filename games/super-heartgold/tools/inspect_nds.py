"""Inspect a locally supplied NDS dump; never copy its payload into the project."""
import argparse
import hashlib
import json
import struct
from pathlib import Path


def crc16(data):
    value = 0xFFFF
    for byte in data:
        value ^= byte
        for _ in range(8):
            value = (value >> 1) ^ (0xA001 if value & 1 else 0)
    return value


def inspect(path):
    size = path.stat().st_size
    with path.open('rb') as stream:
        header = stream.read(512)
        if len(header) != 512:
            raise ValueError('NDS header is truncated')
        digest = hashlib.sha1(header)
        for block in iter(lambda: stream.read(1024 * 1024), b''):
            digest.update(block)
    sections = {}
    for name, offset, length in [('arm9', 0x20, 0x2C), ('arm7', 0x30, 0x3C),
                                 ('fnt', 0x40, 0x44), ('fat', 0x48, 0x4C),
                                 ('arm9_overlays', 0x50, 0x54), ('arm7_overlays', 0x58, 0x5C)]:
        start = struct.unpack_from('<I', header, offset)[0]
        count = struct.unpack_from('<I', header, length)[0]
        if start > size or count > size - start:
            raise ValueError(f'{name} range exceeds file size')
        sections[name] = {'offset': start, 'bytes': count}
    return {'title': header[:12].rstrip(b'\0').decode('ascii', errors='replace'),
            'game_code': header[12:16].decode('ascii', errors='replace'),
            'revision': header[0x1E], 'bytes': size, 'sha1': digest.hexdigest(),
            'header_crc_ok': crc16(header[:0x15E]) == struct.unpack_from('<H', header, 0x15E)[0],
            'sections': sections,
            'scope': 'Header/range inspection only. Does not establish rights, clean-dump identity or AOT compatibility.'}


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('rom', type=Path)
    parser.add_argument('--output', type=Path)
    args = parser.parse_args()
    try:
        result = json.dumps(inspect(args.rom), indent=2) + '\n'
    except (OSError, ValueError) as error:
        parser.exit(1, f'{error}\n')
    if args.output:
        args.output.write_text(result, encoding='utf-8')
    else:
        print(result, end='')
