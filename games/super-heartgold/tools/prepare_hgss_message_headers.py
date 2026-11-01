"""Generate private compile-probe indices from pinned GMM sources and a local NARC.

No message text or game assets are copied into the repository. Output is for
the English HGSS module probe, not a replacement for the game's asset build.
"""
import argparse
import hashlib
import json
import re
import struct
import urllib.request
import xml.etree.ElementTree as ET
from pathlib import Path

PRET_COMMIT = 'a6ebf56ab7a454d91cc4715c73b62e93c305dc29'
SS_USA_ARCHIVE_SHA256 = '570433fadb7f2959c99e73ee446ad7bc770702dd71b472acf51c0e358a51136c'


def read_banks(data):
    if data[:4] != b'NARC' or len(data) < 16:
        raise ValueError('Expected NARC')
    if struct.unpack_from('<I', data, 8)[0] != len(data):
        raise ValueError('NARC size differs')
    offset = struct.unpack_from('<H', data, 12)[0]
    chunks = {}
    for _ in range(struct.unpack_from('<H', data, 14)[0]):
        tag, size = struct.unpack_from('<4sI', data, offset)
        if size < 8 or offset + size > len(data) or tag in chunks:
            raise ValueError('Invalid NARC section')
        chunks[tag] = data[offset + 8:offset + size]
        offset += size
    fat, image = chunks[b'BTAF'], chunks[b'GMIF']
    count = struct.unpack_from('<H', fat)[0]
    banks = []
    for index in range(count):
        start, end = struct.unpack_from('<II', fat, 4 + index * 8)
        if not 0 <= start < end <= len(image):
            raise ValueError('Invalid member bounds')
        banks.append(image[start:end])
    return banks


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--hgss', type=Path, required=True)
    parser.add_argument('--archive', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--modules', nargs='+', default=['message_format','msgdata'],
                        help='Original source modules whose message indices are required')
    args = parser.parse_args()
    raw = args.archive.read_bytes()
    if hashlib.sha256(raw).hexdigest() != SS_USA_ARCHIVE_SHA256:
        raise ValueError('Unsupported message archive: expected verified SoulSilver USA data')
    banks = read_banks(raw)
    source = '\n'.join((args.hgss / 'src' / (name+'.c')).read_text()
                       for name in args.modules)
    requested = sorted(set(re.findall(r'#include "msgdata/msg/(msg_\d{4})\.h"', source)))
    bank_ids = sorted(set(map(int, re.findall(r'NARC_msg_msg_(\d{4})_bin', source))) |
                      {int(name[4:]) for name in requested})
    if any(bank >= len(banks) for bank in bank_ids):
        raise ValueError('Referenced bank outside archive')
    args.output.mkdir(parents=True, exist_ok=False)
    target = args.output / 'msgdata'
    (target / 'msg').mkdir(parents=True)
    (target / 'msg.naix').write_text('#pragma once\n' + ''.join(
        f'#define NARC_msg_msg_{bank:04d}_bin {bank}\n' for bank in bank_ids))
    evidence = []
    for name in requested:
        url = f'https://raw.githubusercontent.com/pret/pokeheartgold/{PRET_COMMIT}/files/msgdata/msg/{name}.gmm'
        with urllib.request.urlopen(url, timeout=30) as response:
            document = response.read()
        rows = ET.fromstring(document).findall('row')
        count = struct.unpack_from('<H', banks[int(name[4:])])[0]
        indices = [int(row.attrib['index']) for row in rows]
        if sorted(indices) != list(range(count)):
            raise ValueError(f'{name}: source indices differ from archive count {count}')
        identifiers = [row.attrib['id'] for row in rows]
        if len(set(identifiers)) != count or any(
                not re.fullmatch(r'[A-Za-z_][A-Za-z_0-9]*', ident) for ident in identifiers):
            raise ValueError(f'{name}: invalid or duplicate identifier')
        (target / 'msg' / f'{name}.h').write_text('#pragma once\n' + ''.join(
            f'#define {ident} {index}\n' for ident, index in zip(identifiers, indices)))
        evidence.append({'bank': name, 'count': count,
                         'gmm_sha256': hashlib.sha256(document).hexdigest()})
    report = {'pret_commit': PRET_COMMIT, 'archive_sha256': hashlib.sha256(raw).hexdigest(),
              'source_modules': args.modules,
              'archive_members': len(banks), 'referenced_banks': len(bank_ids),
              'headers': evidence, 'runtime_tested': False}
    (args.output / 'provenance.json').write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps(report, indent=2))


if __name__ == '__main__':
    main()
