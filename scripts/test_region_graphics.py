import unittest
from PIL import Image
from convert_hoenn_graphics import pad_sheet, rendered_entry


class GraphicsTests(unittest.TestCase):
    def test_flips_transparency_and_palette(self):
        sheet = Image.new('P', (8, 8), 0)
        sheet.putpixel((0, 0), 1)
        palettes = {2: [(0, 0, 0), (255, 0, 0)] + [(0, 0, 0)] * 14}
        for flip, pos in [(0, 0), (0x400, 7), (0x800, 56), (0xC00, 63)]:
            result = rendered_entry(0x2000 | flip, sheet, sheet, 512, palettes)
            self.assertEqual(result[pos], (255, 0, 0))
            self.assertEqual(result.count(None), 63)

    def test_padding_preserves_pixels_and_rejects_non_4bpp(self):
        sheet = Image.new('P', (128, 8), 3)
        padded = pad_sheet(sheet, 640)
        self.assertEqual(padded.size, (128, 320))
        self.assertEqual(padded.getpixel((127, 7)), 3)
        self.assertEqual(padded.getpixel((0, 8)), 0)
        sheet.putpixel((0, 0), 16)
        with self.assertRaises(ValueError):
            pad_sheet(sheet, 640)


if __name__ == '__main__':
    unittest.main()
