"""Select a pinned original save-status decision function into a private build.

This does not select a save parser: status flags are controlled host fixtures.
No original resources, SDK headers, or source are vendored into this project.
"""
import argparse
import hashlib
from pathlib import Path
import urllib.request

PIN = '6984a3d67592157195940049be2e4bbbf933f9ff'
SOURCE = 'src/application/check_savedata.c'
SHA256 = 'f0132f1d62e38aa6358144ed971163336fd09377e9d708e3ceef083012e26016'


def prepare(destination):
    destination = destination.resolve()
    project = Path(__file__).resolve().parents[1]
    if destination.is_relative_to(project) or any((p / '.git').exists() for p in (destination, *destination.parents)):
        raise ValueError('Original sources require a private build folder outside Git')
    destination.mkdir(parents=True, exist_ok=True)
    original = destination / 'check_savedata.c'
    if not original.exists() or hashlib.sha256(original.read_bytes()).hexdigest() != SHA256:
        url = f'https://raw.githubusercontent.com/ClickingMaster/pokeheartgold-single-screen/{PIN}/{SOURCE}'
        with urllib.request.urlopen(url, timeout=60) as response:
            payload = response.read()
        if hashlib.sha256(payload).hexdigest() != SHA256:
            raise ValueError('Pinned save-status source differs')
        original.write_bytes(payload)
    source = original.read_text()
    start = source.index('typedef enum CheckSavedataApp_MainState {')
    end = source.index('} CheckSavedataApp_MainState;', start) + len('} CheckSavedataApp_MainState;')
    enumeration = source[start:end]
    start = source.index('static BOOL CheckSavedataApp_DoMainTask(CheckSavedataApp_Data *data) {')
    end = source.index('\nstatic BOOL CheckSavedataApp_PrintMessage(', start)
    function = source[start:end].rstrip()
    selected = '#include "checksave_bridge.h"\n' + enumeration + '\n' + function
    selected += '\nBOOL HGCheck_Run(HGCheck_Data *data) { return CheckSavedataApp_DoMainTask(data); }\n'
    (destination / 'checksave_decision.c').write_text(selected)
    print('Private original save-status decision SHA256:', hashlib.sha256(function.encode()).hexdigest())


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('destination', type=Path)
    args = parser.parse_args()
    try:
        prepare(args.destination)
    except (OSError, ValueError) as error:
        parser.exit(1, str(error) + '\n')
