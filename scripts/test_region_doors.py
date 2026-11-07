import unittest
from PIL import Image
from prepare_hoenn_doors import encode_4bpp, decode_4bpp


class DoorTests(unittest.TestCase):
    def test_explicit_palette_bank_conversion(self):
        image = Image.new('P', (8, 8), 28)
        self.assertEqual(encode_4bpp(image, banked=True), bytes([0xCC] * 32))

    def test_nibble_and_tile_order(self):
        image = Image.new('P', (16, 16))
        for y in range(16):
            for x in range(16):
                image.putpixel((x, y), (x + 3*y) % 16)
        data = encode_4bpp(image)
        self.assertEqual(data[0], 0x10)
        self.assertEqual(data[32], 0x98)
        self.assertEqual(decode_4bpp(data, 16, 16).tobytes(), image.tobytes())

    def test_invalid_dimensions_and_pixels(self):
        with self.assertRaises(ValueError):
            encode_4bpp(Image.new('P', (7, 8)))
        with self.assertRaises(ValueError):
            encode_4bpp(Image.new('P', (8, 8), 16))
        with self.assertRaises(ValueError):
            decode_4bpp(bytes(31), 8, 8)


if __name__ == '__main__':
    unittest.main()
