"""Linux/WSLg integration gate: build and capture original HGSS display routing.
Requires GCC/G++, Meson, Ninja, NASM, Flex, Bison, SDL2, ENet and OpenGL.
Uses a new private workspace; never modifies the supplied source checkouts.
"""
import argparse
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys
from prepare_hgss_display_demo import PIN


def run(command, *, cwd=None, env=None, timeout=600):
    subprocess.run([str(value) for value in command], cwd=cwd, env=env,
                   check=True, timeout=timeout)


def pixels(path):
    with path.open('rb') as stream:
        if stream.readline() != b'P6\n': raise ValueError('Expected PPM RGB capture')
        size = stream.readline().split()
        if size != [b'512', b'384'] or stream.readline() != b'255\n':
            raise ValueError('Unexpected single-screen dimensions or RGB format')
        data = stream.read(512 * 384 * 3 + 1)
    if len(data) != 512 * 384 * 3: raise ValueError('Truncated or oversized capture')
    return data


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--hgss', type=Path, required=True)
    parser.add_argument('--libntr', type=Path, required=True)
    parser.add_argument('--workspace', type=Path, required=True)
    args = parser.parse_args()
    root = Path(__file__).resolve().parents[1]
    work = args.workspace.resolve()
    work.mkdir(parents=True, exist_ok=False)
    source, build, generated, runtime = [work/name for name in ('source','build','generated','runtime')]
    run(['git','clone','--no-checkout',args.libntr.resolve(),source])
    run(['git','-C',source,'checkout','--detach',PIN])
    for name in ('libntr-clip-matrix.patch','libntr-single-screen-prototype.patch'):
        run(['git','-C',source,'apply','--check',root/'tools/patches'/name])
        run(['git','-C',source,'apply',root/'tools/patches'/name])
    shutil.copyfile(root/'tools/reference_compat/single_screen_touch.h',
                    source/'include/simulator/hg_single_screen_touch.h')
    run([sys.executable,root/'tools/prepare_hgss_display_demo.py','--hgss',args.hgss.resolve(),
         '--libntr',source,'--output',generated])
    for name in ('main.c','hgss_display.c'):
        shutil.copyfile(generated/name,source/'standalone'/name)
    meson = source/'standalone/meson.build'
    text = meson.read_text()
    old = "sources: files('main.c')"
    if text.count(old) != 1: raise ValueError('Standalone build contract differs')
    meson.write_text(text.replace(old,"sources: files('main.c', 'hgss_display.c')"))
    run(['meson','setup',build,source,'-Dbuild_target=linux','-Dstandalone=true',
         '-Dcard_game_code=IPGE','-Dcard_maker_code=01'])
    run(['meson','compile','-C',build,'-j4'])
    runtime.mkdir()
    run(['gcc','-shared','-fPIC',root/'tools/probe_libntr_frames.c','-I/usr/include/SDL2',
         '-lSDL2','-ldl','-o',runtime/'capture.so'])
    (runtime/'sim_config.ini').write_text('[General]\nScreenLayout=single\nSwapScreens=true\nWindowWidth=512\nWindowHeight=384\n')
    env = dict(os.environ, LD_PRELOAD=str(runtime/'capture.so'))
    run(['timeout','-k','3s','30s',build/'standalone/libntr'],cwd=runtime,env=env,timeout=40)
    sub = pixels(runtime/'libntr-frame-60.ppm')
    main_frame = pixels(runtime/'libntr-frame-120.ppm')
    if sub != bytes((0,0,255))*(512*384): raise ValueError('Secondary engine did not fill visible LCD blue')
    center = (192*512+256)*3
    if main_frame[center:center+3] != bytes((248,248,248)):
        raise ValueError('Main engine geometry did not return to visible LCD')
    if main_frame[:3] != bytes(3): raise ValueError('Main-engine background differs')
    report = dict(result='passed',libntr_pin=PIN,dimensions=[512,384],
        original_hgss_routing=True,secondary_blue=True,main_geometry_returned=True,
        process_exit=0,scope='private_routing_demo_not_gameplay')
    (work/'result.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps(report,indent=2))

if __name__ == '__main__': main()
