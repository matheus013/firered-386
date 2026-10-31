"""Interact with the actual coach from the overworld and edit via buttons."""
from pathlib import Path
import argparse,hashlib,json,re,shutil,subprocess,tempfile
from png_util import read_png
root=Path(__file__).resolve().parents[3]
parser=argparse.ArgumentParser();parser.add_argument('--language',choices=['en','ptbr'],default='en');parser.add_argument('--mode',choices=['normal','hardcore'],default='hardcore');args=parser.parse_args()
out=root/'games/super-fire-red/output/tests'/(('training-ui' if args.language=='en' else 'training-ui-ptbr')+('-normal' if args.mode=='normal' else '')); out.mkdir(parents=True,exist_ok=True)
source=Path((root/'games/super-fire-red/output/hack-build-path.txt').read_text().strip())
reference_sha=hashlib.sha256((root/'games/super-fire-red/output/firered-386-dev.gba').read_bytes()).hexdigest()
assert hashlib.sha256((source/'pokefirered.gba').read_bytes()).hexdigest()==reference_sha
test=Path(tempfile.mkdtemp(prefix='training-ui-',dir=Path.home()/'.cache/firered-386'))
shutil.copytree(source,test,dirs_exist_ok=True)
(out/'build-path.txt').write_text(str(test)+'\n')
original=(root/'games/super-fire-red/src/main.c').read_text()
hook='''
#include "new_game.h"
#include "nuzlocke.h"
#include "overworld.h"
#include "pokemon.h"
#include "task.h"
#include "training.h"
#include "load_save.h"
#include "string_util.h"
#include "script.h"
EWRAM_DATA u32 gTrainingUiResult = 0;
static EWRAM_DATA u8 sNurseStage = 0;
static void CheckTrainingUi(u8 task)
{
    if (gTrainingUiResult == 1) gTrainingUiResult = GetMonData(&gPlayerParty[0], MON_DATA_HP_EV) == 0 ? 101 : 201;
    if (gTrainingUiResult == 2) gTrainingUiResult = GetMonData(&gPlayerParty[0], MON_DATA_HP_EV) == 252 && GetMonData(&gPlayerParty[0], MON_DATA_HP_IV) == 31 ? 102 : 202;
    if (gTrainingUiResult == 10) {
        gPlayerParty[0].hp = 1;
        sNurseStage = 1;
        gTrainingUiResult = 110;
    }
    if (sNurseStage == 1 && ScriptContext_IsEnabled()) sNurseStage = 2;
    if (sNurseStage == 2 && !ScriptContext_IsEnabled() && !ArePlayerFieldControlsLocked()) {
        gTrainingUiResult = 200 + Training_GetCenterUses() + (gPlayerParty[0].hp > 1 ? 1000 : 0);
        sNurseStage = 0;
    }
}
static void TrainingUiBoot(void) {
    static const u8 name[] = _("TEST");
    SetSaveBlocksPointers();
    StringCopy(gSaveBlock2Ptr->playerName, name);
    StringCopy(gSaveBlock1Ptr->rivalName, name);
    SetMainCallback2(CB2_NewGame);
}
'''
(test/'src/main.c').write_text(original.replace('static void InitMainCallbacks(void)\n{',hook+'\nstatic void InitMainCallbacks(void)\n{').replace('SetMainCallback2(CB2_InitCopyrightScreenAfterBootup);','SetMainCallback2(TrainingUiBoot);'))
# NewGame's existing map-load and field callbacks are used; only its destination changes.
overworld=(root/'games/super-fire-red/src/overworld.c').read_text().replace('#include "global.h"','#include "global.h"\n#include "game_settings.h"')
# Start from a non-neutral nature so undoing it cannot pass for the new nature's colors.
START_NATURE=11 # Hasty
overworld=overworld.replace('#include "game_settings.h"','#include "game_settings.h"\n#define START_NATURE '+str(START_NATURE),1)
overworld=overworld.replace('    NewGameInitData();\n    ResetInitialPlayerAvatarState();','''    NewGameInitData();
    Nuzlocke_ChooseMode(GAME_MODE_HARDCORE);
    CreateMon(&gPlayerParty[0], SPECIES_BULBASAUR, 5, 15, TRUE, START_NATURE, OT_ID_PRESET, 0);
    CalculatePlayerPartyCount();
    SetWarpDestination(MAP_GROUP(MAP_VIRIDIAN_CITY_POKEMON_CENTER_1F), MAP_NUM(MAP_VIRIDIAN_CITY_POKEMON_CENTER_1F), -1, 10, 7);
    WarpIntoMap();
    ResetInitialPlayerAvatarState();''')
