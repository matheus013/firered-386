"""Send button sequences to the unmodified ROM; retain a resumable emulator state.

Example on Windows: python games/super-fire-red/scripts/play_route.py 'wait:300 start:1 wait:120'
No bus writes, ROM patching, injected scripts, warps or supplied game resources.
"""
from pathlib import Path
import hashlib,json,subprocess,sys,time
root=Path(__file__).resolve().parents[3]
out=root/'games/super-fire-red/output/tests/natural-route';out.mkdir(parents=True,exist_ok=True)
rom=root/'games/super-fire-red/output/firered-386-dev.gba'
sha=hashlib.sha256(rom.read_bytes()).hexdigest()
target=out/'rom.gba'
if not target.exists(): target.write_bytes(rom.read_bytes())
assert hashlib.sha256(target.read_bytes()).hexdigest()==sha
def linux(path):
 p=str(path.resolve()).replace('\\','/');return '/mnt/'+p[0].lower()+p[2:]
assert sha=='d880845011b29532abf333cc082da02b20eee97cf9f7767fe5fd61acf3b55cf8','Refresh read-only symbol observations for a new ROM'
keys={'a':1,'b':2,'select':4,'start':8,'right':16,'left':32,'up':64,'down':128,'r':256,'l':512,'wait':0,'battle':1024}
walk='--walk' in sys.argv
sequence=[]
for token in ' '.join(a for a in sys.argv[1:] if a!='--walk').split():
 key,n=token.split(':');sequence.append((sum(keys[k] for k in key.lower().split('+')),int(n)))
assert sequence
exe='/tmp/firered-route-player'
command=['wsl','-d','Ubuntu','-u','root','--cd','/root','--']
subprocess.run(command+['gcc','-O2','-I'+linux(root/'games/super-fire-red/scripts'),linux(root/'games/super-fire-red/scripts/route_player.c'),'-o',exe,'-lmgba','-lpng'],check=True,capture_output=True)
entry={'time':time.time(),'rom_sha256':sha,'inputs':sequence}
with (out/'inputs.jsonl').open('a',encoding='utf-8') as f:f.write(json.dumps(entry)+'\n')
run=subprocess.run(command+[exe,linux(target),linux(out)]+(['walk'] if walk else []),input=''.join(f'{k} {n}\n' for k,n in sequence),text=True,capture_output=True)
print(run.stdout,run.stderr);run.check_returncode()
with (out/'batches.jsonl').open('a',encoding='utf-8') as f:
 f.write(json.dumps({'time':time.time(),'inputs':sequence,'result':run.stdout,'screen_sha256':hashlib.sha256((out/'screen.png').read_bytes()).hexdigest()})+'\n')
