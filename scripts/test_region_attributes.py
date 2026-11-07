import struct
import unittest
from convert_hoenn_attributes import convert_selected


class AttributeTests(unittest.TestCase):
    def test_land_encounters_terrain_layers_and_door(self):
        raw = struct.pack('<3H', 0x2002, 0x1069, 0x003B)
        out, report = convert_selected(raw, [0, 1, 2],
                                      {2: 'MB_TALL_GRASS', 0x69: 'MB_ANIMATED_DOOR', 0x3B: 'MB_JUMP_SOUTH'}, 4)
        self.assertEqual(struct.unpack('<4I', out), (0x41000202, 0x20000069, 0x3B, 0))
        self.assertEqual(len(report), 3)

    def test_unreviewed_behavior_rejected(self):
        with self.assertRaises(ValueError):
            convert_selected(b'\xAA\0', [0], {0xAA: 'MB_NEW_BEHAVIOR'}, 1)


if __name__ == '__main__':
    unittest.main()
