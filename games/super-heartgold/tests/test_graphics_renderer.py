"""Exercise the actual SDL renderer with authorial synthetic resource files."""
from pathlib import Path
import struct
import subprocess
import sys
import tempfile
import zlib


def container(magic, section, body):
    block = section + struct.pack('<I', 8+len(body)) + body
    return magic + struct.pack('<HHIHH', 0xFEFF, 0x100, 16+len(block), 16, 1) + block


def characters(eight, pixels):
    return container(b'RGCN', b'RAHC', struct.pack('<HHIIIII', 1, 1, 4 if eight else 3, 0, 0, len(pixels), 24)+pixels)


def screen(eight):
    return container(b'RCSN', b'NRCS', struct.pack('<HHHHIH', 8, 8, 1 if eight else 0, 0, 2, 0))


def literal_lz(data, compression):
    output = bytearray([compression]) + len(data).to_bytes(3, 'little')
    for offset in range(0, len(data), 8):
        output += b'\0' + data[offset:offset+8]
    return output


def archive(compression=None):
    palette = bytearray(512)
    struct.pack_into('<HH', palette, 2, 31, 31 << 5)
    members = [b'']*38
    members[2] = container(b'RLCN', b'TTLP', struct.pack('<IIII', 4, 0, 512, 16)+palette)
    members[36] = characters(True, bytes([1]*64))
    members[37] = screen(True)
    members[1] = characters(True, bytes(2 if x < 4 else 0 for y in range(8) for x in range(8)))
    members[0] = screen(True)
    members[15] = characters(False, bytes(32))
    members[17] = screen(False)
    if compression is not None:
        members = [literal_lz(member, compression) if member else member for member in members]
    table, data = bytearray(), bytearray()
    for member in members:
        table += struct.pack('<II', len(data), len(data)+len(member))
        data += member
    fat = b'BTAF'+struct.pack('<IHH', 12+len(table), len(members), 0)+table
    names = b'BTNF'+struct.pack('<II', 12, 0)
    image = b'GMIF'+struct.pack('<I',8+len(data))+data
    body = fat+names+image
    return b'NARC'+struct.pack('<HHIHH',0xFFFE,0x100,16+len(body),16,3)+body


def png_rgba(path):
    # Test-only reader for SDL's 8-bit RGBA, noninterlaced PNG captures.
    b = path.read_bytes()
    assert b[:8] == b'\x89PNG\r\n\x1a\n'
    cursor, packed, dimensions = 8, bytearray(), None
    while cursor < len(b):
        count = struct.unpack_from('>I', b, cursor)[0]
        tag, data = b[cursor+4:cursor+8], b[cursor+8:cursor+8+count]
        assert zlib.crc32(tag+data) == struct.unpack_from('>I', b, cursor+8+count)[0]
        if tag == b'IHDR':
            width,height,depth,color,compression,filtering,interlace = struct.unpack('>IIBBBBB',data)
            assert (depth,color,compression,filtering,interlace) == (8,6,0,0,0)
            dimensions = width,height
        if tag == b'IDAT':
            packed += data
        cursor += 12+count
    assert dimensions
    width,height = dimensions
    raw, stride, pixels = zlib.decompress(packed), width*4, bytearray()
    assert len(raw) == (stride+1)*height
    previous = bytearray(stride)
    for y in range(height):
        mode, row = raw[y*(stride+1)], bytearray(raw[y*(stride+1)+1:(y+1)*(stride+1)])
        assert mode <= 4
        if mode == 1:
            for x in range(4,stride):
                row[x] = (row[x]+row[x-4])&255
        elif mode == 2:
            row = bytearray((value+up)&255 for value,up in zip(row,previous))
        elif mode in (3,4):
            for x in range(stride):
                left,up,corner = row[x-4] if x>=4 else 0, previous[x], previous[x-4] if x>=4 else 0
                if mode == 3:
                    predictor = (left+up)//2
                else:
                    prediction = left+up-corner
                    distances = [abs(prediction-left),abs(prediction-up),abs(prediction-corner)]
                    predictor = [left,up,corner][distances.index(min(distances))]
                row[x] = (row[x]+predictor)&255
        pixels += row
        previous = row
    return dimensions, pixels


def main():
    executable = str(Path(sys.argv[1]).resolve())
    with tempfile.TemporaryDirectory(prefix='hg-renderer-') as folder:
        root = Path(folder)
        target = root/'a/0/4/6'
        target.parent.mkdir(parents=True)
        target.write_bytes(archive())
        shot = root/'capture.png'
        base = [executable,'--video-driver','dummy','--frames','2','--title-resources',str(root)]
        result = subprocess.run(base+['--capture-at','1','--capture',str(shot)],capture_output=True,text=True,timeout=30)
        assert result.returncode == 0, result.stdout+result.stderr
        size,pixels = png_rgba(shot)
        assert size == (1280,720)
        colors = [tuple(pixels[p:p+4]) for p in range(0,len(pixels),4)]
        assert colors.count((255,0,0,255)) == 288, 'Opaque background / transparent foreground failed'
        assert colors.count((0,255,0,255)) == 288, 'Nearest texture scaling or palette upload failed'
        for compression in (0x10, 0x11):
            target.write_bytes(archive(compression))
            compressed_shot = root/f'compressed-{compression}.png'
            result = subprocess.run(base+['--capture-at','1','--capture',str(compressed_shot)],
                                    capture_output=True,text=True,timeout=30)
            assert result.returncode == 0, result.stdout+result.stderr
            compressed_size, compressed_pixels = png_rgba(compressed_shot)
            assert compressed_size == size
            compressed_colors = [tuple(compressed_pixels[p:p+4]) for p in range(0,len(compressed_pixels),4)]
            assert compressed_colors.count((255,0,0,255)) == 288, 'Compressed background failed'
            assert compressed_colors.count((0,255,0,255)) == 288, 'Compressed foreground failed'
        target.write_bytes(archive())
        before = shot.read_bytes()
        result = subprocess.run(base+['--capture',str(shot)],capture_output=True,timeout=30)
        assert result.returncode != 0 and shot.read_bytes() == before, 'Existing capture was overwritten'
        missing_shot=root/'not-reached.png'
        result=subprocess.run(base+['--capture-at','2','--capture',str(missing_shot)],capture_output=True,timeout=30)
        assert result.returncode != 0 and not missing_shot.exists(), 'Unreached capture frame should fail'
        target.write_bytes(b'NARC')
        result = subprocess.run(base,capture_output=True,timeout=30)
        assert result.returncode != 0, 'Malformed resource should fail startup'
    print('PASS native renderer: raw/LZ10/LZ11 fixtures, nearest RGBA upload, layering, capture and error paths')


if __name__ == '__main__':
    main()
