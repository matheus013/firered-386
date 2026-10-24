"""Fetch pinned reference sources into a PRIVATE build directory only.

The probes compile real HG task scheduling/resource C as host code. This is not a game
port and its generated/reference sources must not be publicly packaged.
"""
import argparse
import hashlib
import urllib.request
from pathlib import Path

PIN = '6984a3d67592157195940049be2e4bbbf933f9ff'
FILES = {
    'src/sys_task.c': '9ef0caac3b9c0f869e50e5df5278e4de50f65b0b76813ea4ecd533acc5bc767a',
    'include/sys_task.h': '100b3c1d5a0cc2c26ff743679e8782b039e1fb7bf135fad0b30fb5ff5cd04ffb',
}

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('destination', type=Path)
    parser.add_argument('--filesystem', action='store_true', help='Also prepare the private native resource reader probe')
    args = parser.parse_args()
    destination = args.destination.resolve()
    # Never place downloaded game sources in the source tree or its Git repo.
    source = Path(__file__).resolve()
    repositories = [p for p in source.parents if (p / '.git').exists()]
    if destination.is_relative_to(source.parents[1]) or any(destination.is_relative_to(p) for p in repositories):
        parser.error('Reference probe requires a private build folder outside the repository')
    destination.mkdir(parents=True, exist_ok=True)
    if args.filesystem:
        FILES.update({
            'src/filesystem.c': 'ee526fc2835b2c2259dd393087fb1a5ca036dbf467bcfbd7735b1476819c869d',
            'include/filesystem.h': '50d8756e99ae5d980d85c4dfeb63291c99c1e17a47d3f810e693ef0237fa920e',
            'include/filesystem_files_def.h': '871b6abe3fb53e5ff810bb0b0680f2e305daff9670565e4d3eb9c13aebc3c5cf',
        })
    for relative, expected in FILES.items():
        target = destination / Path(relative).name
        if target.exists() and hashlib.sha256(target.read_bytes()).hexdigest() == expected:
            continue
        url = f'https://raw.githubusercontent.com/ClickingMaster/pokeheartgold-single-screen/{PIN}/{relative}'
        with urllib.request.urlopen(url, timeout=60) as response:
            data = response.read()
        if hashlib.sha256(data).hexdigest() != expected:
            raise SystemExit(f'Reference source checksum differs: {relative}')
        target.write_bytes(data)
    if args.filesystem:
        target = destination / 'filesystem.c'
        original = target.read_bytes()
        before = b'// Bug: File is never closed\n    return chunk_size;'
        after = b'// Host port: release the file before returning its member size.\n    FS_CloseFile(&file);\n    return chunk_size;'
        if original.count(before) != 1:
            raise SystemExit('Filesystem close patch did not match the pinned source')
        target.write_bytes(original.replace(before, after))
