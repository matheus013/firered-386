"""Synthetic NitroFS tests: no original game data."""
from pathlib import Path
import struct
import sys
import unittest
import tempfile

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools'))
from import_nds_resources import index_files, import_resources
from inspect_nds import crc16


class FilesystemTests(unittest.TestCase):
    def fixture(self):
        # Root contains folder a, which contains file x.
        return (struct.pack('<IHHIHH', 16, 0, 2, 21, 0, 0xF000) +
                b'\x81a\x01\xf0\0\x01x\0', struct.pack('<II', 40, 44))

    def test_nested_file(self):
        self.assertEqual(index_files(*self.fixture(), 50), {'a/x': (40, 44)})

    def test_all_truncations_rejected(self):
        fnt, fat = self.fixture()
        for size in range(len(fnt)):
            with self.subTest(size=size), self.assertRaises(ValueError):
                index_files(fnt[:size], fat, 50)

    def test_allocation_outside_rom(self):
        fnt, _ = self.fixture()
        for fat in (struct.pack('<II', 40, 51), struct.pack('<II', 44, 40), b'123'):
            with self.assertRaises(ValueError):
                index_files(fnt, fat, 50)

    def test_cycle_rejected(self):
        fnt, fat = self.fixture()
        fnt = fnt[:18] + b'\0\xf0' + fnt[20:]
        with self.assertRaises(ValueError):
            index_files(fnt, fat, 50)

    def test_unsafe_filename_rejected(self):
        fnt, fat = self.fixture()
        for char in (b'/', b'\\', b':', b'\0'):
            with self.assertRaises(ValueError):
                index_files(fnt[:22]+char+fnt[23:], fat, 50)

    def dump(self, path, payload=b'NARC'+bytes(12)):
        data = bytearray(1024)
        fnt = struct.pack('<IHH', 8, 0, 1)+b'\x04data\0'
        struct.pack_into('<IIII', data, 0x40, 0x200, len(fnt), 0x220, 8)
        data[0x200:0x200+len(fnt)] = fnt
        struct.pack_into('<II', data, 0x220, 0x240, 0x240+len(payload))
        data[0x240:0x240+len(payload)] = payload
        struct.pack_into('<H', data, 0x15e, crc16(data[:0x15e]))
        path.write_bytes(data)

    def test_import_selected_and_never_overwrite(self):
        with tempfile.TemporaryDirectory() as folder:
            root = Path(folder)
            self.dump(root/'test.nds')
            result = import_resources(root/'test.nds', root/'private', ['data'])
            self.assertEqual(result['files'][0]['bytes'],16)
            self.assertEqual((root/'private/data').read_bytes(), b'NARC'+bytes(12))
            with self.assertRaises(FileExistsError):
                import_resources(root/'test.nds', root/'private', ['data'])

    def test_missing_non_archive_and_public_output_rejected(self):
        with tempfile.TemporaryDirectory() as folder:
            root = Path(folder)
            self.dump(root/'test.nds', bytes(16))
            for selected in (['missing'], ['data'], ['data','data']):
                with self.assertRaises(ValueError):
                    import_resources(root/'test.nds', root/'private', selected)
            self.assertFalse((root/'private').exists())
            with self.assertRaises(ValueError):
                import_resources(root/'test.nds', Path(__file__).parent/'forbidden-output', ['data'])


if __name__ == '__main__':
    unittest.main()
