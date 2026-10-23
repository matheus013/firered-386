"""Read-only release checks, native AI assertions and headless production-ROM boot.

Run under the configured Ubuntu/WSL build environment after build_hack.sh.
Does not rewrite release assets or load the player's save.
"""
from pathlib import Path
import hashlib
import json
import subprocess
import argparse
import re
from source_base import game_files, check_patch

ROOT = Path(__file__).resolve().parents[2]
OUT = ROOT / 'game/output/tests/checklist'
OUT.mkdir(parents=True, exist_ok=True)
results = []
parser = argparse.ArgumentParser()
parser.add_argument('--version', default='v0.4.0-dev')
args = parser.parse_args()
if not re.fullmatch(r'v\d+\.\d+\.\d+(?:-[A-Za-z0-9.-]+)?', args.version):
    parser.error('Invalid version')


def run(name, action):
    try:
        detail = action()
        results.append({'id': name, 'status': 'passed', 'detail': detail})
    except Exception as error:
        results.append({'id': name, 'status': 'failed', 'detail': str(error)})
    print(results[-1]['status'].upper(), name, flush=True)


def command(args):
    return subprocess.check_output(args, cwd=ROOT, text=True, stderr=subprocess.STDOUT).strip()


def package():
    folder = ROOT / 'game/output/release' / args.version
    prefix = 'firered-386-' + args.version
    rom = (ROOT / 'game/output/firered-386-dev.gba').read_bytes()
    assert rom == (folder / (prefix + '.gba')).read_bytes()
    for line in (folder / 'SHA256SUMS.txt').read_text().splitlines():
        digest, name = line.split(maxsplit=1)
        assert hashlib.sha256((folder / name.strip()).read_bytes()).hexdigest() == digest, name
    base = (ROOT / 'game/output/baseline.gba').read_bytes()
    assert hashlib.sha1(base).hexdigest() == '41cb23d8dccc8ebd7c649cd8fbb58eeace6e2fdc'
    data = bytearray(base)
    patch = (folder / (prefix + '.ips')).read_bytes()
    assert patch[:5] == b'PATCH'
    cursor = 5
    while patch[cursor:cursor + 3] != b'EOF':
        offset = int.from_bytes(patch[cursor:cursor + 3], 'big')
        size = int.from_bytes(patch[cursor + 3:cursor + 5], 'big')
        cursor += 5
        if size:
            block = patch[cursor:cursor + size]
            assert len(block) == size
            cursor += size
        else:
            size = int.from_bytes(patch[cursor:cursor + 2], 'big')
            block = patch[cursor + 2:cursor + 3] * size
            cursor += 3
        assert offset + size <= len(data)
        data[offset:offset + size] = block
    assert cursor + 3 == len(patch)
    assert bytes(data) == rom
    assert (folder / (prefix + '-source.patch')).read_bytes() == (ROOT / 'game/patches/current.patch').read_bytes()
    return 'All manifest hashes match; clean USA 1.0 + IPS equals the production ROM; source patch matches.'


def ai():
    runner = OUT / 'doubles-ai-test'
    command(['gcc', '-Wall', '-Wextra', '-Werror', '-Igame/include', 'game/scripts/test_doubles_ai.c', 'game/src/doubles_ai.c', '-o', str(runner)])
    return command([str(runner)])


def boot():
    runner = OUT / 'boot-smoke'
    command(['gcc', '-O2', '-Wall', '-Wextra', 'game/scripts/smoke.c', '-o', str(runner), '-lmgba', '-lpng'])
    return command([str(runner), str(ROOT / 'game/output/firered-386-dev.gba'), str(OUT / 'production-boot.png'), '900'])


def cache_sources():
    cache = Path((ROOT / 'game/output/hack-build-path.txt').read_text().strip())
    files = game_files()
    for name in files:
        assert (ROOT / 'game' / name).read_bytes() == (cache / name).read_bytes(), name
    return f'{len(files)} source files match the cache used to derive the disposable fixtures.'


def clean_rebuild():
    folder = Path((ROOT / 'game/output/release-check-build-path.txt').read_text().strip())
    assert (folder / 'pokefirered.gba').read_bytes() == (ROOT / 'game/output/firered-386-dev.gba').read_bytes()
    return 'Fresh build from pinned base plus cumulative source patch equals production ROM.'


for script in ['balance_trainers.py', 'balance_encounters.py', 'generate_guides.py']:
    run(script, lambda script=script: command(['python3', 'game/scripts/' + script, '--check']))
run('doubles_ai_native', ai)
run('production_boot_900_frames', boot)
run('release_integrity_ips', package)
run('fixture_cache_sources_match', cache_sources)
run('clean_source_rebuild_matches', clean_rebuild)
run('source_patch_applicable', check_patch)
run('source_whitespace', lambda: command(['git', '-c', 'core.autocrlf=false', 'diff', '--check', '--', 'game']))
report = {'version': args.version, 'rom_sha256': hashlib.sha256((ROOT / 'game/output/firered-386-dev.gba').read_bytes()).hexdigest(), 'checks': results}
(OUT / 'release_audit.json').write_text(json.dumps(report, indent=2) + '\n')
if any(row['status'] == 'failed' for row in results):
    raise SystemExit(1)
