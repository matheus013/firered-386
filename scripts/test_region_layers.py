import struct
import unittest
from PIL import Image
from normalize_region_layers import normalize


class LayerTests(unittest.TestCase):
    def test_opaque_upper_proves_hidden_lower(self):
        image = Image.new('P', (8, 8), 1)
        raw = struct.pack('<8H', *([0xe320]*4 + [0]*4))
        result, proof = normalize(raw, b'\0\0', [0], image, image, 512)
        self.assertEqual(result, bytes(16))
        self.assertEqual(len(proof), 4)

    def test_rejects_transparency_or_non_normal_layer(self):
        image = Image.new('P', (8, 8), 1)
        raw = struct.pack('<8H', *([0xe320]*4 + [0]*4))
        with self.assertRaises(ValueError):
            normalize(raw, b'\0\x10', [0], image, image, 512)
        image.putpixel((3, 4), 0)
        with self.assertRaises(ValueError):
            normalize(raw, b'\0\0', [0], image, image, 512)

    def test_preserves_valid_entries(self):
        image = Image.new('P', (8, 8), 1)
        raw = struct.pack('<8H', *([0x0400]*8))
        result, proof = normalize(raw, b'\0\0', [0], image, image, 512)
        self.assertEqual(result, raw)
        self.assertEqual(proof, [])


if __name__ == '__main__':
    unittest.main()
