"""Test community-inspired edge cases; report failures without hiding other cases."""
from pathlib import Path
import hashlib,json,shutil,subprocess,tempfile
root=Path(__file__).resolve().parents[1];out=root/'output/tests/community';out.mkdir(parents=True,exist_ok=True)
source=Path((root/'output/hack-build-path.txt').read_text().strip())
test=Path(tempfile.mkdtemp(prefix='community-fixture-',dir=Path.home()/'.cache/firered-386'));shutil.copytree(source,test,dirs_exist_ok=True)
(out/'build-path.txt').write_text(str(test)+'\n')
main=(root/'game/src/main.c').read_text();hook=(root/'scripts/community_fixture.c').read_text()
main=main.replace('static void InitMainCallbacks(void)\n{',hook+'\nstatic void InitMainCallbacks(void)\n{').replace('SetMainCallback2(CB2_InitCopyrightScreenAfterBootup);','SetMainCallback2(CommunityBoot);')
(test/'src/main.c').write_text(main)
pokemon=(root/'game/src/pokemon.c').read_text()+'''\nu32 CommunityCountShinies(void) {u32 i, count=0; for(i=0;i<65536;i++) if(IsShinyOtIdPersonality(0,i)) count++; return count;}\n'''
(test/'src/pokemon.c').write_text(pokemon)
with (out/'build.log').open('w') as log:subprocess.run(['make','-j4'],cwd=test,stdout=log,stderr=subprocess.STDOUT,check=True)
symbols={p[2]:p[0] for line in subprocess.check_output(['arm-none-eabi-nm','pokefirered.elf'],cwd=test,text=True).splitlines() if len(p:=line.split())==3}
runner=test/'community-smoke';subprocess.run(['gcc','-O2','-Wall','-Wextra',str(root/'scripts/community_smoke.c'),'-o',str(runner),'-lmgba','-lpng'],check=True)
result=subprocess.run([str(runner),str(test/'pokefirered.gba'),str(out/'native'),*['0x'+symbols[s] for s in ('gCommunityDone','gCommunityPassed','gCommunityFailed','gCommunityObserved')]],capture_output=True,text=True,timeout=120)
(out/'emulator.log').write_text(result.stdout+result.stderr);assert result.returncode==0,(result.stdout,result.stderr[-2000:])
native=json.loads(result.stdout)
names=['national_dex_api','kadabra_37','machoke_37','graveler_37','haunter_37','everstone','sentret_furret_national','thunder_stone','shiny_boundaries','shiny_256_of_65536','packed_ivs_32bit','roamer_ivs','roamer_status_width','daycare_cap_normal','daycare_cap_hardcore','nature_pc_identity','full_pc_no_overwrite','last_pc_slot']
cases=[{'id':name,'status':'passed' if native['passed']&(1<<i) else 'failed'} for i,name in enumerate(names)]
assert all((native['passed']|native['failed'])&(1<<i) for i in range(len(names)))
report={'rom_sha256':hashlib.sha256((root/'output/firered-386-dev.gba').read_bytes()).hexdigest(),'cases':cases,'observed':dict(zip(['shiny_count','packed_ivs','roamer_ivs','roamer_status','daycare_level_normal','daycare_level_hardcore'],native['observed']))}
(out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
for case in cases:print(case['status'].upper(),case['id'])
print('Observed:',report['observed'])
if native['failed']:raise SystemExit(1)
