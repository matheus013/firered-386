"""Optional private original-C title/check-save integration; all assets synthetic.

Run against a Windows executable built with HG_REFERENCE_TITLE_FLOW=ON.
"""
from pathlib import Path
import struct
import subprocess
import sys
import tempfile

from test_graphics_renderer import archive as title_archive, png_rgba
from test_text_renderer import narc


def bank_of(messages):
    # Zero bank key keeps the allocation table plain. Each entry's payload still
    # uses its own rolling encryption seed, independently of production C++.
    table, payload = bytearray(), bytearray()
    offset = 4+8*len(messages)
    for index,units in enumerate(messages):
        table += struct.pack('<II',offset+len(payload),len(units))
        encrypted = [unit ^ (((index+1)*596947+at*18749)&0xffff) for at,unit in enumerate(units)]
        payload += struct.pack('<'+'H'*len(units),*encrypted)
    return struct.pack('<HH',len(messages),0)+table+payload


def main():
    executable = str(Path(sys.argv[1]).resolve())
    with tempfile.TemporaryDirectory(prefix='hg-title-text-') as folder:
        root = Path(folder)
        for name in ('a/0/4/6','a/0/1/6','a/0/2/7'):
            (root/name).parent.mkdir(parents=True,exist_ok=True)
        (root/'a/0/4/6').write_bytes(title_archive())
        font = struct.pack('<III4B',16,48,2,8,8,1,1)+b'\x55'*16+b'\xaa'*16+bytes([8,8])
        (root/'a/0/1/6').write_bytes(narc([font,font]))
        banks = [bytes(4)]*230
        banks[229] = bank_of([[1,0xffff],[2,0xffff],[2,0x25bc,1,0xffff],
                             [2,2,0xffff],[1,0x25bc,2,0xffff],[1,2,0xffff]])
        (root/'a/0/2/7').write_bytes(narc(banks))
        base = [executable,'--video-driver','dummy','--title-resources',str(root),
                '--checksave-text',str(root),'--title-flow-self-test']
        for flags,frame,foreground,shadow in ((1,119,576,0),(2,119,0,576),(8,119,576,576),(32,119,0,1152),
                                             (4,119,576,0),(4,131,0,576),(16,119,0,576),(16,131,576,0)):
            shot = root/f'flag-{flags}-{frame}.png'
            command = base+['--checksave-flags',str(flags),'--frames','180',
                            '--capture-at',str(frame),'--capture',str(shot)]
            run = subprocess.run(command,capture_output=True,timeout=40)
            assert run.returncode == 0, run.stdout+run.stderr
            size,pixels = png_rgba(shot)
            assert size == (1280,720)
            colors = [tuple(pixels[at:at+4]) for at in range(0,len(pixels),4)]
            assert colors.count((12,16,24,255)) == foreground, ('Wrong original message selection',flags)
            assert colors.count((128,144,160,255)) == shadow, ('Wrong original message pixels',flags)
        run = subprocess.run(base+['--checksave-flags','63','--frames','360'],capture_output=True,timeout=40)
        assert run.returncode == 0, run.stdout+run.stderr
        # Failure while preparing pages must unwind session/texture ownership.
        (root/'a/0/1/6').write_bytes(b'NARC')
        run = subprocess.run(base+['--frames','2'],capture_output=True,timeout=30)
        assert run.returncode != 0
        run = subprocess.run([executable,'--checksave-text',str(root),'--frames','2'],capture_output=True,timeout=30)
        assert run.returncode != 0
    print('PASS original title/check-save: all6 messages, flags1/2/4/8/16/32/63, page input and failure cleanup')


if __name__ == '__main__':
    main()
