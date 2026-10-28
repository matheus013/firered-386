"""Render nine intro emitters and three original groups in a validated SDK workspace."""
import argparse
import hashlib
import json
import os
import shutil
import subprocess
from pathlib import Path
from verify_hgss_renderer import pixels


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--workspace', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--scene-camera', action='store_true', help='Test three groups with original projection parameters and draw/update order')
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
    for kind, index in cases:
        run_dir = output/f'{kind}-{index}'
        run_dir.mkdir()
        shutil.copyfile(resource, run_dir/'intro-particles.spa')
        shutil.copyfile(work/'runtime/sim_config.ini', run_dir/'sim_config.ini')
        env = dict(os.environ, SDL_AUDIODRIVER='dummy',
                   LD_PRELOAD=str(work/'runtime/capture.so'))
        for key in ('HG_SPL_DISABLE_DRAW', 'HG_SPL_RESOURCE', 'HG_SPL_GROUP', 'HG_SPL_SCENE_CAMERA'):
            env.pop(key, None)
        env['HG_SPL_RESOURCE' if kind == 'resource' else 'HG_SPL_GROUP'] = str(index)
        if args.scene_camera: env['HG_SPL_SCENE_CAMERA'] = '1'
        result = subprocess.run(['timeout','-k','3s','30s',str(work/'build/standalone/libntr')],
                                cwd=run_dir,env=env,capture_output=True,text=True,timeout=40)
        (run_dir/'run.log').write_text(result.stdout+result.stderr)
        entry = {kind: index, 'exit_code': result.returncode}
        if result.returncode == 0:
            frame = pixels(run_dir/'libntr-frame-120.ppm')
            entry['visible_pixels'] = sum(frame[i:i+3] != bytes(3) for i in range(0,len(frame),3))
            entry['secondary_blue'] = pixels(run_dir/'libntr-frame-60.ppm') == bytes((0,0,255))*(512*384)
        results.append(entry)
        print(json.dumps(entry), flush=True)
    report = {'scope':'nine individual intro emitters and three original groups, diagnostic camera and timing, Linux only',
              'harness_sha256': hashlib.sha256(harness.read_bytes()).hexdigest(),
              'results':results, 'passed': all(x.get('visible_pixels',0) >= 16 and x.get('secondary_blue') for x in results),
              'audio_tested':False, 'gameplay_tested':False}
    if args.scene_camera:
        report['scope'] = 'three original groups, scene camera parameters and draw/update order; diagnostic capture timing, Linux only'
        report['original_camera_parameters'] = {'eye':[0,0,4], 'target':[0,0,0], 'projection':'orthographic', 'angle_index':8192, 'near':1, 'far':900, 'aspect_fx32':5461}
    (output/'result.json').write_text(json.dumps(report,indent=2)+'\n')
    if not report['passed']: raise SystemExit('Some emitters failed or were not visible at the sampled frame')


if __name__ == '__main__': main()
