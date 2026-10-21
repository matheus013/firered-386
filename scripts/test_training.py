"""Exercise trainer edits, mode gates, shop stock and center limits in a saved ROM."""
from pathlib import Path
import shutil
import subprocess
import tempfile

root=Path(__file__).resolve().parents[1]
out=root/'output/tests/training'
out.mkdir(parents=True,exist_ok=True)
cache=Path.home()/'.cache/firered-386'
source=Path((root/'output/hack-build-path.txt').read_text().strip())
test=Path(tempfile.mkdtemp(prefix='training-fixture-',dir=cache))
shutil.copytree(source,test,dirs_exist_ok=True)
(out/'build-path.txt').write_text(str(test)+'\n')
runner=test/'training-smoke'
subprocess.run(['gcc','-O2','-Wall','-Wextra',str(root/'scripts/hardcore_smoke.c'),'-o',str(runner),'-lmgba','-lpng'],check=True)
original=(root/'game/src/main.c').read_text()
hook=(root/'scripts/training_fixture.c').read_text()
main=original.replace('static void InitMainCallbacks(void)\n{',hook+'\nstatic void InitMainCallbacks(void)\n{').replace('SetMainCallback2(CB2_InitCopyrightScreenAfterBootup);','SetMainCallback2(TrainingTestBoot);')
(test/'src/main.c').write_text(main)
with (out/'rules-build.log').open('w') as log:
 subprocess.run(['make','-j4'],cwd=test,stdout=log,stderr=subprocess.STDOUT,check=True)
symbols={p[2]:p[0] for line in subprocess.check_output(['arm-none-eabi-nm','pokefirered.elf'],cwd=test,text=True).splitlines() if len(p:=line.split())==3}
rom=out/'training-fixture.gba'
shutil.copyfile(test/'pokefirered.gba',rom)
with (out/'rules-emulator.log').open('w') as log:
 subprocess.run([str(runner),str(rom),str(out/'rules'),'rules','0x'+symbols['gHardcoreTestResult']],stdout=log,stderr=subprocess.STDOUT,timeout=90,check=True)
print('PASS: training edits, mode gates, stock/price, independent three-use center limits and fresh-instance save reload.')
