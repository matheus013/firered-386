"""Evaluate original shutter fades 9/8 using real task queues and native graphics."""
import json
import os
from pathlib import Path
import shutil
import subprocess
import struct
from prepare_hgss_message_headers import read_banks
from prepare_hgss_opening_headers import decompress_lz10
from verify_hgss_renderer import pixels
from verify_scene_backgrounds import expected_image


def run(args, tools, raw):
    capture = args.output/'capture.so'
    subprocess.run(['gcc','-shared','-fPIC','-DHG_CAPTURE_LAST_FRAME=160','-DHG_CAPTURE_EARLY_FRAMES=1','-DHG_CAPTURE_FADE_FRAMES=1',
                    str(tools/'probe_libntr_frames.c'),'-I/usr/include/SDL2','-lSDL2','-ldl','-o',str(capture)],check=True)
    banks = read_banks(raw)
    members = [53,54,57,58,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96]
    assets = {i:banks[i] if i in (53,81,85,89,93) else decompress_lz10(banks[i]) for i in members}
    runtime = args.output/'runtime'; runtime.mkdir()
    for i,data in assets.items(): (runtime/f'{i}.bin').write_bytes(data)
    shutil.copyfile(args.hgss/'files/data/clact_default.NANR',runtime/'default.NANR')
    (runtime/'sim_config.ini').write_text('[General]\nScreenLayout=single\nSwapScreens=true\nWindowWidth=512\nWindowHeight=384\n')
    env=dict(os.environ,LD_PRELOAD=str(capture),SDL_AUDIODRIVER='dummy',HG_SPRITE_CASE='0',HG_SPRITE_DISABLE_DRAW='1',HG_SPRITE_COMPOSE='1')
    if args.fade_sprites: env.pop('HG_SPRITE_DISABLE_DRAW', None)
    for key in ('HG_SPRITE_PLAYBACK','HG_SPRITE_INITIAL_TRANSFORM'): env.pop(key,None)
    result=subprocess.run(['timeout','-k','3s','30s',str(args.workspace/'build/standalone/libntr')],cwd=runtime,env=env,capture_output=True,text=True,timeout=40)
    (runtime/'run.log').write_text(result.stdout+result.stderr); result.check_returncode()
    for frame in (30,70,110,150):
        if f'FADE completed frame={frame}' not in result.stdout: raise ValueError('Missing completion evidence')
    palette = bytearray(assets[53])
    offset = 24 + struct.unpack_from('<I', palette, 36)[0]
    palette[offset:offset+2] = bytes(2)  # Original SetBothBackdropColors(black).
    checks=[]
    open_frames={58:pixels(runtime/'libntr-frame-20.ppm'),57:pixels(runtime/'libntr-frame-100.ppm')}
    if args.fade_sprites:
        for member,image in open_frames.items():
            if image==expected_image(palette,assets[54],assets[member]):
                raise ValueError('Sprite absent from open composite')
    for frame,member in ((20,58),(60,None),(100,57),(140,None)):
        actual=pixels(runtime/f'libntr-frame-{frame}.ppm')
        expected=bytes(len(actual)) if member is None else (open_frames[member] if args.fade_sprites else expected_image(palette,assets[54],assets[member]))
        checks.append({'frame':frame,'expected':'black' if member is None else str(member),'exact':actual==expected})
    phases=[]
    for base,member,opening in ((0,58,True),(40,58,False),(80,57,True),(120,57,False)):
        background=open_frames[member] if args.fade_sprites else expected_image(palette,assets[54],assets[member])
        full=sum(any(background[i:i+3]) for i in range(0,len(background),3))
        counts=[]
        invalid_rows=0
        for offset in range(2,20,2):
            actual=pixels(runtime/f'libntr-frame-{base+offset}.ppm')
            for row in range(384):
                start=row*512*3; end=start+512*3
                if actual[start:end] not in (bytes(512*3),background[start:end]):
                    invalid_rows += 1
            counts.append(sum(any(actual[i:i+3]) for i in range(0,len(actual),3)))
        partial=len({n for n in counts if 0<n<full})
        monotonic=counts==sorted(counts,reverse=not opening)
        phases.append({'start':base,'opening':opening,'nonblack_pixels':counts,
                       'partial_states':partial,'monotonic':monotonic,'invalid_rows':invalid_rows})
    report={'passed':all(c['exact'] for c in checks) and all(p['partial_states']>=2 and p['monotonic'] and p['invalid_rows']==0 for p in phases),'phases':phases,'checks':checks,'scope':'original_9_8_fades_with_scanline_progression_and_task_completion','platform':'linux-x86_64'}
    report['sprites_enabled']=args.fade_sprites
    (args.output/'result.json').write_text(json.dumps(report,indent=2)+'\n'); print(json.dumps(report))
    if not report['passed']: raise ValueError('Fade endpoints mismatch')
