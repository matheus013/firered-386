"""Verify opening resource indices against the private USA SoulSilver archive."""
import argparse
import hashlib
import json
import re
from pathlib import Path

from prepare_hgss_message_headers import read_banks

ARCHIVE_SHA256 = '1b62632b6b569616e6374ac81a2e69d4bd51e8c381904051ad51550779c865d5'


def decompress_lz10(data):
    if len(data) < 4 or data[0] != 0x10:
        raise ValueError('Expected LZ10 resource')
    size = int.from_bytes(data[1:4], 'little')
    if not 0 < size <= 16 * 1024 * 1024:
        raise ValueError('Invalid decompressed size')
    output = bytearray()
    cursor = 4
    while len(output) < size:
        if cursor >= len(data):
            raise ValueError('Truncated LZ flags')
        flags = data[cursor]
        cursor += 1
        for bit in range(7, -1, -1):
            if len(output) == size:
                break
            if flags & (1 << bit):
                if cursor + 2 > len(data):
                    raise ValueError('Truncated LZ reference')
                first, second = data[cursor:cursor + 2]
                cursor += 2
                count = (first >> 4) + 3
                distance = ((first & 15) << 8 | second) + 1
                if distance > len(output) or count > size - len(output):
                    raise ValueError('Invalid LZ reference')
                for _ in range(count):
                    output.append(output[-distance])
            else:
                if cursor >= len(data):
                    raise ValueError('Truncated LZ literal')
                output.append(data[cursor])
                cursor += 1
    return bytes(output)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--hgss', type=Path, required=True)
    parser.add_argument('--archive', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    raw = args.archive.read_bytes()
    if hashlib.sha256(raw).hexdigest() != ARCHIVE_SHA256:
        raise ValueError('Unsupported opening archive')
    banks = read_banks(raw)
    source = '\n'.join((args.hgss / 'src' / f'intro_movie_scene_{i}.c').read_text()
                       for i in range(1, 6))
    pattern = r'NARC_gs_opening_gs_opening_(\d{8})_([A-Z0-9]+)(_lz)?\b'
    tokens = sorted(set(match.group() for match in re.finditer(pattern, source)))
    definitions, evidence = [], []
    for token in tokens:
        match = re.fullmatch(pattern, token)
        number, extension, compressed = match.groups()
        index = int(number)
        if index >= len(banks):
            raise ValueError('Resource index exceeds archive')
        data = banks[index]
        if bool(compressed) != (data[0] == 0x10):
            raise ValueError('Compression differs: ' + token)
        decoded = decompress_lz10(data) if compressed else data
        local = args.hgss / 'files/demo/opening/gs_opening' / f'gs_opening_{number}.{extension}'
        if decoded != local.read_bytes():
            raise ValueError('Extracted resource differs at index: ' + str(index))
        definitions.append(f'#define {token} {index}\n')
        evidence.append({'index': index, 'compressed': bool(compressed),
                         'decoded_sha256': hashlib.sha256(decoded).hexdigest()})
    if not definitions:
        raise ValueError('No opening resources referenced')
    args.output.mkdir(parents=True, exist_ok=False)
    target = args.output / 'demo/opening'
    target.mkdir(parents=True)
    (target / 'gs_opening.naix').write_text('#pragma once\n' + ''.join(definitions))
    report = {'archive_sha256': ARCHIVE_SHA256, 'member_count': len(banks),
              'verified_references': len(evidence), 'resources': evidence,
              'gameplay_tested': False}
    (args.output / 'opening-indices.json').write_text(json.dumps(report, indent=2) + '\n')
    print(f'Verified {len(evidence)} opening references against {len(banks)} archive members')


if __name__ == '__main__':
    main()
