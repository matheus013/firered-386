"""Exercise the read-only native inspector using independently generated saves.

Usage: python test_save_inspector.py /absolute/path/to/hg_save_inspect[.exe]
Fixtures contain authorial bytes only. They are not playable game saves.
"""

import binascii
import json
from pathlib import Path
import struct
import subprocess
import sys
import tempfile


IMAGE_SIZE = 0x80000
BANK_SIZE = 0x40000
CHUNKS = ((0, 0xF628), (0xF700, 0x12310))


def require(condition, message):
    if not condition:
        raise AssertionError(message)


def build_image(counters=((5, 5), (6, 6))):
    image = bytearray([0xFF]) * IMAGE_SIZE
    crcs = []
    for bank in range(2):
        bank_crcs = []
        for chunk, (offset, size) in enumerate(CHUNKS):
            start = bank * BANK_SIZE + offset
            payload = bytes([17 + bank * 3 + chunk]) * (size - 16)
            crc = binascii.crc_hqx(payload, 0xFFFF)
            image[start:start + size - 16] = payload
            struct.pack_into('<IIIHH', image, start + size - 16,
                             counters[bank][chunk], size, 0x20060623, chunk, crc)
            bank_crcs.append(crc)
        crcs.append(bank_crcs)
    return image, crcs


def mutate_footer(image, bank, chunk, field_offset, value, width):
    offset, size = CHUNKS[chunk]
    struct.pack_into('<' + width, image, bank * BANK_SIZE + offset + size - 16 + field_offset, value)


def invoke(executable, *arguments):
    return subprocess.run([str(executable), *map(str, arguments)], capture_output=True,
                          text=True, timeout=30, check=False)


def inspect(executable, path, image, code, status, reference_bank, readable_bank):
    path.write_bytes(image)
    before = path.read_bytes(), path.stat().st_mtime_ns
    process = invoke(executable, path)
    require(process.returncode == code, f'Inspector exited {process.returncode}: {process.stderr}')
    require(not process.stderr, 'Inspection unexpectedly produced stderr')
    result = json.loads(process.stdout)
    require(set(result) == {'scope', 'reference_status', 'reference_bank', 'readable_primary_bank', 'chunks'},
            'JSON schema contains missing or unexpected top-level fields')
    require(result['scope'] == 'primary_chunks_only', 'Scope overstates inspected data')
    require(result['reference_status'] == status and result['reference_bank'] == reference_bank,
            'Reference policy status or bank differs')
    require(result['readable_primary_bank'] == readable_bank, 'Trusted primary bank differs')
    require(str(path) not in process.stdout and path.name not in process.stdout, 'JSON exposed input path')
    require(len(result['chunks']) == 2 and all(len(bank) == 2 for bank in result['chunks']),
            'Inspector did not report four primary chunks')
    for bank in result['chunks']:
        for chunk in bank:
            require(set(chunk) == {'error', 'footer', 'calculated_crc'} and set(chunk['footer']) == {'count'},
                    'Chunk JSON includes unexpected fields')
    require((path.read_bytes(), path.stat().st_mtime_ns) == before, 'Inspector changed bytes or modification time')
    repeated = invoke(executable, path)
    require(repeated.stdout == process.stdout and repeated.returncode == code, 'JSON is not deterministic')
    require((path.read_bytes(), path.stat().st_mtime_ns) == before, 'Repeated inspection modified input')
    return result


def run(executable):
    for arguments in ((), ('one', 'two')):
        process = invoke(executable, *arguments)
        require(process.returncode == 1 and not process.stdout and 'Usage:' in process.stderr,
                'Invalid argument count did not produce usage/error')
    with tempfile.TemporaryDirectory(prefix='hg-save-inspector-test-') as directory:
        root = Path(directory)
        missing = root / 'private-missing-save.sav'
        process = invoke(executable, missing)
        require(process.returncode == 1 and not process.stdout and not missing.exists(),
                'Missing input was created or accepted')
        require(str(missing) not in process.stderr, 'Error exposed a private input path')
        path = root / 'private-synthetic-save.sav'
        for size in (IMAGE_SIZE - 1, IMAGE_SIZE + 1):
            path.write_bytes(bytes(size))
            before = path.read_bytes(), path.stat().st_mtime_ns
            process = invoke(executable, path)
            require(process.returncode == 1 and not process.stdout, 'Truncated/padded image was accepted')
            require((path.read_bytes(), path.stat().st_mtime_ns) == before, 'Bad-size input was changed')

        blank = inspect(executable, path, bytes([0xFF]) * IMAGE_SIZE, 2, 'no_save', None, None)
        require(all(chunk['error'] == 'size' for bank in blank['chunks'] for chunk in bank),
                'Erased flash did not fail size validation')
        image, crcs = build_image()
        valid = inspect(executable, path, image, 0, 'good', 1, 1)
        unicode_directory = root / 'save-ação-ポケモン'
        unicode_directory.mkdir()
        inspect(executable, unicode_directory / 'pokémon-記録.sav', image, 0, 'good', 1, 1)
        for bank in range(2):
            for chunk in range(2):
                entry = valid['chunks'][bank][chunk]
                require(entry['error'] == 'none' and entry['calculated_crc'] == crcs[bank][chunk],
                        'Native CRC differs from independent Python crc_hqx')
                require(entry['footer']['count'] == bank + 5, 'Footer count was not decoded little endian')

        for name, offset, value, width in (
            ('size', 4, 123, 'I'), ('magic', 8, 0, 'I'), ('slot', 12, 7, 'H'), ('crc', 14, 0, 'H')
        ):
            damaged = bytearray(image)
            mutate_footer(damaged, 1, 0, offset, value, width)
            result = inspect(executable, path, damaged, 0, 'recovered', 0, 0)
            require(result['chunks'][1][0]['error'] == name, f'{name} corruption not identified')

        mismatched, _ = build_image(((4, 5), (6, 7)))
        inspect(executable, path, mismatched, 2, 'corrupt', None, None)
        payload_damage = bytearray(image)
        payload_damage[BANK_SIZE + CHUNKS[0][0]] ^= 1
        result = inspect(executable, path, payload_damage, 0, 'recovered', 0, 0)
        require(result['chunks'][1][0]['error'] == 'crc', 'Payload corruption was not detected')

        wrapped, _ = build_image(((0, 0), (0xFFFFFFFF, 0xFFFFFFFF)))
        inspect(executable, path, wrapped, 0, 'good', 0, 0)
        mutate_footer(wrapped, 0, 1, 8, 0, 'I')
        result = inspect(executable, path, wrapped, 2, 'good', 0, None)
        require(result['chunks'][0][1]['error'] == 'magic', 'Invalid wrap-selected PC was not exposed')
    print('Read-only native save inspector integration passed')


if __name__ == '__main__':
    if len(sys.argv) != 2:
        raise SystemExit('Usage: test_save_inspector.py <inspector-executable>')
    run(Path(sys.argv[1]).resolve())
