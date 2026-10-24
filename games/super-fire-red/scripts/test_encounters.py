"""Audit actual compiled wild slots/386 species and enter a postgame Mew battle."""
from pathlib import Path
import shutil,subprocess,tempfile
root=Path(__file__).resolve().parents[3]
out=root/'games/super-fire-red/output/tests/encounters';out.mkdir(parents=True,exist_ok=True)
source=Path((root/'games/super-fire-red/output/hack-build-path.txt').read_text().strip())
test=Path(tempfile.mkdtemp(prefix='encounter-fixture-',dir=Path.home()/'.cache/firered-386'))
shutil.copytree(source,test,dirs_exist_ok=True)
(out/'build-path.txt').write_text(str(test)+'\n')
text=(root/'games/super-fire-red/src/main.c').read_text().replace('static void InitMainCallbacks(void)\n{','extern void EncounterTestBoot(void);\nstatic void InitMainCallbacks(void)\n{').replace('SetMainCallback2(CB2_InitCopyrightScreenAfterBootup);','SetMainCallback2(EncounterTestBoot);')
(test/'src/main.c').write_text(text)
text=(root/'games/super-fire-red/src/wild_encounter.c').read_text()+'\n'+(root/'games/super-fire-red/scripts/encounter_fixture.c').read_text()
(test/'src/wild_encounter.c').write_text(text)
with (out/'build.log').open('w') as log:subprocess.run(['make','-j4'],cwd=test,stdout=log,stderr=subprocess.STDOUT,check=True)
symbols={p[2]:p[0] for line in subprocess.check_output(['arm-none-eabi-nm','pokefirered.elf'],cwd=test,text=True).splitlines() if len(p:=line.split())==3}
runner=test/'encounter-smoke'
subprocess.run(['gcc','-O2','-Wall','-Wextra',str(root/'games/super-fire-red/scripts/encounter_smoke.c'),'-o',str(runner),'-lmgba','-lpng'],check=True)
names=('gEncounterTestResult','gEncounterTestSpecies','gEncounterTestSlots','gBattleMainFunc','HandleTurnActionSelectionState','gEnemyParty','gBattlersCount')
result=subprocess.run([str(runner),str(test/'pokefirered.gba'),str(out/'encounters'),*['0x'+symbols[s] for s in names]],capture_output=True,text=True,timeout=120)
(out/'emulator.log').write_text(result.stdout+result.stderr)
assert result.returncode==0,(result.stdout,result.stderr[-1500:])
print(result.stdout.strip())
