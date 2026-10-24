"""Import selected NARC archives from a user-supplied NDS into a PRIVATE native data folder.

No code, BIOS or executable is extracted. The output is not part of the public
product. Existing files are never overwritten. Requires Python 3.10+.
"""
import argparse
import hashlib
import json
from pathlib import Path, PurePosixPath
import struct

from inspect_nds import inspect


def index_files(fnt, fat, rom_size):
    def u16(data, offset):
        if offset < 0 or offset + 2 > len(data):
            raise ValueError('Truncated filesystem table')
        return struct.unpack_from('<H', data, offset)[0]

    def u32(data, offset):
        if offset < 0 or offset + 4 > len(data):
            raise ValueError('Truncated filesystem table')
        return struct.unpack_from('<I', data, offset)[0]

    count = u16(fnt, 6)
    if not 1 <= count <= 4096 or count * 8 > len(fnt) or len(fat) % 8:
        raise ValueError('Invalid filesystem table size')
    ranges = []
    for offset in range(0, len(fat), 8):
        start, end = u32(fat, offset), u32(fat, offset+4)
        if not 0 <= start <= end <= rom_size:
            raise ValueError('File allocation exceeds ROM')
        ranges.append((start, end))
    files, visited = {}, set()

    def walk(directory, prefix, parent=None):
        number = directory - 0xF000
        if not 0 <= number < count or number in visited:
            raise ValueError('Invalid or cyclic directory')
        visited.add(number)
        cursor, file_id = u32(fnt, number*8), u16(fnt, number*8+4)
        if parent is not None and u16(fnt, number*8+6) != parent:
            raise ValueError('Directory parent differs')
        if not count*8 <= cursor < len(fnt):
            raise ValueError('Invalid directory entry offset')
        while True:
            if cursor >= len(fnt):
                raise ValueError('Unterminated directory')
            tag = fnt[cursor]
            cursor += 1
            if tag == 0:
                return
            length = tag & 127
            if length == 0 or cursor+length > len(fnt):
                raise ValueError('Invalid filename length')
            name = fnt[cursor:cursor+length].decode('ascii')
            cursor += length
            if name in ('.', '..') or any(c in name for c in '/\\:\0'):
                raise ValueError('Invalid filename component')
            path = prefix + name
            if tag & 128:
                child = u16(fnt, cursor)
                cursor += 2
                walk(child, path+'/', directory)
            else:
                if file_id >= len(ranges) or path in files:
                    raise ValueError('Invalid file ID or duplicate path')
                files[path] = ranges[file_id]
                file_id += 1

    walk(0xF000, '')
    if len(visited) != count:
        raise ValueError('Unreachable directory')
    return files


def import_resources(rom, output, selected):
    rom, output = rom.resolve(strict=True), output.resolve()
    project = Path(__file__).resolve().parents[1]
    if output.is_relative_to(project) or any((p / '.git').exists() for p in (output, *output.parents)):
        raise ValueError('Resource output must be outside all Git repositories and the project')
    if not selected or len(selected) != len(set(selected)):
        raise ValueError('Select one or more unique file paths')
    report = inspect(rom)
    with rom.open('rb') as source:
        tables = []
        for name in ('fnt', 'fat'):
            entry = report['sections'][name]
            if entry['bytes'] > 8*1024*1024:
                raise ValueError('Filesystem table exceeds import limit')
            source.seek(entry['offset'])
            tables.append(source.read(entry['bytes']))
        files = index_files(*tables, report['bytes'])
        for name in selected:
            if name not in files or PurePosixPath(name).is_absolute():
                raise ValueError('Selected resource is not present in the filesystem')
            start, end = files[name]
            source.seek(start)
            if end-start < 16 or source.read(4) != b'NARC':
                raise ValueError('Only NARC resource archives are supported by this importer')
        output.mkdir(parents=True, exist_ok=True)
        if (output / 'resource-import.json').exists():
            raise FileExistsError('Import manifest already exists; choose a fresh output folder')
        result = {'rom_sha1': report['sha1'], 'game_code': report['game_code'], 'files': [],
                  'scope': 'Private selected data only; not a public game distribution'}
        for name in selected:
            start, end = files[name]
            target = output / name
            target.parent.mkdir(parents=True, exist_ok=True)
            if not target.resolve().is_relative_to(output):
                raise ValueError('Output path escapes private resource folder')
            source.seek(start)
            digest = hashlib.sha256()
            with target.open('xb') as destination:
                remaining = end-start
                while remaining:
                    data = source.read(min(remaining, 1024*1024))
                    if not data:
                        raise ValueError('Truncated resource')
                    destination.write(data)
                    digest.update(data)
                    remaining -= len(data)
            result['files'].append({'path': name, 'bytes': end-start, 'sha256': digest.hexdigest()})
        with (output / 'resource-import.json').open('x', encoding='utf-8') as manifest:
            json.dump(result, manifest, indent=2)
            manifest.write('\n')
    return result


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('rom', type=Path)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--file', action='append', required=True, dest='selected')
    args = parser.parse_args()
    try:
        print(json.dumps(import_resources(args.rom, args.output, args.selected), indent=2))
    except (OSError, ValueError, UnicodeError, RecursionError) as error:
        parser.exit(1, f'Import failed: {error}\n')