(test/'src/overworld.c').write_text(overworld)
if args.language=='ptbr':(test/'src/overworld.c').write_text(overworld.replace('    Nuzlocke_ChooseMode(GAME_MODE_HARDCORE);','    Nuzlocke_ChooseMode(GAME_MODE_HARDCORE);\n    GameSettings_SetLanguage(GAME_LANGUAGE_PTBR);'))
# A task would be cleared by the menu; put assertions in the existing main loop instead.
if args.mode=='normal':
 (test/'src/overworld.c').write_text((test/'src/overworld.c').read_text().replace('Nuzlocke_ChooseMode(GAME_MODE_HARDCORE)','Nuzlocke_ChooseMode(GAME_MODE_NORMAL)'))
 (test/'src/main.c').write_text((test/'src/main.c').read_text().replace('MON_DATA_HP_IV) == 31','MON_DATA_HP_IV) == 15'))
main=(test/'src/main.c').read_text().replace('#include "global.h"', '#include "global.h"\nstatic void CheckTrainingUi(u8 task);').replace('    ReadKeys();','    CheckTrainingUi(0);\n    ReadKeys();')
assert 'CheckTrainingUi(0)' in main
(test/'src/main.c').write_text(main)
with (out/'build.log').open('w') as log:
 subprocess.run(['make','-j4'],cwd=test,stdout=log,stderr=subprocess.STDOUT,check=True)
symbols={p[2]:p[0] for line in subprocess.check_output(['arm-none-eabi-nm','pokefirered.elf'],cwd=test,text=True).splitlines() if len(p:=line.split())==3}
runner=test/'training-ui-smoke'
subprocess.run(['gcc','-O2','-Wall','-Wextra',*(['-DNORMAL_EDITOR'] if args.mode=='normal' else []),str(root/'games/super-fire-red/scripts/training_ui_smoke.c'),'-o',str(runner),'-lmgba','-lpng'],check=True)
with (out/'emulator.log').open('w') as log:
 subprocess.run([str(runner),str(test/'pokefirered.gba'),str(out/'coach'),*['0x'+symbols[s] for s in ('sTraining','gPlayerParty','gTrainingUiResult')]],stdout=log,stderr=subprocess.STDOUT,check=True,timeout=90)
if args.mode!='normal':
 # Each nature page must color exactly the raised stat green and the lowered stat red (none if neutral).
 reference=json.loads((root/'games/super-fire-red/hack-data/nature_reference.json').read_text())
 ids={name:int(value) for name,value in re.findall(r'#define NATURE_(\w+)\s+(\d+)',(root/'games/super-fire-red/include/constants/pokemon.h').read_text())}
 screen_rows=['hp','attack','defense','speed','spatk','spdef'];expected={}
 for raised,row in zip(reference['stats'],reference['grid']):
  for lowered,nature in zip(reference['stats'],row):expected[ids[nature]]=(nature,set() if raised==lowered else {screen_rows.index(raised)},set() if raised==lowered else {screen_rows.index(lowered)})
 for nature_id,(nature,green,red) in sorted(expected.items()):
  pixels=read_png(out/f'coach-nature-{(nature_id-START_NATURE)%25:02d}.png')
  found={color:{(y-57)//12 for y in range(57,129) for x in range(150,240) if pixels[y][x]==value} for color,value in (('green',(13,27,17)),('red',(31,12,14)))}
  assert (found['green'],found['red'])==(green,red),(nature,'green rows',found['green'],'red rows',found['red'],'expected',green,red)
 print('PASS: 25 nature pages color only the raised (green) and lowered (red) stat.')
print('PASS: Normal EV access, cancel/apply, Nature/IV locks, exit/reopen.' if args.mode=='normal' else 'PASS: actual coach/editor, cancel/confirm/return; nurse heals three times and refuses fourth without healing.')
assert hashlib.sha256((root/'games/super-fire-red/output/firered-386-dev.gba').read_bytes()).hexdigest()==reference_sha
(out/'results.json').write_text(json.dumps({'rom_sha256':reference_sha,'status':'passed','language':args.language,'mode':args.mode,'scope':'native Normal EV access, cancel/apply and advanced locks' if args.mode=='normal' else 'disposable fixture: native NPC/editor/nurse interaction by buttons; nature/EV/IV changes, cancellation, exit/reopen and healing limits'},indent=2)+'\n')
