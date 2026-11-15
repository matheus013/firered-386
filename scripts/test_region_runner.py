"""Regression checks for complete coverage and failure reporting across shards."""
import importlib.util
from pathlib import Path
import tempfile
from types import SimpleNamespace
import unittest
from unittest.mock import patch

MODULE = Path(__file__).resolve().parents[1] / 'games/super-fire-red/scripts/region_runner.py'
spec = importlib.util.spec_from_file_location('region_runner', MODULE)
runner = importlib.util.module_from_spec(spec)
spec.loader.exec_module(runner)


class RegionRunnerTests(unittest.TestCase):
    def run_fixture(self, count, workers, fail=False):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            rom = root / 'test.gba'
            rom.write_bytes(b'fixture')
            paths = []

            def emulate(args, stdout, stderr, timeout):
                first, stride = map(int, args[-2:])
                paths.append(args[1])
                self.assertEqual(Path(args[1]).read_bytes(), b'fixture')
                for case in range(first, int(args[3]) + 1, stride):
                    stdout.write(f'{case}\n')
                return SimpleNamespace(returncode=1 if fail and first == 1 else 0)

            with patch.object(runner.subprocess, 'run', side_effect=emulate):
                command = ['runner', str(rom), str(root / 'map'), str(count)]
                if fail:
                    with self.assertRaisesRegex(RuntimeError, 'workers failed'):
                        runner.run_cases(command, workers, root / 'logs')
                    self.assertTrue((root / 'logs/emulator.log').is_file())
                    return
                result = runner.run_cases(command, workers, root / 'logs')
            self.assertEqual(sorted(map(int, result.splitlines())), list(range(1, count + 1)))
            self.assertEqual(len(set(paths)), min(workers, count))

    def test_uneven_partition_has_every_case_once(self):
        self.run_fixture(23, 4)

    def test_more_workers_than_cases(self):
        self.run_fixture(2, 8)

    def test_serial_mode(self):
        self.run_fixture(5, 1)

    def test_failure_is_not_reported_as_success(self):
        self.run_fixture(5, 2, fail=True)

    def test_invalid_worker_count(self):
        with self.assertRaises(ValueError):
            runner.run_cases(['runner', 'rom', 'map', '5'], 0, '.')


if __name__ == '__main__':
    unittest.main()
