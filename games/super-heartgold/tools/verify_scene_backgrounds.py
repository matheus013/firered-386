"""Render all six scene 4 backgrounds through native GX and compare every pixel."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import struct
import subprocess
import sys

from prepare_hgss_display_demo import PIN
from prepare_hgss_message_headers import read_banks
from prepare_hgss_opening_headers import ARCHIVE_SHA256, decompress_lz10
from verify_hgss_renderer import pixels


def expected_image(palette, character, screen):
    """Independent 4bpp text-map decoder; no SDK descriptor structures."""
    colors = palette[24 + struct.unpack_from('<I', palette, 36)[0]:][:0x80]
    tiles = character[24 + struct.unpack_from('<I', character, 44)[0]:]
    mapping = screen[36:]
    image = bytearray()
    for y in range(192):
        row = bytearray()
        for x in range(256):
            entry = struct.unpack_from('<H', mapping, 2 * ((y // 8) * 32 + x // 8))[0]
            tx = (x % 8) ^ (7 if entry & 0x400 else 0)
            ty = (y % 8) ^ (7 if entry & 0x800 else 0)
            packed = tiles[(entry & 0x3ff) * 32 + ty * 4 + tx // 2]
            index = (packed >> (4 * (tx % 2))) & 15
            index = (entry >> 12) * 16 + index if index else 0
            color = struct.unpack_from('<H', colors, 2 * index)[0]
            # Normalized RGB555 texture components become full-range RGB8.
            rgb = bytes((((color >> shift) & 31) * 255 + 15) // 31 for shift in (0, 5, 10))
            row.extend(rgb * 2)
        image.extend(row * 2)
    return bytes(image)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('hgss', 'libntr', 'stack', 'archive', 'output'):
        parser.add_argument('--' + name, type=Path, required=True)
    args = parser.parse_args()
    raw = args.archive.read_bytes()
    if hashlib.sha256(raw).hexdigest() != ARCHIVE_SHA256:
        raise ValueError('Opening archive hash differs')
    args.output.mkdir(parents=True, exist_ok=False)
    source, build, generated = [args.output / n for n in ('source', 'build', 'generated')]
    tools = Path(__file__).resolve().parent
    def run(command):
        result = subprocess.run(list(map(str, command)), capture_output=True, text=True, timeout=600)
        with (args.output / 'build.log').open('a') as log:
            log.write(result.stdout + result.stderr)
        result.check_returncode()
    run(['git', 'clone', '--no-checkout', args.libntr, source])
    run(['git', '-C', source, 'checkout', '--detach', PIN])
    for name in ('libntr-clip-matrix.patch', 'libntr-single-screen-prototype.patch'):
        run(['git', '-C', source, 'apply', tools / 'patches' / name])
    shutil.copyfile(tools / 'reference_compat/single_screen_touch.h', source / 'include/simulator/hg_single_screen_touch.h')
    run([sys.executable, tools / 'prepare_hgss_display_demo.py', '--hgss', args.hgss,
         '--libntr', source, '--output', generated])
    shutil.copyfile(generated / 'hgss_display.c', source / 'standalone/hgss_display.c')
    shutil.copyfile(tools / 'probe_scene_backgrounds.c', source / 'standalone/main.c')
    nns = args.stack / 'subprojects/libntrsystem'
    names = ['main.c', 'hgss_display.c']
    for name in ('g2d_NCG_load.c', 'g2d_NSC_load.c', 'g2d_NCL_load.c', 'g2d_Load.c'):
        shutil.copyfile(nns / 'libraries/g2d/src/load' / name, source / 'standalone' / name)
        names.append(name)
    flags = ['-ffunction-sections', '-fdata-sections', '-DNNS_FINALROM',
             '-I' + str(nns / 'include'), '-I' + str(nns / 'libraries/g2d/include')]
    (source / 'standalone/meson.build').write_text(
        "libntr_exe = executable('libntr', sources: files(" + ', '.join(map(repr, names)) + "),\n"
        "dependencies: [nitrosdk_dep], c_args: " + repr(flags) + ",\n"
        "link_args: ['-Wl,--gc-sections'], native: build_target != 'arm')\n")
    run(['meson', 'setup', build, source, '-Dbuild_target=linux', '-Dstandalone=true',
         '-Dcard_game_code=IPGE', '-Dcard_maker_code=01'])
    run(['meson', 'compile', '-C', build, '-j6'])
    capture = args.output / 'capture.so'
    run(['gcc', '-shared', '-fPIC', tools / 'probe_libntr_frames.c', '-I/usr/include/SDL2',
         '-lSDL2', '-ldl', '-o', capture])
    banks = read_banks(raw)
    assets = {member: banks[member] if member == 53 else decompress_lz10(banks[member])
              for member in range(53, 59)}
    cases = []
    for layer in range(1, 4):
        runtime = args.output / f'layer-{layer}'
        runtime.mkdir()
        for member, data in assets.items():
            (runtime / f'{member}.bin').write_bytes(data)
        (runtime / 'sim_config.ini').write_text('[General]\nScreenLayout=single\nSwapScreens=true\nWindowWidth=512\nWindowHeight=384\n')
        env = dict(os.environ, LD_PRELOAD=str(capture), SDL_AUDIODRIVER='dummy', HG_BG_LAYER=str(layer))
        result = subprocess.run(['timeout', '-k', '3s', '30s', str(build / 'standalone/libntr')],
                                cwd=runtime, env=env, capture_output=True, text=True, timeout=40)
        (runtime / 'run.log').write_text(result.stdout + result.stderr)
        result.check_returncode()
        for frame, member in ((60, [57, 55, 56][layer - 1]), (120, [58, 55, 56][layer - 1])):
            actual = pixels(runtime / f'libntr-frame-{frame}.ppm')
            expected = expected_image(assets[53], assets[54], assets[member])
            mismatch = sum(actual[i:i+3] != expected[i:i+3] for i in range(0, len(actual), 3))
            cases.append({'layer': layer, 'engine': 'sub' if frame == 60 else 'main',
                          'member': member, 'mismatched_pixels': mismatch})
    report = {'passed': all(c['mismatched_pixels'] == 0 for c in cases), 'cases': cases,
              'libntr_pin': PIN, 'dimensions': [512, 384], 'platform': 'linux-x86_64',
              'scope': 'isolated_static_backgrounds_not_intro_state_machine',
              'harness_sha256': hashlib.sha256((tools / 'probe_scene_backgrounds.c').read_bytes()).hexdigest()}
    (args.output / 'result.json').write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps(report))
    if not report['passed']:
        raise ValueError('Native backgrounds differ from independent tile-map decoding')


if __name__ == '__main__':
    main()
