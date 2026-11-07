"""Write an audited 4bpp file back as the indexed PNG the FireRed build converts with gbagfx.

The build regenerates the .4bpp from the PNG, so the PNG must carry exactly the audited
pixel indices. The palette is copied from the upstream PNG only so the file looks right
in an image viewer: gbagfx keeps the indices and ignores the colors.
"""
import argparse
from pathlib import Path

from PIL import Image
from prepare_hoenn_doors import decode_4bpp


def export(binary, width, palette_from, out):
    data = Path(binary).read_bytes()
    image = decode_4bpp(data, width, len(data) * 2 // width)
    palette = (Image.open(palette_from).getpalette() or [])[:48]
    image.putpalette(palette + [0] * (48 - len(palette)))
    out = Path(out)
    out.parent.mkdir(parents=True, exist_ok=True)
    image.save(out, bits=4)
    return image.height


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--bin', required=True, help='audited .4bpp input')
    parser.add_argument('--width', type=int, required=True, help='image width in pixels')
    parser.add_argument('--palette-from', required=True, help='upstream PNG whose first 16 colors are reused')
    parser.add_argument('--out', required=True)
    args = parser.parse_args()
    height = export(args.bin, args.width, args.palette_from, args.out)
    print(f'{args.out}: {args.width}x{height}')


if __name__ == '__main__':
    main()
