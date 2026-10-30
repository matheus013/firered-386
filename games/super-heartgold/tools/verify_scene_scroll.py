"""Compare original scroll task output to independent text-map decoding."""
import json
import struct
from verify_hgss_renderer import pixels
from verify_scene_backgrounds import expected_image


def verify(args,assets,runtime,stdout):
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
        matches=[]
        for frame in range(base+2,base+20,2):
            actual=pixels(runtime/f'libntr-frame-{frame}.ppm')
            matches.append(next((tick for tick,image in enumerate(images) if actual==image),-1))
        endpoint=pixels(runtime/f'libntr-frame-{base+20}.ppm')==images[10]
        phases.append({'start':base,'engine':'main' if base<80 else 'sub',
                       'queue':'vblank' if base<80 else 'main_with_scheduled_commit',
                       'matched_ticks':matches,'endpoint_exact':endpoint,
                       'passed':endpoint and -1 not in matches and matches==sorted(matches)
                                and len({tick for tick in matches if 0<tick<10})>=2})
    report={'passed':all(p['passed'] for p in phases),'phases':phases,'platform':'linux-x86_64',
            'scope':'original_finite_BG1_scroll_tasks_and_commits_not_full_intro',
            'limits':'Timing may differ within capture phase; no affine BGs, infinite scroll or Windows run'}
    (args.output/'result.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps(report))
    if not report['passed']: raise ValueError('Original BG scroll differs from CPU tile-map oracle')
