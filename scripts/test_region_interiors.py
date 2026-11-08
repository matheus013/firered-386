import unittest
from PIL import Image
from convert_hoenn_interiors import RULES, packed_sheet
from normalize_region_layers import tile_pixels


class InteriorPackingTests(unittest.TestCase):
    def test_packed_sheet_keeps_chosen_tiles_in_order(self):
        source = Image.new('P', (128, 16))
        for tile in range(32):
            for y in range(8):
                for x in range(8):
                    source.putpixel((tile % 16 * 8 + x, tile // 16 * 8 + y), (tile + x + y) % 16)
        chosen = [3, 17, 30]
        packed = packed_sheet(source, chosen)
        self.assertEqual(packed.size, (128, 8))
        for slot, tile in enumerate(chosen):
            self.assertEqual(tile_pixels(packed, slot), tile_pixels(source, tile))

    def test_rules_are_firered_behaviors_without_layer_bits(self):
        for name, (value, reason) in RULES.items():
            self.assertTrue(0 <= value < 0xF0, name)
            self.assertTrue(reason, name)
        self.assertEqual(RULES['MB_NON_ANIMATED_DOOR'][0], 0x60)
        self.assertEqual(RULES['MB_IMPASSABLE_SOUTH_AND_NORTH'][0], 0xD2)


if __name__ == '__main__':
    unittest.main()
