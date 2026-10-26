"""Synthetic LZ10 bounds tests; requires no original game data."""
import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools'))
from prepare_hgss_opening_headers import decompress_lz10


class OpeningCompressionTests(unittest.TestCase):
    def test_literal(self):
        self.assertEqual(decompress_lz10(b'\x10\x03\x00\x00\x00ABC'), b'ABC')

    def test_overlapping_reference(self):
        self.assertEqual(decompress_lz10(b'\x10\x06\x00\x00\x40A\x20\x00'), b'AAAAAA')

    def test_invalid_streams(self):
        invalid = [b'', b'\x11\x01\x00\x00', b'\x10\x00\x00\x00',
                   b'\x10\x01\x00\x00', b'\x10\x01\x00\x00\x00',
                   b'\x10\x03\x00\x00\x80\x00',
                   b'\x10\x03\x00\x00\x80\x00\x00',
                   b'\x10\x02\x00\x00\x40A\x00\x00']
        for data in invalid:
            with self.subTest(data=data), self.assertRaises(ValueError):
                decompress_lz10(data)


if __name__ == '__main__':
    unittest.main()
