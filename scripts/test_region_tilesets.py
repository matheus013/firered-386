import struct
import unittest
from region_tilesets import convert_entries, convert_attributes


class TilesetTests(unittest.TestCase):
    def test_flip_and_palette_remapping(self):
        for flip in (0, 0x400, 0x800, 0xC00):
            raw = struct.pack('<H', 512 | flip | 6 << 12)
            result = convert_entries(raw, {512: 640}, {6: 7})
            self.assertEqual(struct.unpack('<H', result)[0], 640 | flip | 7 << 12)

    def test_layer_and_reviewed_attributes(self):
        for layer in range(3):
            raw = struct.pack('<H', 2 | layer << 12)
            self.assertEqual(struct.unpack('<I', convert_attributes(raw, {2: 0x01000202}))[0],
                             0x01000202 | layer << 29)

    def test_reject_unknowns_and_overflows(self):
        with self.assertRaises(ValueError):
            convert_entries(b'\0\0', {}, {})
        with self.assertRaises(ValueError):
            convert_entries(b'\0\0', {0: 1024}, {0: 0})
        with self.assertRaises(ValueError):
            convert_entries(b'\0\0', {0: 0}, {0: 13})
        for raw, rules in [(b'\0', {}), (b'\x01\0', {}), (b'\0\x01', {0: 0}),
                           (b'\0\x30', {0: 0}), (b'\0\0', {0: 0x60000000})]:
            with self.assertRaises(ValueError):
                convert_attributes(raw, rules)


if __name__ == '__main__':
    unittest.main()
