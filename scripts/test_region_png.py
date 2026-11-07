import tempfile
import unittest
from pathlib import Path
from PIL import Image
from export_region_png import export
from prepare_hoenn_doors import encode_4bpp


class ExportPngTests(unittest.TestCase):
    def test_round_trip_keeps_every_index(self):
        # Two 8x8 tiles wide, three tall, using all 16 indices: what gbagfx reads back must match.
        data = bytes((i * 37 + 11) % 256 for i in range(6 * 32))
        with tempfile.TemporaryDirectory() as folder:
            folder = Path(folder)
            (folder / 'in.4bpp').write_bytes(data)
            upstream = Image.new('P', (8, 8))
            upstream.putpalette([n for i in range(16) for n in (i * 16, 0, 255 - i * 16)])
            upstream.save(folder / 'upstream.png')
            self.assertEqual(export(folder / 'in.4bpp', 16, folder / 'upstream.png', folder / 'out.png'), 24)
            image = Image.open(folder / 'out.png')
            self.assertEqual((image.mode, image.size), ('P', (16, 24)))
            self.assertEqual(encode_4bpp(image), data)
            self.assertEqual(image.getpalette()[:6], [0, 0, 255, 16, 0, 239])

    def test_rejects_width_that_does_not_fit(self):
        with tempfile.TemporaryDirectory() as folder:
            folder = Path(folder)
            (folder / 'in.4bpp').write_bytes(bytes(32))
            Image.new('P', (8, 8)).save(folder / 'upstream.png')
            with self.assertRaises(ValueError):
                export(folder / 'in.4bpp', 12, folder / 'upstream.png', folder / 'out.png')


if __name__ == '__main__':
    unittest.main()
