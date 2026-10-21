"""Interact with the actual coach from the overworld and edit via buttons."""
from pathlib import Path
import shutil, subprocess, tempfile
root=Path(__file__).resolve().parents[1]
out=root/'output/tests/training-ui'; out.mkdir(parents=True,exist_ok=True)
source=Path((root/'output/hack-build-path.txt').read_text().strip())
test=Path(tempfile.mkdtemp(prefix='training-ui-',dir=Path.home()/'.cache/firered-386'))
shutil.copytree(source,test,dirs_exist_ok=True)
(out/'build-path.txt').write_text(str(test)+'\n')
original=(root/'game/src/main.c').read_text()
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
overworld=(root/'game/src/overworld.c').read_text()
overworld=overworld.replace('    NewGameInitData();\n    ResetInitialPlayerAvatarState();','''    NewGameInitData();
    Nuzlocke_ChooseMode(GAME_MODE_HARDCORE);
    CreateMon(&gPlayerParty[0], SPECIES_BULBASAUR, 5, 15, TRUE, 0, OT_ID_PRESET, 0);
    CalculatePlayerPartyCount();
    SetWarpDestination(MAP_GROUP(MAP_VIRIDIAN_CITY_POKEMON_CENTER_1F), MAP_NUM(MAP_VIRIDIAN_CITY_POKEMON_CENTER_1F), -1, 10, 7);
    WarpIntoMap();
    ResetInitialPlayerAvatarState();''')
(test/'src/overworld.c').write_text(overworld)
# A task would be cleared by the menu; put assertions in the existing main loop instead.
main=(test/'src/main.c').read_text().replace('#include "global.h"', '#include "global.h"\nstatic void CheckTrainingUi(u8 task);').replace('    ReadKeys();','    CheckTrainingUi(0);\n    ReadKeys();')
assert 'CheckTrainingUi(0)' in main
(test/'src/main.c').write_text(main)
with (out/'build.log').open('w') as log:
 subprocess.run(['make','-j4'],cwd=test,stdout=log,stderr=subprocess.STDOUT,check=True)
symbols={p[2]:p[0] for line in subprocess.check_output(['arm-none-eabi-nm','pokefirered.elf'],cwd=test,text=True).splitlines() if len(p:=line.split())==3}
runner=test/'training-ui-smoke'
subprocess.run(['gcc','-O2','-Wall','-Wextra',str(root/'scripts/training_ui_smoke.c'),'-o',str(runner),'-lmgba','-lpng'],check=True)
with (out/'emulator.log').open('w') as log:
 subprocess.run([str(runner),str(test/'pokefirered.gba'),str(out/'coach'),*['0x'+symbols[s] for s in ('sTraining','gPlayerParty','gTrainingUiResult')]],stdout=log,stderr=subprocess.STDOUT,check=True,timeout=90)
print('PASS: actual coach/editor, cancel/confirm/return; nurse heals three times and refuses fourth without healing.')
