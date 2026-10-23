"""Audit actual E4/Champion parties and play turns in disposable ROMs."""
from pathlib import Path
import json
import re
import shutil
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[2]
OUT = ROOT / 'game/output/tests/elite-four'
OUT.mkdir(parents=True, exist_ok=True)
cache = Path.home() / '.cache/firered-386'
source = Path((ROOT / 'game/output/hack-build-path.txt').read_text().strip())
fixture = Path(tempfile.mkdtemp(prefix='elite-four-fixture-', dir=cache))
shutil.copytree(source, fixture, dirs_exist_ok=True)
(OUT / 'build-path.txt').write_text(str(fixture) + '\n')
runner = fixture / 'elite-four-smoke'
subprocess.run(['gcc', '-O2', '-Wall', '-Wextra', str(ROOT/'game/scripts/smoke.c'), '-o', str(runner), '-lmgba', '-lpng'], check=True)
teams = json.loads((ROOT / 'game/hack-data/trainer_balance.json').read_text())['teams']
configs = {name: config for name, config in teams.items() if name.startswith(('sParty_EliteFour', 'sParty_Champion'))}
main_source = (ROOT / 'game/src/main.c').read_text()
battle_source = (ROOT / 'game/src/battle_main.c').read_text()
boot = r'''
#include "new_game.h"
#include "pokemon.h"
#include "battle.h"
#include "overworld.h"
#include "fieldmap.h"
#include "string_util.h"
#include "constants/opponents.h"
#include "constants/maps.h"
extern void CB2_InitBattle(void);
static void EliteTestIdle(void) {}
static void EliteTestBoot(void)
{
    u8 i;
    static const u8 name[] = _("TEST");
    StringCopy(gSaveBlock2Ptr->playerName, name);
    StringCopy(gSaveBlock1Ptr->rivalName, name);
    NewGameInitData();
    gSaveBlock2Ptr->optionsTextSpeed = 2;
    gSaveBlock2Ptr->optionsBattleSceneOff = TRUE;
    gMapHeader = *Overworld_GetMapHeaderByGroupAndId(MAP_GROUP(MAP_ROUTE1), MAP_NUM(MAP_ROUTE1));
    for (i = 0; i < PARTY_SIZE; i++) {
        CreateMon(&gPlayerParty[i], SPECIES_SNORLAX, 100, 31, FALSE, 0, OT_ID_PLAYER_ID, 0);
        // Keep this automation fixture alive through level-100 doubles;
        // the test audits NPC construction/turn execution, not win probability.
        gPlayerParty[i].hp = gPlayerParty[i].maxHP = 2000;
    }
    CalculatePlayerPartyCount();
    gTrainerBattleOpponent_A = TEST_TRAINER;
    gBattleTypeFlags = BATTLE_TYPE_TRAINER;
    gMain.savedCallback = EliteTestIdle;
    SetMainCallback2(CB2_InitBattle);
}
'''
for key, config in configs.items():
    for _ in (0,):
        trainer = config['id']
        label = key.removeprefix('sParty_').lower()
        # Independent expectations from the source config; no calls to the
        # production training helper in the audit. Check actual decrypted mons.
        checks = ['EWRAM_DATA volatile u32 gEliteFixtureResult = 0;', 'static void AuditEliteParty(struct Pokemon *party) {', 'gEliteFixtureResult = 2;']
        for slot, mon in enumerate(config['team']):
            prefix = f'&party[{slot}]'
            level = mon['level']
            for field, val in [('SPECIES', 'SPECIES_'+mon['species']), ('LEVEL', str(level)), ('HELD_ITEM', 'ITEM_'+mon['item'])]:
                checks.append(f'if (GetMonData({prefix}, MON_DATA_{field}) != {val}) return;')
            checks += [f'if (GetNature({prefix}) != NATURE_{mon["nature"]}) return;', f'if (GetMonAbility({prefix}) != ABILITY_{mon["ability"]}) return;']
            for stat, ev in enumerate(mon['evs']):
                checks += [f'if (GetMonData({prefix}, MON_DATA_HP_EV + {stat}) != {ev}) return;', f'if (GetMonData({prefix}, MON_DATA_HP_IV + {stat}) != 31) return;']
            for move_slot, move in enumerate(mon['moves']):
                checks.append(f'if (GetMonData({prefix}, MON_DATA_MOVE1 + {move_slot}) != MOVE_{move}) return;')
            checks += [f'if (GetMonData({prefix}, MON_DATA_HP) != GetMonData({prefix}, MON_DATA_MAX_HP)) return;', f'if (GetMonData({prefix}, MON_DATA_HP) < 100) return;']
        checks += ['gEliteFixtureResult = 1;', '}']
        audit = '\n'.join(checks) + '\n'
        text = battle_source.replace('static u8 CreateNPCTrainerParty(struct Pokemon *party, u16 trainerNum)\n{', audit + '\nstatic u8 CreateNPCTrainerParty(struct Pokemon *party, u16 trainerNum)\n{')
        text = text.replace('gBattleTypeFlags |= gTrainers[trainerNum].doubleBattle;', 'AuditEliteParty(party);\n        gBattleTypeFlags |= gTrainers[trainerNum].doubleBattle;')
        (fixture / 'src/battle_main.c').write_text(text)
        text = main_source.replace('static void InitMainCallbacks(void)\n{', boot.replace('TEST_TRAINER', trainer) + '\nstatic void InitMainCallbacks(void)\n{')
        text = text.replace('SetMainCallback2(CB2_InitCopyrightScreenAfterBootup);', 'SetMainCallback2(EliteTestBoot);')
        (fixture / 'src/main.c').write_text(text)
        with (OUT / f'{label}-build.log').open('w') as log:
            subprocess.run(['make', '-j4'], cwd=fixture, stdout=log, stderr=subprocess.STDOUT, check=True)
        symbols = {}
        for line in subprocess.check_output(['arm-none-eabi-nm', 'pokefirered.elf'], cwd=fixture, text=True).splitlines():
            parts = line.split()
            if len(parts) == 3: symbols[parts[2]] = int(parts[0], 16)
        rom = OUT / f'{label}.gba'
        shutil.copyfile(fixture/'pokefirered.gba', rom)
        addresses = [symbols[s] for s in ('gBattleMainFunc', 'HandleTurnActionSelectionState', 'gBattleTypeFlags', 'gAbsentBattlerFlags', 'gBattlersCount', 'gEliteFixtureResult')]
        addresses.append(symbols['gBattleResults'] + 0x13)
        args = [str(runner), str(rom), str(OUT/f'{label}.png'), '18000', 'battle'] + [hex(a) for a in addresses]
        result = subprocess.run(args, capture_output=True, text=True, timeout=90)
        (OUT / f'{label}-emulator.log').write_text(result.stdout + result.stderr)
        assert result.returncode == 0, (label, result.stdout[-2000:], result.stderr[-1000:])
        assert re.search(r'battle_ready=1 battle_flags=\d+ absent=0 battlers=4', result.stdout), (label, result.stdout)
        assert 'party_audit=1' in result.stdout
        print(f'PASS {label}: six actual mons audited; ' + result.stdout.split('party_audit=')[1].splitlines()[0], flush=True)
