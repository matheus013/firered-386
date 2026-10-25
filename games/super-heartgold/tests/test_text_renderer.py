"""Native SDL font/message integration using authorial fixtures only."""
from pathlib import Path
import struct
import subprocess
import sys
import tempfile

from test_graphics_renderer import png_rgba


def narc(members):
    table, data = bytearray(), bytearray()
    for member in members:
        table += struct.pack('<II',len(data),len(data)+len(member))
        data += member
    fat = b'BTAF'+struct.pack('<IHH',12+len(table),len(members),0)+table
    names = b'BTNF'+struct.pack('<II',12,0)
    image = b'GMIF'+struct.pack('<I',8+len(data))+data
    body = fat+names+image
    return b'NARC'+struct.pack('<HHIHH',0xfffe,0x100,16+len(body),16,3)+body


def message_bank(units):
    key = 0x4567
    mask16 = (key*765) & 0xffff
    mask32 = mask16 | (mask16 << 16)
    encrypted = [unit ^ ((596947+index*18749)&0xffff) for index,unit in enumerate(units)]
    return struct.pack('<HHII',1,key,12^mask32,len(units)^mask32)+struct.pack('<'+'H'*len(units),*encrypted)


def main():
    executable = str(Path(sys.argv[1]).resolve())
    with tempfile.TemporaryDirectory(prefix='hg-text-renderer-') as folder:
        root = Path(folder)/'ação-ポケモン'
        font_path, text_path = root/'a/0/1/6', root/'a/0/2/7'
        font_path.parent.mkdir(parents=True)
        text_path.parent.mkdir(parents=True)
        font = struct.pack('<III4B',16,48,2,8,8,1,1)+b'\x55'*16+b'\xaa'*16+bytes([8,8])
        font_path.write_bytes(narc([font]))
        text_path.write_bytes(narc([message_bank([1,2,0xe000,2,1,0xffff])]))
        shot = root/'captura-記録.png'
        base = [executable,'--video-driver','dummy','--frames','2','--text-resources',str(root),
                '--message-bank','0','--message-id','0']
        result = subprocess.run(base+['--capture',str(shot)],capture_output=True,timeout=30)
        assert result.returncode == 0, result.stdout+result.stderr
        size,pixels = png_rgba(shot)
        assert size == (1280,720)
        colors = [tuple(pixels[offset:offset+4]) for offset in range(0,len(pixels),4)]
        assert colors.count((12,16,24,255)) == 1152, 'Foreground glyph pixels or layout differs'
        assert colors.count((128,144,160,255)) == 1152, 'Shadow glyph pixels or linefeed differs'
        before = shot.read_bytes()
        result = subprocess.run(base+['--capture',str(shot)],capture_output=True,timeout=30)
        assert result.returncode != 0 and shot.read_bytes() == before, 'Existing Unicode capture overwritten'
        for units in ([0xfffe,0x100,0,0xffff], [1]*40+[0xffff], [1,0x25bc,2,0xffff], [1,0x25bd,0xffff]):
            text_path.write_bytes(narc([message_bank(units)]))
            assert subprocess.run(base,capture_output=True,timeout=30).returncode != 0, 'Unsupported message accepted'
        text_path.write_bytes(narc([message_bank([1,0xffff])]))
        assert subprocess.run(base+['--title-resources',str(root)],capture_output=True,timeout=30).returncode != 0
        assert subprocess.run(base+['--message-id','1'],capture_output=True,timeout=30).returncode != 0
        font_path.write_bytes(b'NARC')
        assert subprocess.run(base,capture_output=True,timeout=30).returncode != 0
    print('PASS native text renderer: font roles, message decrypt, LF, scale, Unicode paths and failures')


if __name__ == '__main__':
    main()
