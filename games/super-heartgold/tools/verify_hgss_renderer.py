"""Linux/WSLg integration gate: build and capture original HGSS display routing.
Requires GCC/G++, Meson, Ninja, NASM, Flex, Bison, SDL2, ENet and OpenGL.
Uses a new private workspace; never modifies the supplied source checkouts.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys
from prepare_hgss_display_demo import PIN
from prepare_spl_drawing import prepare as prepare_spl_drawing


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
    parser.add_argument('--spl-source', type=Path, help='Private original spl_draw_ptcl.c for plane rendering')
    parser.add_argument('--spl-probe', type=Path, help='Existing module probe with compatible HGSS headers')
    parser.add_argument('--spl-resource', type=Path, help='Verified private intro SPA; render emitter zero')
    parser.add_argument('--libntrsystem', type=Path, help='NitroSystem headers required for SPL uploads')
    args = parser.parse_args()
    if bool(args.spl_source) != bool(args.spl_probe):
        parser.error('--spl-source and --spl-probe must be supplied together')
    if args.spl_resource and not args.spl_probe:
        parser.error('--spl-resource requires --spl-source and --spl-probe')
    if args.spl_resource and not args.libntrsystem:
        parser.error('--spl-resource requires --libntrsystem')
    if args.spl_resource and hashlib.sha256(args.spl_resource.read_bytes()).hexdigest() != '748eded76a142bcf2247df73ed54bfec6e24d15d48d2c1c9b825cf1c061c916e':
        parser.error('Intro SPA hash differs')
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
    if args.spl_source:
        prepare_spl_drawing(args.spl_source, source/'standalone/spl_draw_ptcl.c')
        demo_path = source/'standalone/main.c'
        demo = demo_path.read_text()
        begin = demo.index('    G3_Begin(GX_BEGIN_QUADS);')
        end = demo.index('    G3_End();', begin) + len('    G3_End();')
        demo = demo[:begin] + '    drawXYPlane(FX32_ONE, FX32_ONE, 0, 0);' + demo[end:]
        # A single plane must not disappear as the original cube rotates away.
        demo = demo.replace('GX_CULL_BACK', 'GX_CULL_NONE')
        for axis in 'XYZ':
            demo = demo.replace(f'      G3_Rot{axis}(s, c);', '')
        demo = 'void drawXYPlane(int s, int t, int x, int y);\n' + demo
        demo_path.write_text(demo)
        if args.spl_resource:
            sources = sorted((args.spl_probe/'spl-native').glob('spl_*.c'))
            if len(sources) != 9: raise ValueError('Expected nine adapted SPL units')
            for unit in sources:
                if unit.name != 'spl_draw_ptcl.c':
                    shutil.copyfile(unit, source/'standalone'/unit.name)
            shutil.copyfile(root/'tools/probe_spl_render.c', demo_path)
    meson = source/'standalone/meson.build'
    text = meson.read_text()
    old = "sources: files('main.c')"
    if text.count(old) != 1: raise ValueError('Standalone build contract differs')
    if args.spl_source:
        names = ['main.c', 'hgss_display.c', 'spl_draw_ptcl.c']
        if args.spl_resource:
            names = ['main.c', 'hgss_display.c'] + [p.name for p in sources]
        text = text.replace(old, 'sources: files('+', '.join(repr(n) for n in names)+')')
        includes = [args.spl_probe.resolve(), args.spl_probe.resolve()/'hgss-include',
                    args.spl_probe.resolve()/'hgss-include/library']
        if args.spl_resource: includes.append(args.libntrsystem.resolve()/'include')
        if any("'" in str(p) for p in includes): raise ValueError('Unsupported quote in include path')
        flags = ['-ffunction-sections', '-fdata-sections'] + ['-I'+str(p) for p in includes]
        text = text.replace('dependencies: [ nitrosdk_dep ],',
                            'dependencies: [ nitrosdk_dep ],\n    c_args: '+repr(flags)+',')
        text = text.replace('libntr_link_args = []', "libntr_link_args = ['-Wl,--gc-sections']")
    else:
        text = text.replace(old,"sources: files('main.c', 'hgss_display.c')")
    meson.write_text(text)
    run(['meson','setup',build,source,'-Dbuild_target=linux','-Dstandalone=true',
         '-Dcard_game_code=IPGE','-Dcard_maker_code=01'])
    run(['meson','compile','-C',build,'-j4'])
    runtime.mkdir()
    if args.spl_resource:
        shutil.copyfile(args.spl_resource, runtime/'intro-particles.spa')
    run(['gcc','-shared','-fPIC',root/'tools/probe_libntr_frames.c','-I/usr/include/SDL2',
         '-lSDL2','-ldl','-o',runtime/'capture.so'])
    (runtime/'sim_config.ini').write_text('[General]\nScreenLayout=single\nSwapScreens=true\nWindowWidth=512\nWindowHeight=384\n')
    env = dict(os.environ, LD_PRELOAD=str(runtime/'capture.so'))
    for key in ('HG_SPL_RESOURCE', 'HG_SPL_GROUP', 'HG_SPL_DISABLE_DRAW'):
        env.pop(key, None)
    if args.spl_resource: env['SDL_AUDIODRIVER'] = 'dummy'
    command = ['timeout','-k','3s','30s',str(build/'standalone/libntr')]
    result = subprocess.run(command,cwd=runtime,env=env,timeout=40,capture_output=True,text=True)
    (runtime/'run.log').write_text(result.stdout+result.stderr)
    result.check_returncode()
    sub = pixels(runtime/'libntr-frame-60.ppm')
    main_frame = pixels(runtime/'libntr-frame-120.ppm')
    if sub != bytes((0,0,255))*(512*384): raise ValueError('Secondary engine did not fill visible LCD blue')
    center = (192*512+256)*3
    if not args.spl_resource and main_frame[center:center+3] != bytes((248,248,248)):
        raise ValueError('Main engine geometry did not return to visible LCD')
    if main_frame[:3] != bytes(3): raise ValueError('Main-engine background differs')
    report = dict(result='passed',libntr_pin=PIN,dimensions=[512,384],
        original_hgss_routing=True,secondary_blue=True,main_geometry_returned=True,
        process_exit=0,scope='private_routing_demo_not_gameplay')
    if args.spl_source:
        report.update(spl_xy_plane_rendered=True, particle_emitter_rendered=False,
                      scope='adapted_SPL_plane_with_original_HGSS_routing_not_gameplay')
    if args.spl_resource:
        nonblack = sum(main_frame[i:i+3] != bytes(3) for i in range(0,len(main_frame),3))
        if nonblack < 16: raise ValueError('Emitter produced no visible particle geometry')
        control = work/'control'
        control.mkdir()
        for name in ('sim_config.ini', 'intro-particles.spa'):
            shutil.copyfile(runtime/name, control/name)
        disabled_env = dict(env, HG_SPL_DISABLE_DRAW='1')
        result = subprocess.run(command,cwd=control,env=disabled_env,timeout=40,capture_output=True,text=True)
        (control/'run.log').write_text(result.stdout+result.stderr)
        result.check_returncode()
        if pixels(control/'libntr-frame-120.ppm') != bytes(len(main_frame)):
            raise ValueError('Disabled SPL draw control did not remain black')
        report.update(spl_xy_plane_rendered=False, particle_emitter_rendered=True,
                      emitter_resource=0, visible_pixels=nonblack, disabled_draw_control_black=True,
                      audio_tested=False, audio_driver='dummy',
                      scope='one_intro_SPL_emitter_real_SDK_upload_and_render_not_gameplay')
    (work/'result.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps(report,indent=2))

if __name__ == '__main__': main()
