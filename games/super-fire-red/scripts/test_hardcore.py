"""Build disposable ROM fixtures and verify Hardcore rules, savedata and new-game UI in mGBA."""
from pathlib import Path
import shutil
import subprocess
import tempfile
import argparse

root = Path(__file__).resolve().parents[3]
out = root / 'games/super-fire-red/output/tests/hardcore'
out.mkdir(parents=True, exist_ok=True)
source = Path((root / 'games/super-fire-red/output/hack-build-path.txt').read_text().strip())
parser = argparse.ArgumentParser()
parser.add_argument('--build-dir', type=Path, help='Reuse a previous disposable fixture build after test-only changes')
parser.add_argument('--case', choices=('rules', 'menu', 'gameover', 'battle-loss'))
args = parser.parse_args()
test = args.build_dir or Path(tempfile.mkdtemp(prefix='hardcore-fixture-', dir=Path.home() / '.cache/firered-386'))
if not args.build_dir:
    shutil.copytree(source, test, dirs_exist_ok=True)
(out / 'build-path.txt').write_text(str(test) + '\n')
runner = test / 'hardcore-smoke'
subprocess.run(['gcc', '-O2', '-Wall', '-Wextra', str(root / 'games/super-fire-red/scripts/hardcore_smoke.c'), '-o', str(runner), '-lmgba', '-lpng'], check=True)
original = (root / 'games/super-fire-red/src/main.c').read_text()
for case in ((args.case,) if args.case else ('rules', 'menu', 'gameover', 'battle-loss')):
    if case == 'rules':
        hook = (root / 'games/super-fire-red/scripts/hardcore_fixture.c').read_text()
    elif case == 'battle-loss':
        hook = r'''
#include "nuzlocke.h"
#include "new_game.h"
#include "load_save.h"
#include "pokemon.h"
#include "battle.h"
#include "overworld.h"
#include "fieldmap.h"
#include "string_util.h"
#include "constants/trainers.h"
#include "constants/maps.h"
extern void CB2_InitBattle(void);
static void HardcoreTestBoot(void)
{
    static const u8 name[] = _("TEST");
    SetSaveBlocksPointers();
    StringCopy(gSaveBlock2Ptr->playerName, name);
    StringCopy(gSaveBlock1Ptr->rivalName, name);
    NewGameInitData();
    Nuzlocke_ChooseMode(GAME_MODE_HARDCORE);
    Nuzlocke_Start();
    gSaveBlock2Ptr->optionsTextSpeed = 2;
    gSaveBlock2Ptr->optionsBattleSceneOff = TRUE;
    CreateMon(&gPlayerParty[0], SPECIES_MAGIKARP, 2, 15, FALSE, 0, OT_ID_PLAYER_ID, 0);
    CalculatePlayerPartyCount();
    gMapHeader = *Overworld_GetMapHeaderByGroupAndId(MAP_GROUP(MAP_ROUTE1), MAP_NUM(MAP_ROUTE1));
    gTrainerBattleOpponent_A = TRAINER_YOUNGSTER_BEN;
    gBattleTypeFlags = BATTLE_TYPE_TRAINER | BATTLE_TYPE_DOUBLE;
    gMain.savedCallback = CB2_WhiteOut;
    SetMainCallback2(CB2_InitBattle);
}
'''
    else:
        callback = 'ScanlineEffect_Stop(); ResetTasks(); StartNewGameScene();' if case == 'menu' else 'Nuzlocke_ChooseMode(GAME_MODE_HARDCORE); Nuzlocke_Start(); SetMainCallback2(CB2_WhiteOut);'
        hook = '#include "nuzlocke.h"\n#include "oak_speech.h"\n#include "overworld.h"\n#include "task.h"\n#include "scanline_effect.h"\nstatic void HardcoreTestBoot(void) {' + callback + '}\n'
    main = original.replace('static void InitMainCallbacks(void)\n{', hook + '\nstatic void InitMainCallbacks(void)\n{')
    main = main.replace('SetMainCallback2(CB2_InitCopyrightScreenAfterBootup);', 'SetMainCallback2(HardcoreTestBoot);')
    (test / 'src/main.c').write_text(main)
    with (out / f'{case}-build.log').open('w') as log:
        subprocess.run(['make', '-j4'], cwd=test, stdout=log, stderr=subprocess.STDOUT, check=True)
    symbols = {}
    for line in subprocess.check_output(['arm-none-eabi-nm', 'pokefirered.elf'], cwd=test, text=True).splitlines():
        parts = line.split()
        if len(parts) == 3: symbols[parts[2]] = parts[0]
    rom = out / f'{case}-fixture.gba'
    shutil.copy2(test / 'pokefirered.gba', rom)
    if case == 'rules':
        addresses = ['gHardcoreTestResult']
        variants = ['rules']
    elif case == 'menu':
        addresses = ['gTasks', 'gSaveBlock2Ptr', 'Task_ChooseGameMode', 'Task_ConfirmGameMode', 'Task_ControlsGuide_HandleInput', 'Task_ChooseRandomizer', 'gMain']
        variants = ['normal', 'hardcore']
    elif case == 'gameover':
        addresses = ['gSaveBlock2Ptr']
        variants = ['gameover']
    else:
        addresses = ['gSaveBlock2Ptr', 'gPlayerParty', 'gBattlerControllerFuncs', 'gActiveBattler', 'gBattlersCount']
        variants = ['battle-loss']
    for variant in variants:
        args = [str(runner), str(rom), str(out / variant), variant] + ['0x' + symbols[name] for name in addresses]
        with (out / f'{variant}-emulator.log').open('w') as log:
            subprocess.run(args, stdout=log, stderr=subprocess.STDOUT, check=True, timeout=90)
        print(f'PASS: {variant}', flush=True)
print('All Hardcore fixture tests passed. User ROM and saves were not opened or modified by these tests.')
