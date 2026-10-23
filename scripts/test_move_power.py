"""Native checks of generation-nine variable power, stat-source moves and Freeze-Dry.

Each damage check compares CalculateBaseDamage for the move with the same call using the expected
power as powerOverride (or a reference move with the swapped stat), so results must match exactly.
"""
from pathlib import Path
import hashlib,json,shutil,subprocess,tempfile,sys
ROOT=Path(__file__).resolve().parents[1];OUT=ROOT/'output/tests/move-power';OUT.mkdir(parents=True,exist_ok=True)
source=Path((ROOT/'output/hack-build-path.txt').read_text().strip())
reference=hashlib.sha256((ROOT/'output/firered-386-dev.gba').read_bytes()).hexdigest()
assert hashlib.sha256((source/'pokefirered.gba').read_bytes()).hexdigest()==reference
NAMES=['hex_plain','hex_status','venoshock_poison','venoshock_other_status','brine_half','brine_full','acrobatics_no_item','acrobatics_item',
       'assurance_hit','assurance_fresh','payback_last','payback_first','electro_ball_4x','electro_ball_2x','electro_ball_slower','electro_ball_paralysis',
       'gyro_ball_101','gyro_ball_cap','heavy_slam_heavy','heavy_slam_equal','heat_crash_heavy','hard_press_full','hard_press_quarter',
       'stored_power_3','stored_power_0','power_trip_3','foul_play_target_attack','body_press_defense','psyshock_defense','psystrike_defense',
       'freeze_dry_water','freeze_dry_water_ice','ice_beam_water','typecalc_restores_move','explosion_no_defense_halving']
