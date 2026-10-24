"""Fetch two pinned reference sources into a PRIVATE build directory only.

The probe compiles real HG task scheduling C as host code. It is not a game
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
    args = parser.parse_args()
    destination = args.destination.resolve()
    # Never place downloaded game sources in the source tree or its Git repo.
    source = Path(__file__).resolve()
    repositories = [p for p in source.parents if (p / '.git').exists()]
    if destination.is_relative_to(source.parents[1]) or any(destination.is_relative_to(p) for p in repositories):
        parser.error('Reference probe requires a private build folder outside the repository')
    destination.mkdir(parents=True, exist_ok=True)
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
