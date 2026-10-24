"""Tests use synthetic bytes, never game data or ROMs."""
import importlib.util
import json
import struct
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


def load(name):
    spec = importlib.util.spec_from_file_location(name, ROOT / 'tools' / f'{name}.py')
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


class InspectorTests(unittest.TestCase):
    def setUp(self):
        self.temporary = tempfile.TemporaryDirectory()
        self.addCleanup(self.temporary.cleanup)
        self.path = Path(self.temporary.name) / 'synthetic.nds'
        self.inspector = load('inspect_nds')

    def test_truncated_header(self):
        self.path.write_bytes(b'not a game')
        with self.assertRaises(ValueError):
            self.inspector.inspect(self.path)

    def test_out_of_bounds_section(self):
        header = bytearray(512)
        struct.pack_into('<II', header, 0x20, 600, 0)
        self.path.write_bytes(header)
        with self.assertRaises(ValueError):
            self.inspector.inspect(self.path)

    def test_valid_synthetic_header_and_no_payload_copy(self):
        header = bytearray(512)
        header[:12] = b'OWN FIXTURE\0'
        header[12:16] = b'TEST'
        struct.pack_into('<H', header, 0x15E, self.inspector.crc16(header[:0x15E]))
        self.path.write_bytes(header)
        result = self.inspector.inspect(self.path)
        self.assertTrue(result['header_crc_ok'])
        self.assertEqual(result['game_code'], 'TEST')
        self.assertEqual(list(self.path.parent.iterdir()), [self.path])


class AssetTests(unittest.TestCase):
    def setUp(self):
        self.temporary = tempfile.TemporaryDirectory()
        self.addCleanup(self.temporary.cleanup)
        self.assets = Path(self.temporary.name) / 'assets'
        self.assets.mkdir()
        self.checker = load('check_assets')

    def manifest(self, entries):
        (self.assets / 'manifest.json').write_text(json.dumps({'version': 1, 'assets': entries}))

    def entry(self, path):
        return dict(path=path, author='Fixture author', license='Own test content', origin='original')

    def test_empty_and_original_manifest(self):
        self.manifest([])
        self.assertFalse(self.checker.check(self.assets))
        (self.assets / 'own.txt').write_text('Synthetic test content')
        self.manifest([self.entry('own.txt')])
        self.assertFalse(self.checker.check(self.assets))

    def test_path_traversal(self):
        (self.assets.parent / 'outside.txt').write_text('private fixture')
        self.manifest([self.entry('../outside.txt')])
        self.assertTrue(self.checker.check(self.assets))

    def test_unlisted_file(self):
        self.manifest([])
        (self.assets / 'own.txt').write_text('fixture')
        self.assertTrue(self.checker.check(self.assets))

    def test_forbidden_dump(self):
        self.manifest([self.entry('synthetic.nds')])
        (self.assets / 'synthetic.nds').write_bytes(b'SYNTHETIC')
        self.assertTrue(self.checker.check(self.assets))

    def test_bad_schema(self):
        (self.assets / 'manifest.json').write_text('[]')
        self.assertTrue(self.checker.check(self.assets))


if __name__ == '__main__':
    unittest.main()
