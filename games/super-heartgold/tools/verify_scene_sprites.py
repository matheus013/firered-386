"""Reuse a private BG diagnostic build to test SDK cell/OAM rendering of intro sprites."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess

from prepare_hgss_message_headers import read_banks
from prepare_hgss_opening_headers import ARCHIVE_SHA256, decompress_lz10
from verify_hgss_renderer import pixels
from verify_scene_backgrounds import expected_image


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--playback', action='store_true', help='Tick original sequences and apply their translations')
    parser.add_argument('--compose', action='store_true', help='Render static cells over the scene BG1 maps')
    parser.add_argument('--sprite-reference', type=Path, help='Passed static sprite test directory for composition comparison')
    for name in ('workspace', 'stack', 'archive', 'output'):
        parser.add_argument('--' + name, type=Path, required=True)
    parser.add_argument('--initial-transform', action='store_true')
    parser.add_argument('--manager', action='store_true')
    parser.add_argument('--hgss', type=Path)
    parser.add_argument('--probe', type=Path)
    args = parser.parse_args()
    if args.manager and (args.playback or args.compose or not args.hgss or not args.probe or not args.sprite_reference):
        parser.error('--manager requires --hgss, --probe, --sprite-reference and static mode')
    if args.initial_transform and (args.manager or args.playback or args.compose):
        parser.error('--initial-transform requires standalone static reference mode')
    if args.manager:
        reference = json.loads((args.sprite_reference/'result.json').read_text())
        if not reference.get('passed') or not reference.get('initial_translation_applied'):
            raise ValueError('Manager requires a passed reference with initial translation applied')
    if args.compose and (args.playback or not args.sprite_reference):
        parser.error('--compose requires --sprite-reference and excludes --playback')
    if args.compose:
        reference = json.loads((args.sprite_reference/'result.json').read_text())
        if not reference.get('passed') or reference.get('scope') != 'static_initial_cells_not_full_sprite_manager_or_animation_playback':
            raise ValueError('Require passed static sprite reference')
    if not json.loads((args.workspace / 'result.json').read_text()).get('passed'):
        raise ValueError('Require a validated background workspace')
    raw = args.archive.read_bytes()
    if hashlib.sha256(raw).hexdigest() != ARCHIVE_SHA256:
        raise ValueError('Opening archive hash differs')
    args.output.mkdir(parents=True, exist_ok=False)
    tools = Path(__file__).resolve().parent
    standalone = args.workspace / 'source/standalone'
    nns = args.stack / 'subprojects/libntrsystem'
    shutil.copyfile(tools / 'probe_scene_sprites.c', standalone / 'main.c')
    names = ['main.c', 'hgss_display.c']
    units = [f'load/g2d_{name}_load.c' for name in ('NCG','NCL','NOB','NAN','NSC')]
    units += ['load/g2d_Load.c', 'g2d_Animation.c', 'g2d_CellAnimation.c',
              'g2d_SRTControl.c', 'g2d_CellTransferManager.c']
    if args.manager:
        units += ['g2d_Image.c','g2d_Renderer.c','g2d_RendererCore.c',
                  'g2d_MultiCellAnimation.c','g2d_Node.c','g2d_OamSoftwareSpriteDraw.c',
                  'load/g2d_NMC_load.c', 'internal/g2di_Mtx32.c', 'g2d_PaletteTable.c']
    for unit in units:
        path = nns / 'libraries/g2d/src' / unit
        shutil.copyfile(path, standalone / path.name)
        names.append(path.name)
    flags = ['-ffunction-sections', '-fdata-sections', '-DNNS_FINALROM',
             '-I' + str(nns / 'include'), '-I' + str(nns / 'libraries/g2d/include')]
    if args.manager:
        from prepare_hgss_sprite_storage import prepare
        from prepare_hgss_graphics import prepare_graphics
        prepare(args.hgss/'include/sprite.h', standalone/'sprite.h')
        prepare_graphics(args.hgss/'src/gf_g2d_renderer.c', standalone/'gf_g2d_renderer.c')
        shutil.copyfile(args.hgss/'src/sprite.c', standalone/'sprite.c')
        shutil.copyfile(tools/'probe_scene_sprite_manager.h', standalone/'probe_scene_sprite_manager.h')
        names += ['sprite.c','gf_g2d_renderer.c']
        flags += ['-DHG_SPRITE_MANAGER', '-DSOULSILVER', '-DENGLISH', '-DSDK_VERSION_MAJOR=4',
                  '-include', str(args.probe/'hgss_backup.h'), '-I'+str(standalone),
                  '-I'+str(args.probe), '-I'+str(args.probe/'hgss-include'),
                  '-I'+str(args.probe/'hgss-include/library')]
    (standalone / 'meson.build').write_text(
        "libntr_exe = executable('libntr', sources: files(" + ', '.join(map(repr, names)) + "),\n"
        "dependencies: [nitrosdk_dep], c_args: " + repr(flags) + ",\n"
        "link_args: ['-Wl,--gc-sections'], native: build_target != 'arm')\n")
    result = subprocess.run(['meson', 'compile', '-C', str(args.workspace / 'build'), '-j6'],
                            capture_output=True, text=True, timeout=600)
    (args.output / 'build.log').write_text(result.stdout + result.stderr)
    result.check_returncode()
    capture = args.workspace / 'capture.so'
    if args.playback:
        capture = args.output / 'capture.so'
        subprocess.run(['gcc', '-shared', '-fPIC', '-DHG_CAPTURE_LAST_FRAME=160', '-DHG_CAPTURE_EARLY_FRAMES=1',
                        str(tools/'probe_libntr_frames.c'), '-I/usr/include/SDL2',
                        '-lSDL2', '-ldl', '-o', str(capture)], check=True)
    banks = read_banks(raw)
    members = [81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96]
    if args.compose: members += [53,54,57,58]
    assets = {i: banks[i] if i in (53,81,85,89,93) else decompress_lz10(banks[i]) for i in members}
    cases = []
    for case in range(6):
        images = {}
        for disabled in (False, True):
            runtime = args.output / f'case-{case}-' / ('disabled' if disabled else 'enabled')
            runtime.mkdir(parents=True)
            if args.manager:
                shutil.copyfile(args.hgss/'files/data/clact_default.NANR', runtime/'default.NANR')
            for member, data in assets.items():
                (runtime / f'{member}.bin').write_bytes(data)
            (runtime / 'sim_config.ini').write_text('[General]\nScreenLayout=single\nSwapScreens=true\nWindowWidth=512\nWindowHeight=384\n')
            env = dict(os.environ, LD_PRELOAD=str(capture),
                       SDL_AUDIODRIVER='dummy', HG_SPRITE_CASE=str(case))
            env.pop('HG_SPRITE_DISABLE_DRAW', None)
            env.pop('HG_SPRITE_PLAYBACK', None)
            env.pop('HG_SPRITE_COMPOSE', None)
            env.pop('HG_SPRITE_INITIAL_TRANSFORM', None)
            if args.initial_transform:
                env['HG_SPRITE_INITIAL_TRANSFORM'] = '1'
            if args.compose:
                env['HG_SPRITE_COMPOSE'] = '1'
            if args.playback:
                env['HG_SPRITE_PLAYBACK'] = '1'
            if disabled:
                env['HG_SPRITE_DISABLE_DRAW'] = '1'
            result = subprocess.run(['timeout', '-k', '3s', '30s', str(args.workspace / 'build/standalone/libntr')],
                                    cwd=runtime, env=env, capture_output=True, text=True, timeout=40)
            (runtime / 'run.log').write_text(result.stdout + result.stderr)
            result.check_returncode()
            captures = [2,4,6,8,10,*range(20,161,20)] if args.playback else (60,120)
            frames = [pixels(runtime / f'libntr-frame-{f}.ppm') for f in captures]
            if not args.playback and not args.compose and frames[0] != frames[1]:
                raise ValueError('Same static cell differs between main and sub engines')
            if args.manager:
                reference_dir = args.sprite_reference / f'case-{case}-' / ('disabled' if disabled else 'enabled')
                for f, actual in zip(captures, frames):
                    if actual != pixels(reference_dir / f'libntr-frame-{f}.ppm'):
                        raise ValueError(f'Manager/reference mismatch: case {case}, disabled {disabled}, frame {f}')
            images[disabled] = frames
        if args.compose:
            reference_frame = pixels(args.sprite_reference/f'case-{case}-/enabled/libntr-frame-120.ppm')
            for index, member in enumerate((57,58)):
                background = expected_image(assets[53], assets[54], assets[member])
                if images[True][index] != background:
                    raise ValueError('Background-only control differs from tile-map oracle')
                foreground = images[False][index]
                colored = changed = 0
                for offset in range(0,len(background),3):
                    sprite = reference_frame[offset:offset+3]
                    actual = foreground[offset:offset+3]
                    if sprite != bytes(3):
                        colored += 1
                        if actual != sprite:
                            raise ValueError('Colored sprite pixel was lost or occluded')
                    elif actual not in (bytes(3), background[offset:offset+3]):
                        raise ValueError('Unexpected pixel outside colored sprite footprint')
                    changed += actual != background[offset:offset+3]
                if changed < 16 or colored < 16:
                    raise ValueError('No visible sprite/background composition')
                cases.append({'case':case, 'engine':'sub' if index == 0 else 'main',
                              'colored_sprite_pixels_matched':colored, 'changed_pixels':changed,
                              'background_control_exact':True})
            continue
        if any(any(frame) for frame in images[True]):
            raise ValueError('Disabled OAM conversion control is not black')
        visible = max(sum(frame[i:i+3] != bytes(3) for i in range(0,len(frame),3)) for frame in images[False])
        if visible < 16 and not (args.initial_transform or args.manager):
            raise ValueError(f'No visible sprite for case {case}')
        distinct = len(set(images[False]))
        if args.playback and distinct < 2:
            raise ValueError(f'Animation did not visibly change for case {case}')
        cases.append({'case': case, 'visible_pixels': visible, 'engines_equal': None if args.playback else True,
                      'distinct_captures': distinct, 'disabled_black': True})
    report = {'passed': True, 'cases': cases, 'dimensions': [512,384],
              'platform': 'linux-x86_64', 'scope': 'static_initial_cells_not_full_sprite_manager_or_animation_playback',
              'harness_sha256': hashlib.sha256((tools / 'probe_scene_sprites.c').read_bytes()).hexdigest()}
    if args.playback:
        report['scope'] = '160_frame_SDK_cell_playback_with_translation_not_full_sprite_manager'
    if args.compose:
        report['scope'] = 'static_BG1_and_SDK_cell_composition_not_full_scene'
    if args.initial_transform:
        report['initial_translation_applied'] = True
    if args.manager:
        report['scope'] = 'original_sprite_manager_static_rendering_matches_cell_reference'
    (args.output / 'result.json').write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps(report))


if __name__ == '__main__':
    main()
