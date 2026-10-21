"""Validate first eligible wild encounter per section, native ball commands and save persistence."""
from pathlib import Path
import hashlib,json,shutil,subprocess,tempfile
root=Path(__file__).resolve().parents[1];out=root/'output/tests/capture';out.mkdir(parents=True,exist_ok=True)
source=Path((root/'output/hack-build-path.txt').read_text().strip())
test=Path(tempfile.mkdtemp(prefix='capture-fixture-',dir=Path.home()/'.cache/firered-386'))
shutil.copytree(source,test,dirs_exist_ok=True);(out/'build-path.txt').write_text(str(test)+'\n')
main=(root/'game/src/main.c').read_text()
main=main.replace('static void InitMainCallbacks(void)\n{',(root/'scripts/capture_fixture.c').read_text()+'\nstatic void InitMainCallbacks(void)\n{').replace('SetMainCallback2(CB2_InitCopyrightScreenAfterBootup);','SetMainCallback2(CaptureBoot);')
(test/'src/main.c').write_text(main)
(test/'src/battle_script_commands.c').write_text((root/'game/src/battle_script_commands.c').read_text()+'\n'+(root/'scripts/capture_commands.c').read_text())
with (out/'build.log').open('w') as log:subprocess.run(['make','-j4'],cwd=test,stdout=log,stderr=subprocess.STDOUT,check=True)
symbols={p[2]:p[0] for line in subprocess.check_output(['arm-none-eabi-nm','pokefirered.elf'],cwd=test,text=True).splitlines() if len(p:=line.split())==3}
runner=test/'capture-smoke'
subprocess.run(['gcc','-O2','-Wall','-Wextra',str(root/'scripts/capture_smoke.c'),'-o',str(runner),'-lmgba','-lpng'],check=True)
result=subprocess.run([str(runner),str(test/'pokefirered.gba'),str(out/'capture'),*['0x'+symbols[s] for s in ('gCaptureDone','gCapturePassed','gCaptureFailed','gCaptureObserved')]],capture_output=True,text=True,timeout=120)
(out/'emulator.log').write_text(result.stdout+result.stderr);assert result.returncode==0,(result.returncode,result.stdout,result.stderr[-2000:])
native=json.loads(result.stdout)
names=['before_oak_balls','first_eligible_consumes','multiple_balls_same_battle','flee_loses_attempt','ko_loses_attempt','different_route_independent','return_route_still_used','duplicate_skips_without_consuming','nonduplicate_after_duplicate','exact_species_not_family','shiny_has_no_exception','trainer_does_not_consume','link_does_not_consume','tutorial_does_not_consume','cave_floors_share','safari_sectors_share_ball_block','static_same_section_blocked','unidentified_ghost_does_not_consume','revealed_marowak_does_not_consume','native_catch_then_next_ball_blocked','normal_unrestricted','fresh_instance_quota_persistence']
names+=['all_compiled_wild_maps_first_only']
cases=[{'id':name,'status':'passed' if native['passed']&(1<<i) and not native['failed']&(1<<i) else 'failed'} for i,name in enumerate(names)]
assert all((native['passed']|native['failed'])&(1<<i) for i in range(len(names)))
report={'rom_sha256':hashlib.sha256((root/'output/firered-386-dev.gba').read_bytes()).hexdigest(),'cases':cases,'save_bytes':native['save_bytes'],'fresh_instances':native['fresh_instances'],'observed':native['observed']}
(out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
for row in cases:print(row['status'].upper(),row['id'])
if any(row['status']=='failed' for row in cases):raise SystemExit(1)
