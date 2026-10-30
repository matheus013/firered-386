"""Compare original scroll task output to independent text-map decoding."""
import json
import struct
from verify_hgss_renderer import pixels
from verify_scene_backgrounds import expected_image


def slide_image(palette,assets,member,base,tick):
    sub=base>=80
    opening=base in (0,80)
    delta=192 if sub else -192
    x=( -delta if opening else 0 )+int(delta*tick/10)
    image=bytearray(expected_image(palette,assets[54],assets[member],x,0))
    edge=int(255*tick/10)
    # PM_LCD_TOP is 0; the original helper routes screen 0 to the sub engine.
    window_sub=not sub
    left,right=(0,edge) if window_sub and opening else (255-edge,255) if opening else (edge,255) if window_sub else (0,255-edge)
    # The full-width helper adds WIN1 at x=0; it remains set during closing.
    for column in range(256):
        visible=(left<=column<right) or (opening and tick==10) or (not opening and column==0)
        if not visible:
            for row in range(384):
                offset=(row*512+column*2)*3
                image[offset:offset+6]=bytes(6)
    return bytes(image)


def verify(args,assets,runtime,stdout):
    lifecycle='SCROLL lifecycle cycles=512 layers=8 queues=2 continuous_ticks=6144 passed'
    if lifecycle not in stdout:
        raise ValueError('Missing scroll lifecycle evidence')
    for frame in (30,70,110,150):
        if f'SCROLL completed frame={frame}' not in stdout:
            raise ValueError('Missing original scroll task completion')
    palette=bytearray(assets[53])
    offset=24+struct.unpack_from('<I',palette,36)[0]
    palette[offset:offset+2]=bytes(2)
    phases=[]
    for base,member,start,change in ((0,58,(0,0),(-48,32)),(40,58,(-48,32),(48,-32)),
                                    (80,57,(0,0),(37,-24)),(120,57,(37,-24),(-37,24))):
        images=[expected_image(palette,assets[54],assets[member],
                 start[0]+int(change[0]*tick/10),start[1]+int(change[1]*tick/10)) for tick in range(11)]
        if args.scene4_slide:
            images=[slide_image(palette,assets,member,base,tick) for tick in range(11)]
        matches=[]
        for frame in range(base+2,base+20,2):
            actual=pixels(runtime/f'libntr-frame-{frame}.ppm')
            matches.append(next((tick for tick,image in enumerate(images) if actual==image),-1))
        endpoint=pixels(runtime/f'libntr-frame-{base+20}.ppm')==images[10]
        phases.append({'start':base,'engine':'main' if base<80 else 'sub',
                       'queue':'vblank' if base<80 or args.scene4_slide else 'main_with_scheduled_commit',
                       'matched_ticks':matches,'endpoint_exact':endpoint,
                       'passed':endpoint and -1 not in matches and matches==sorted(matches)
                                and len({tick for tick in matches if 0<tick<10})>=2})
    report={'passed':all(p['passed'] for p in phases),'phases':phases,'platform':'linux-x86_64',
            'scope':'original_finite_BG1_scroll_tasks_and_commits_not_full_intro',
            'lifecycle':{'cycles':512,'layers':8,'queues':2,'continuous_ticks':6144,
                         'checks':['immediate_offset','continuous_scroll','cancel_twice','stable_after_cancel','finite_restart']},
            'limits':'Lifecycle uses state assertions; visual checks cover finite BG1 only. No affine BGs or Windows run; capture timing may differ.'}
    if args.scene4_slide:
        report['scope']='scene4_window_and_scroll_helpers_together_without_sprites_or_state_machine'
    (args.output/'result.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps(report))
    if not report['passed']: raise ValueError('Original BG scroll differs from CPU tile-map oracle')
