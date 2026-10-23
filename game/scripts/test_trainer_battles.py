"""Emulator integration fixtures in a separate COPY of a Linux build directory.

Does not inject test hooks into the project's actual source or deliverable ROM.
Run inside WSL after a successful build. Logs, screenshots and fixture ROMs remain
in game/output/tests/. Checks battle entry and arrival at the first action-selection
phase, not complete fights or campaign progression.
"""
from pathlib import Path
import re
import shutil
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[2]
OUT = ROOT / "game/output/tests"
CACHE = Path.home() / ".cache/firered-386"
OUT.mkdir(parents=True, exist_ok=True)
source = Path((ROOT / "game/output/hack-build-path.txt").read_text().strip())
test_dir = Path(tempfile.mkdtemp(prefix="trainer-fixture-", dir=CACHE))
shutil.copytree(source, test_dir, dirs_exist_ok=True)
smoke = CACHE / "smoke"
subprocess.run(["gcc", "-O2", "-Wall", "-Wextra", str(ROOT / "game/scripts/smoke.c"), "-o", str(smoke), "-lmgba", "-lpng"], check=True)

hook = r'''
#include "new_game.h"
#include "pokemon.h"
#include "battle.h"
#include "overworld.h"
#include "fieldmap.h"
#include "string_util.h"
#include "constants/trainers.h"
#include "constants/maps.h"
extern void CB2_InitBattle(void);
static void TrainerTestIdle(void) {}
static void TrainerTestBoot(void)
{
    static const u8 testName[] = _("TEST");
    StringCopy(gSaveBlock2Ptr->playerName, testName);
    StringCopy(gSaveBlock1Ptr->rivalName, testName);
    NewGameInitData();
    gSaveBlock2Ptr->optionsTextSpeed = 2;
    gSaveBlock2Ptr->optionsBattleSceneOff = TRUE;
    gMapHeader = *Overworld_GetMapHeaderByGroupAndId(MAP_GROUP(MAP_ROUTE1), MAP_NUM(MAP_ROUTE1));
    CreateMon(&gPlayerParty[0], SPECIES_BULBASAUR, 15, 15, FALSE, 0, OT_ID_PLAYER_ID, 0);
    TEST_SECOND_MON
    CalculatePlayerPartyCount();
    gTrainerBattleOpponent_A = TEST_TRAINER;
    gBattleTypeFlags = BATTLE_TYPE_TRAINER;
    gMain.savedCallback = TrainerTestIdle;
    SetMainCallback2(CB2_InitBattle);
}
'''
base_main = (ROOT / "game/src/main.c").read_text()
cases = [("lab-single", "TRAINER_RIVAL_OAKS_LAB_SQUIRTLE", 1, False),
         ("normal-double", "TRAINER_YOUNGSTER_BEN", 2, True),
         ("one-versus-two", "TRAINER_YOUNGSTER_BEN", 1, True)]
for label, trainer, count, double in cases:
    second = "CreateMon(&gPlayerParty[1], SPECIES_CHARMANDER, 15, 15, FALSE, 0, OT_ID_PLAYER_ID, 0);" if count == 2 else ""
    fixture = hook.replace("TEST_SECOND_MON", second).replace("TEST_TRAINER", trainer)
    main = base_main.replace("static void InitMainCallbacks(void)\n{", fixture + "\nstatic void InitMainCallbacks(void)\n{")
    main = main.replace("SetMainCallback2(CB2_InitCopyrightScreenAfterBootup);", "SetMainCallback2(TrainerTestBoot);")
    (test_dir / "src/main.c").write_text(main)
    with (OUT / f"{label}-build.log").open("w") as log:
        subprocess.run(["make", "-j4"], cwd=test_dir, stdout=log, stderr=subprocess.STDOUT, check=True)
    symbols = {}
    for line in subprocess.check_output(["arm-none-eabi-nm", "pokefirered.elf"], cwd=test_dir, text=True).splitlines():
        parts = line.split()
        if len(parts) == 3: symbols[parts[2]] = parts[0]
    rom = OUT / f"{label}.gba"
    shutil.copy2(test_dir / "pokefirered.gba", rom)
    args = [str(smoke), str(rom), str(OUT / f"{label}.png"), "1800", "battle"]
    args += ["0x" + symbols[s] for s in ("gBattleMainFunc", "HandleTurnActionSelectionState", "gBattleTypeFlags", "gAbsentBattlerFlags", "gBattlersCount")]
    with (OUT / f"{label}-emulator.log").open("w") as log:
        subprocess.run(args, stdout=log, stderr=subprocess.STDOUT, check=True, timeout=45)
    text = (OUT / f"{label}-emulator.log").read_text()
    match = re.search(r"battle_ready=(\d+) battle_flags=(\d+) absent=(\d+) battlers=(\d+)", text)
    assert match, label
    ready, flags, absent, battlers = map(int, match.groups())
    assert ready == 1 and bool(flags & 1) == double, (label, match[0])
    assert battlers == (4 if double else 2), (label, match[0])
    assert bool(absent & 4) == (double and count == 1), (label, match[0])
    print(f"PASS: {label}: {match[0]}", flush=True)
