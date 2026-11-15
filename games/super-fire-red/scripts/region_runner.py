"""Run every ROM case once, in isolated emulator processes, retaining failure logs."""
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path
import shutil
import subprocess


def run_cases(command, workers, output, timeout=5400):
    count = int(command[3])
    if count < 1 or workers < 1:
        raise ValueError('case count and worker count must be positive')
    workers = min(workers, count)
    output = Path(output)
    output.mkdir(parents=True, exist_ok=True)

    def run(first):
        # Separate ROM paths also isolate any save sidecars a core might create.
        folder = Path(command[1]).parent / f'worker-{first}'
        folder.mkdir(exist_ok=True)
        rom = folder / 'test.gba'
        shutil.copyfile(command[1], rom)
        args = [*command]
        args[1] = str(rom)
        args.extend([str(first), str(workers)])
        log = output / f'emulator-worker-{first}.log'
        with log.open('w') as stream:
            result = subprocess.run(args, stdout=stream, stderr=subprocess.STDOUT, timeout=timeout)
        print(f'Emulator worker {first}/{workers}: exit {result.returncode}', flush=True)
        return result.returncode, log.read_text()

    with ThreadPoolExecutor(max_workers=workers) as pool:
        results = list(pool.map(run, range(1, workers + 1)))
    combined = ''.join(log for _, log in results)
    (output / 'emulator.log').write_text(combined)
    failed = [i + 1 for i, (code, _) in enumerate(results) if code]
    if failed:
        raise RuntimeError(f'Emulator workers failed: {failed}; see {output}')
    return combined
