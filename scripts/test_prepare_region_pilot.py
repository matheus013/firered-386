import struct
import unittest
from prepare_region_pilot import remap_blocks


class BlockConversionTests(unittest.TestCase):
    def test_every_collision_and_elevation(self):
        for collision in range(4):
            for elevation in range(16):
                high = collision << 10 | elevation << 12
                values = [high | tile for tile in (0, 511, 512, 800)]
                result = struct.unpack('<4H', remap_blocks(struct.pack('<4H', *values), 512))
                self.assertEqual(result, tuple(high | tile for tile in (0, 511, 640, 928)))

    def test_identity_and_sentinel(self):
        raw = struct.pack('<4H', 0, 640, 0xFC01, 1023)
        self.assertEqual(remap_blocks(raw, 640), raw)

    def test_rejects_overflow_and_truncation(self):
        for raw in (b'\0', struct.pack('<H', 900), struct.pack('<H', 895)):
            with self.assertRaises(ValueError):
                remap_blocks(raw, 512)


if __name__ == '__main__':
    unittest.main()