hook='''
#include "new_game.h"
#include "pokemon.h"
#include "battle.h"
#include "battle_main.h"
#include "string_util.h"
#include "constants/moves.h"
#include "constants/items.h"
#include "constants/battle.h"
extern u8 AI_TypeCalc(u16 move, u16 targetSpecies, u8 targetAbility);
EWRAM_DATA volatile u32 gPowerDone = 0, gPowerPassed = 0, gPowerCount = 0, gPowerFailed = 0, gPowerFirst = 0;
static void Check(bool32 ok){if(ok)gPowerPassed++;else{if(!gPowerFailed)gPowerFirst=gPowerCount+1;gPowerFailed++;}gPowerCount++;}
static void Mon(struct BattlePokemon *mon, u16 species)
{
    u8 i;
    memset(mon, 0, sizeof(*mon));
    mon->species = species; mon->level = 50; mon->hp = mon->maxHP = 200;
    mon->attack = mon->defense = mon->speed = mon->spAttack = mon->spDefense = 100;
    for (i = 0; i < NUM_BATTLE_STATS; i++) mon->statStages[i] = DEFAULT_STAT_STAGE;
}
static s32 Dmg(struct BattlePokemon *a, struct BattlePokemon *d, u16 move, u16 power)
{
    gCritMultiplier = 1; gCurrentMove = move;
    return CalculateBaseDamage(a, d, move, 0, power, 0, 0, 1);
}
static bool32 SamePower(struct BattlePokemon *a, struct BattlePokemon *d, u16 move, u16 power)
{
    return Dmg(a, d, move, 0) == Dmg(a, d, move, power);
}
static void PowerIdle(void){}
static void PowerBoot(void)
{
    static const u8 name[] = _("TEST");
    struct BattlePokemon a, d, ref;
    u8 flags;
    SetSaveBlocksPointers();StringCopy(gSaveBlock2Ptr->playerName,name);StringCopy(gSaveBlock1Ptr->rivalName,name);NewGameInitData();
    gBattleTypeFlags = 0; gBattleWeather = 0; gBattlersCount = 2;
    Mon(&a, SPECIES_SNORLAX); Mon(&d, SPECIES_SLOWBRO);
    Check(SamePower(&a, &d, MOVE_HEX, 65));
    d.status1 = STATUS1_PARALYSIS; Check(SamePower(&a, &d, MOVE_HEX, 130));
    d.status1 = STATUS1_POISON; Check(SamePower(&a, &d, MOVE_VENOSHOCK, 130));
    d.status1 = STATUS1_PARALYSIS; Check(SamePower(&a, &d, MOVE_VENOSHOCK, 65)); d.status1 = 0;
    d.hp = 100; Check(SamePower(&a, &d, MOVE_BRINE, 130)); d.hp = 200; Check(SamePower(&a, &d, MOVE_BRINE, 65));
    Check(SamePower(&a, &d, MOVE_ACROBATICS, 110)); a.item = ITEM_LEFTOVERS; Check(SamePower(&a, &d, MOVE_ACROBATICS, 55)); a.item = ITEM_NONE;
    gProtectStructs[1].physicalDmg = 5; Check(SamePower(&a, &d, MOVE_ASSURANCE, 120));
    gProtectStructs[1].physicalDmg = 0; gProtectStructs[1].specialDmg = 0; Check(SamePower(&a, &d, MOVE_ASSURANCE, 60));
    gBattlerByTurnOrder[0] = 1; gBattlerByTurnOrder[1] = 0; Check(SamePower(&a, &d, MOVE_PAYBACK, 100));
    gBattlerByTurnOrder[0] = 0; gBattlerByTurnOrder[1] = 1; Check(SamePower(&a, &d, MOVE_PAYBACK, 50));
    a.speed = 400; d.speed = 100; Check(SamePower(&a, &d, MOVE_ELECTRO_BALL, 150));
    a.speed = 200; Check(SamePower(&a, &d, MOVE_ELECTRO_BALL, 80));
    a.speed = 100; d.speed = 150; Check(SamePower(&a, &d, MOVE_ELECTRO_BALL, 40));
    a.speed = 200; d.speed = 400; d.status1 = STATUS1_PARALYSIS; Check(SamePower(&a, &d, MOVE_ELECTRO_BALL, 80)); d.status1 = 0;
    a.speed = 50; d.speed = 200; Check(SamePower(&a, &d, MOVE_GYRO_BALL, 101));
    a.speed = 10; d.speed = 400; Check(SamePower(&a, &d, MOVE_GYRO_BALL, 150)); a.speed = d.speed = 100;
    d.species = SPECIES_PIKACHU; Check(SamePower(&a, &d, MOVE_HEAVY_SLAM, 120));
    d.species = SPECIES_SNORLAX; Check(SamePower(&a, &d, MOVE_HEAVY_SLAM, 40));
    d.species = SPECIES_PIKACHU; Check(SamePower(&a, &d, MOVE_HEAT_CRASH, 120)); d.species = SPECIES_SLOWBRO;
    Check(SamePower(&a, &d, MOVE_HARD_PRESS, 100)); d.hp = 50; Check(SamePower(&a, &d, MOVE_HARD_PRESS, 25)); d.hp = 200;
    a.statStages[STAT_ATK] = 8; a.statStages[STAT_SPATK] = 7; a.statStages[STAT_DEF] = 4;
    Check(SamePower(&a, &d, MOVE_STORED_POWER, 80));
    Mon(&a, SPECIES_SNORLAX); Check(SamePower(&a, &d, MOVE_STORED_POWER, 20));
    a.statStages[STAT_SPEED] = 9; Check(SamePower(&a, &d, MOVE_POWER_TRIP, 80)); Mon(&a, SPECIES_SNORLAX);
    a.attack = 50; d.attack = 300; d.statStages[STAT_ATK] = 8; ref = a; ref.attack = 300; ref.statStages[STAT_ATK] = 8;
    Check(Dmg(&a, &d, MOVE_FOUL_PLAY, 0) == Dmg(&ref, &d, MOVE_STRENGTH, 95)); Mon(&d, SPECIES_SLOWBRO);
    a.defense = 300; a.statStages[STAT_DEF] = 7; ref = a; ref.attack = 300; ref.statStages[STAT_ATK] = 7;
    Check(Dmg(&a, &d, MOVE_BODY_PRESS, 0) == Dmg(&ref, &d, MOVE_STRENGTH, 80)); Mon(&a, SPECIES_SNORLAX);
    d.defense = 50; d.spDefense = 300; d.statStages[STAT_DEF] = 5; ref = d; ref.spDefense = 50; ref.statStages[STAT_SPDEF] = 5;
    Check(Dmg(&a, &d, MOVE_PSYSHOCK, 0) == Dmg(&a, &ref, MOVE_PSYCHIC, 80));
    Check(Dmg(&a, &d, MOVE_PSYSTRIKE, 0) == Dmg(&a, &ref, MOVE_PSYCHIC, 100));
    gCurrentMove = MOVE_TACKLE;
    gBattleMoveDamage = 100; flags = AI_TypeCalc(MOVE_FREEZE_DRY, SPECIES_VAPOREON, 0); Check(gBattleMoveDamage == 200 && (flags & MOVE_RESULT_SUPER_EFFECTIVE));
    gBattleMoveDamage = 100; flags = AI_TypeCalc(MOVE_FREEZE_DRY, SPECIES_LAPRAS, 0); Check(gBattleMoveDamage == 100);
    gBattleMoveDamage = 100; flags = AI_TypeCalc(MOVE_ICE_BEAM, SPECIES_VAPOREON, 0); Check(gBattleMoveDamage == 50 && (flags & MOVE_RESULT_NOT_VERY_EFFECTIVE));
    Check(gCurrentMove == MOVE_TACKLE);
    Mon(&a, SPECIES_SNORLAX); Mon(&d, SPECIES_SLOWBRO);
    Check(Dmg(&a, &d, MOVE_EXPLOSION, 0) == Dmg(&a, &d, MOVE_STRENGTH, 250));
    gPowerDone = 1;
    SetMainCallback2(PowerIdle);
}
'''
fixture=Path(tempfile.mkdtemp(prefix='move-power-',dir=source.parent));shutil.copytree(source,fixture,dirs_exist_ok=True)
main=(source/'src/main.c').read_text()
(fixture/'src/main.c').write_text(main.replace('static void InitMainCallbacks(void)\n{',hook+'\nstatic void InitMainCallbacks(void)\n{').replace('SetMainCallback2(CB2_InitCopyrightScreenAfterBootup);','SetMainCallback2(PowerBoot);'))
with (OUT/'build.log').open('w') as log:subprocess.run(['make','-j4'],cwd=fixture,stdout=log,stderr=subprocess.STDOUT,check=True)
symbols={p[2]:p[0] for line in subprocess.check_output(['arm-none-eabi-nm','pokefirered.elf'],cwd=fixture,text=True).splitlines() if len(p:=line.split())==3}
runner=fixture/'native-checks';subprocess.run(['gcc','-O2','-w','-I',str(ROOT/'scripts'),str(ROOT/'scripts/native_checks_smoke.c'),'-o',str(runner),'-lmgba','-lpng'],check=True)
run=subprocess.run([str(runner),str(fixture/'pokefirered.gba'),*['0x'+symbols[s] for s in ('gPowerDone','gPowerPassed','gPowerCount','gPowerFailed','gPowerFirst')]],capture_output=True,text=True,timeout=120)
(OUT/'emulator.log').write_text(run.stdout+run.stderr)
fields=dict(part.split('=') for part in run.stdout.split())
passed=int(fields.get('passed',0));count=int(fields.get('checks',0))
assert count==len(NAMES),(count,len(NAMES),run.stdout)
first=int(fields.get('first_failure',0))
cases=[{'case':n,'status':'failed' if first==i+1 else ('passed' if passed==count else 'not isolated')} for i,n in enumerate(NAMES)]
(OUT/'results.json').write_text(json.dumps({'rom_sha256':reference,'cases':cases},indent=2)+'\n')
for c in cases:print(c['status'].upper(),c['case'])
if run.returncode:sys.exit(1)
