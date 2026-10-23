"""Native desktop integration smoke tests. No emulator or CPU core is used."""
import argparse, hashlib, json, os, re, shutil, subprocess, tempfile
from pathlib import Path
ROOT = Path(__file__).resolve().parents[1]
parser = argparse.ArgumentParser()
parser.add_argument('--platform', choices=['windows', 'linux'], default='windows' if os.name == 'nt' else 'linux')
parser.add_argument('--save', type=Path, help='Optional existing GBA save to import into an isolated test directory')
parser.add_argument('--case', help='Run only one named case')
args = parser.parse_args()
exe = ROOT / 'output/pc' / args.platform / ('firered-386.exe' if args.platform == 'windows' else 'firered-386')
base = ROOT / 'output/native-tests' / args.platform
base.mkdir(parents=True, exist_ok=True)
base_keys = [(180, 183, 8), (300, 303, 8), (450, 453, 1), (600, 603, 1), (800, 803, 1)]
cases = [
    ('opening', [], 1000, None),
    ('normal', base_keys + [(1100, 1103, 1), (1300, 1303, 1)], 1400, (0, 0)),
    ('hardcore', base_keys + [(750, 753, 128), (1100, 1103, 1), (1300, 1303, 1)], 1400, (1, 0)),
    ('randomizer', base_keys + [(1100, 1103, 1), (1200, 1203, 128), (1300, 1303, 1)], 1400, (0, 1)),
]
import_keys = [(180, 183, 8), (300, 303, 8), (450, 453, 1), (800, 803, 1), (1100, 1103, 2), (1300, 1303, 1), (1600, 1603, 2)]
if args.save:
    cases.append(('import-save', import_keys, 2200, None))
    cases.append(('save-roundtrip', import_keys + [(2300, 2303, 8), (2400, 2403, 64),
        (2450, 2453, 64), (2500, 2503, 64), (2600, 2603, 1), (3100, 3103, 1),
        (3400, 3403, 1), (3800, 3803, 1)], 4200, None))
if args.case:
    cases = [row for row in cases if row[0] == args.case]
    if not cases: parser.error('Unknown case or --save missing')
results = []
for name, keys, frames, expected in cases:
    temporary = tempfile.TemporaryDirectory(prefix='firered-native-')
    directory = Path(temporary.name)
    save = directory / 'firered-386.sav'
    if name in {'import-save', 'save-roundtrip'}: shutil.copy2(args.save, save)
    elif save.exists(): save.unlink()  # Only this test's isolated save.
    inputs = directory / 'inputs.txt'
    inputs.write_text(''.join(f'{start} {end} {key:x}\n' for start, end, key in sorted(keys)))
    environment = dict(os.environ, SDL_VIDEODRIVER='dummy', SDL_AUDIODRIVER='dummy',
        FIRERED_PC_SAVE_DIR=str(directory), FIRERED_PC_TEST_FRAMES=str(frames), FIRERED_PC_TEST_FAST='1',
        FIRERED_PC_TEST_INPUT=str(inputs), FIRERED_PC_SCREENSHOT=str(directory / 'screen.bmp'))
    process = subprocess.run([str(exe)], cwd=exe.parent, env=environment, capture_output=True, timeout=45)
    log = (process.stdout + process.stderr).decode(errors='replace')
    (directory / 'run.log').write_text(log, encoding='utf-8')
    state = re.search(r'Native state: hardcore=(\d+) randomizer=(\d+) map=(\d+),(\d+) party=(\d+) saveCounter=(\d+)', log)
    screen = directory / 'screen.bmp'
    passed = process.returncode == 0 and f'completed: {frames} frames' in log and screen.exists() and screen.stat().st_size > 10000
    if expected: passed = passed and state is not None and tuple(map(int, state.groups()[:2])) == expected
    if name == 'import-save': passed = passed and state is not None and int(state[6]) > 0 and int(state[5]) > 0
    if name == 'save-roundtrip':
        passed = passed and state is not None and int(state[6]) > 0 and save.read_bytes() != args.save.read_bytes()
        inputs.write_text(''.join(f'{start} {end} {key:x}\n' for start, end, key in import_keys))
        environment['FIRERED_PC_TEST_FRAMES'] = '2200'
        environment['FIRERED_PC_SCREENSHOT'] = str(directory / 'reload.bmp')
        reload = subprocess.run([str(exe)], cwd=exe.parent, env=environment, capture_output=True, timeout=45)
        reload_log = (reload.stdout + reload.stderr).decode(errors='replace')
        (directory / 'reload.log').write_text(reload_log, encoding='utf-8')
        reloaded = re.search(r'Native state: hardcore=(\d+) randomizer=(\d+) map=(\d+),(\d+) party=(\d+) saveCounter=(\d+)', reload_log)
        passed = passed and reload.returncode == 0 and reloaded is not None and reloaded.groups() == state.groups()
    row = dict(case=name, passed=bool(passed), exit_code=process.returncode,
        frames=frames, state=list(map(int, state.groups())) if state else None,
        screenshot_sha256=hashlib.sha256(screen.read_bytes()).hexdigest() if screen.exists() else None)
    results.append(row)
    shutil.copytree(directory, base / name, dirs_exist_ok=True)
    temporary.cleanup()
    print(name, 'PASS' if passed else 'FAIL', row['state'], flush=True)
report = dict(platform=args.platform, executable_sha256=hashlib.sha256(exe.read_bytes()).hexdigest(),
    scope='Native opening, menu selection and optional isolated GBA save import/save/reload. Does not validate the full campaign, battles, audio quality or multiplayer.', cases=results)
(base / 'results.json').write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
raise SystemExit(0 if all(row['passed'] for row in results) else 1)
