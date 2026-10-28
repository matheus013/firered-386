"""Render nine intro emitters and three original groups in a validated SDK workspace."""
import argparse
import hashlib
import json
import os
import re
import shutil
import subprocess
from pathlib import Path
from verify_hgss_renderer import pixels


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--workspace', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--scene-camera', action='store_true', help='Test three groups with original projection parameters and draw/update order')
    parser.add_argument('--sequence', action='store_true', help='Run all three groups to natural completion in one process')
    args = parser.parse_args()
    work, output = args.workspace.resolve(), args.output.resolve()
    previous = json.loads((work/'result.json').read_text())
    if not previous.get('particle_emitter_rendered') or previous.get('result') != 'passed':
        raise ValueError('Workspace must have passed the single-emitter integration test')
    resource = work/'runtime/intro-particles.spa'
    if hashlib.sha256(resource.read_bytes()).hexdigest() != '748eded76a142bcf2247df73ed54bfec6e24d15d48d2c1c9b825cf1c061c916e':
        raise ValueError('SPA hash differs')
    output.mkdir(parents=True, exist_ok=False)
    harness = Path(__file__).with_name('probe_spl_render.c')
    shutil.copyfile(harness, work/'source/standalone/main.c')
    subprocess.run(['meson','compile','-C',str(work/'build'),'-j4'],check=True)
    results = []
    cases = [('resource', i) for i in range(9)] + [('group', i) for i in range(3)]
    if args.scene_camera: cases = [('group', i) for i in range(3)]
    capture = work/'runtime/capture.so'
    if args.sequence:
        cases = [('sequence', 0)]
        capture = output/'capture.so'
        subprocess.run(['gcc','-shared','-fPIC','-DHG_CAPTURE_LAST_FRAME=600',
                        str(Path(__file__).with_name('probe_libntr_frames.c')),
                        '-I/usr/include/SDL2','-lSDL2','-ldl','-o',str(capture)],check=True)
    for kind, index in cases:
        run_dir = output/f'{kind}-{index}'
        run_dir.mkdir()
        shutil.copyfile(resource, run_dir/'intro-particles.spa')
        shutil.copyfile(work/'runtime/sim_config.ini', run_dir/'sim_config.ini')
        env = dict(os.environ, SDL_AUDIODRIVER='dummy',
                   LD_PRELOAD=str(capture))
        for key in ('HG_SPL_DISABLE_DRAW', 'HG_SPL_RESOURCE', 'HG_SPL_GROUP', 'HG_SPL_SCENE_CAMERA', 'HG_SPL_SEQUENCE'):
            env.pop(key, None)
        env[{'resource':'HG_SPL_RESOURCE', 'group':'HG_SPL_GROUP', 'sequence':'HG_SPL_SEQUENCE'}[kind]] = str(index)
        if args.scene_camera: env['HG_SPL_SCENE_CAMERA'] = '1'
        result = subprocess.run(['timeout','-k','3s','30s',str(work/'build/standalone/libntr')],
                                cwd=run_dir,env=env,capture_output=True,text=True,timeout=40)
        (run_dir/'run.log').write_text(result.stdout+result.stderr)
        entry = {kind: index, 'exit_code': result.returncode}
        if result.returncode == 0:
            frame = pixels(run_dir/'libntr-frame-120.ppm')
            entry['visible_pixels'] = sum(frame[i:i+3] != bytes(3) for i in range(0,len(frame),3))
            if not args.sequence:
                entry['secondary_blue'] = pixels(run_dir/'libntr-frame-60.ppm') == bytes((0,0,255))*(512*384)
        results.append(entry)
        if args.sequence and result.returncode == 0:
            events = re.findall(r'SPL_SEQUENCE group=(\d+) end_frame=(\d+) peak=(\d+) particles_free=200 emitters_free=20', result.stdout)
            entry['completions'] = [{'group':int(g), 'end_frame':int(f), 'peak':int(p)} for g,f,p in events]
            entry['natural_completion'] = [int(g) for g,_,_ in events] == [0,1,2]
            entry['final_black'] = pixels(run_dir/'libntr-frame-600.ppm') == bytes(512*384*3)
            entry['visible_samples'] = sum(any(pixels(path)) for path in run_dir.glob('libntr-frame-*.ppm'))
            previous_frame = 0
            for event in entry['completions']:
                event['visible_samples'] = sum(any(pixels(path)) for path in run_dir.glob('libntr-frame-*.ppm')
                    if previous_frame < int(path.stem.rsplit('-',1)[1]) <= event['end_frame'])
                previous_frame = event['end_frame']
            entry['all_groups_visible'] = len(events) == 3 and all(e['visible_samples'] > 0 for e in entry['completions'])
        print(json.dumps(entry), flush=True)
    report = {'scope':'nine individual intro emitters and three original groups, diagnostic camera and timing, Linux only',
              'harness_sha256': hashlib.sha256(harness.read_bytes()).hexdigest(),
              'results':results, 'passed': all(x.get('visible_pixels',0) >= 16 and x.get('secondary_blue') for x in results),
              'audio_tested':False, 'gameplay_tested':False}
    if args.scene_camera:
        report['scope'] = 'three original groups, scene camera parameters and draw/update order; diagnostic capture timing, Linux only'
        report['original_camera_parameters'] = {'eye':[0,0,4], 'target':[0,0,0], 'projection':'orthographic', 'angle_index':8192, 'near':1, 'far':900, 'aspect_fx32':5461}
    if args.sequence:
        report['scope'] = 'three particle groups to natural completion, shared pools, scene camera parameters; no original scene state machine or sprites'
        report['passed'] = all(x.get('natural_completion') and x.get('final_black') and x.get('all_groups_visible') for x in results)
    (output/'result.json').write_text(json.dumps(report,indent=2)+'\n')
    if not report['passed']: raise SystemExit('Some emitters failed or were not visible at the sampled frame')


if __name__ == '__main__': main()
