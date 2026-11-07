"""Exercise the actual door C functions with host stubs, not a game build.

Run under Linux/WSL with cc and AddressSanitizer installed.
"""
import os
from pathlib import Path
import re
import shutil
import subprocess
import tempfile
import unittest


SOURCE = Path(__file__).resolve().parents[1] / 'games/super-fire-red/src/field_door.c'


def function(source, name):
    match = re.search(r'^static [^;\n]+\b' + name + r'\([^;]*?\)\s*\{', source, re.M)
    if not match:
        raise ValueError(f'Missing definition: {name}')
    depth, cursor = 1, match.end()
    while depth:
        depth += (source[cursor] == '{') - (source[cursor] == '}')
        cursor += 1
    return source[match.start():cursor]


@unittest.skipUnless(os.name != 'nt' and shutil.which('cc'), 'Run in Linux/WSL with cc')
class DoorNativeTests(unittest.TestCase):
    def test_palette_bounds_and_closed_door_lookup(self):
        source = SOURCE.read_text()
        header = r'''
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
typedef uint8_t u8;
typedef uint16_t u16;
enum { DOOR_SIZE_1x1, DOOR_SIZE_1x2 };
struct DoorGraphics { u16 metatileId; u8 sound, size; const u8 *tiles, *paletteNums; };
static int draws, lastX, lastY;
static u16 metatile;
static u16 MapGridGetMetatileIdAt(int x, int y) { (void)x; (void)y; return metatile; }
static void CurrentMapDrawMetatileAt(int x, int y) { draws++; lastX=x; lastY=y; }
'''
        code = header + '\n'.join(function(source, name) for name in (
            'BuildDoorTiles', 'GetDoorGraphics', 'DrawClosedDoorTiles', 'DrawClosedDoor'))
        code += r'''
int main(void) {
    /* Exactly eight bytes: ASan detects reading the ninth byte in the lower half. */
    const u8 palettes[8] = {1,2,3,4,5,6,7,8};
    const u8 pixels[1] = {0};
    u16 tiles[8];
    const struct DoorGraphics table[] = {
        {10,0,DOOR_SIZE_1x1,pixels,palettes},
        {20,0,DOOR_SIZE_1x2,pixels,palettes},
        {0,0,0,NULL,NULL}
    };
    for (int half=0; half<2; half++) {
        BuildDoorTiles(tiles, 1016+half*4, palettes+half*4);
        for (int i=0; i<4; i++) {
            assert(tiles[i] == ((palettes[half*4+i]<<12) | (1016+half*4+i)));
            assert(tiles[i+4] == (palettes[half*4+i]<<12));
        }
    }
    metatile=20; draws=0; DrawClosedDoor(table,7,9);
    assert(draws==2 && lastX==7 && lastY==8);
    metatile=10; draws=0; DrawClosedDoor(table,7,9);
    assert(draws==1 && lastY==9);
    metatile=99; draws=0; DrawClosedDoor(table,7,9);
    assert(draws==0);
    return 0;
}
'''
        with tempfile.TemporaryDirectory(prefix='firered-door-test-') as directory:
            path = Path(directory)
            (path / 'test.c').write_text(code)
            subprocess.run(['cc', '-std=c99', '-Wall', '-Wextra', '-Werror',
                            '-fsanitize=address,undefined', '-fno-omit-frame-pointer',
                            '-g', str(path / 'test.c'), '-o', str(path / 'test')], check=True)
            subprocess.run([str(path / 'test')], check=True)


if __name__ == '__main__':
    unittest.main()
