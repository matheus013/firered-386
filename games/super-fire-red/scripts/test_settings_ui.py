"""Button tests for the production intro plus a disposable native Options screen."""
from pathlib import Path
import hashlib,json,shutil,subprocess,tempfile
root=Path(__file__).resolve().parents[3];out=root/'games/super-fire-red/output/tests/settings-ui';out.mkdir(parents=True,exist_ok=True)
source=Path((root/'games/super-fire-red/output/hack-build-path.txt').read_text().strip())
reference_sha=hashlib.sha256((root/'games/super-fire-red/output/firered-386-dev.gba').read_bytes()).hexdigest()
assert hashlib.sha256((source/'pokefirered.gba').read_bytes()).hexdigest()==reference_sha
def symbols(folder):return {p[2]:'0x'+p[0] for line in subprocess.check_output(['arm-none-eabi-nm','pokefirered.elf'],cwd=folder,text=True).splitlines() if len(p:=line.split())==3}
runner=out/'settings-ui-smoke';subprocess.run(['gcc','-O2',str(root/'games/super-fire-red/scripts/settings_ui_smoke.c'),'-o',str(runner),'-lmgba','-lpng'],check=True)
production=out/'production.gba';shutil.copyfile(root/'games/super-fire-red/output/firered-386-dev.gba',production)
sym=symbols(source)
with (out/'intro.log').open('w') as log:subprocess.run([str(runner),str(production),str(out/'intro'),'intro',*[sym[s] for s in ['gTasks','Task_ChooseGameMode','Task_ConfirmGameMode','Task_ChooseRandomizer','gSaveBlock2Ptr']]],stdout=log,stderr=subprocess.STDOUT,check=True,timeout=90)
test=Path(tempfile.mkdtemp(prefix='settings-ui-',dir=Path.home()/'.cache/firered-386'));shutil.copytree(source,test,dirs_exist_ok=True)
hook='''
#include "new_game.h"
#include "load_save.h"
#include "game_settings.h"
#include "nuzlocke.h"
#include "option_menu.h"
#include "string_util.h"
EWRAM_DATA u32 gSettingsUiResult=0;
static void SettingsUiIdle(void){gSettingsUiResult=GameSettings_GetLanguage()==GAME_LANGUAGE_PTBR && Nuzlocke_IsHardcore() && GameSettings_IsRandomizer()?1:2;}
static void SettingsUiBoot(void){
 static const u8 name[] = _("TEST");
 SetSaveBlocksPointers();StringCopy(gSaveBlock2Ptr->playerName,name);StringCopy(gSaveBlock1Ptr->rivalName,name);
 NewGameInitData();Nuzlocke_ChooseMode(GAME_MODE_HARDCORE);GameSettings_ChooseRandomizer(TRUE);
 gMain.savedCallback=SettingsUiIdle;SetMainCallback2(CB2_OptionsMenuFromStartMenu);
}
'''
main=(test/'src/main.c').read_text();(test/'src/main.c').write_text(main.replace('static void InitMainCallbacks(void)\n{',hook+'\nstatic void InitMainCallbacks(void)\n{').replace('SetMainCallback2(CB2_InitCopyrightScreenAfterBootup);','SetMainCallback2(SettingsUiBoot);'))
with (out/'build.log').open('w') as log:subprocess.run(['make','-j4'],cwd=test,stdout=log,stderr=subprocess.STDOUT,check=True)
sym=symbols(test)
with (out/'options.log').open('w') as log:subprocess.run([str(runner),str(test/'pokefirered.gba'),str(out/'menu'),'options',*[sym[s] for s in ['sOptionMenuPtr','gSettingsUiResult','gSaveBlock2Ptr']]],stdout=log,stderr=subprocess.STDOUT,check=True,timeout=90)
assert hashlib.sha256((root/'games/super-fire-red/output/firered-386-dev.gba').read_bytes()).hexdigest()==reference_sha
(out/'results.json').write_text(json.dumps({'rom_sha256':reference_sha,'status':'passed','intro':'unmodified production ROM; real buttons; mode/randomizer choice flow','options':'disposable fixture opening native Options; real button language change and persistence'},indent=2)+'\n')
print('PASS production new-game flow and native bilingual options',flush=True)
