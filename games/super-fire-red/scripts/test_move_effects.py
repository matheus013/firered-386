"""Native wild battles that use one move on turn 1 and check the resulting battle state.

The player's controller always picks move slot 0 against the wild target, whose only move is Splash.
Each case reboots the same disposable ROM; results are read from memory after turn 1.
"""
from pathlib import Path
import hashlib,json,re,shutil,subprocess,tempfile,sys
ROOT=Path(__file__).resolve().parents[3];OUT=ROOT/'games/super-fire-red/output/tests/move-effects';OUT.mkdir(parents=True,exist_ok=True)
source=Path((ROOT/'games/super-fire-red/output/hack-build-path.txt').read_text().strip())
reference=hashlib.sha256((ROOT/'games/super-fire-red/output/firered-386-dev.gba').read_bytes()).hexdigest()
assert hashlib.sha256((source/'pokefirered.gba').read_bytes()).hexdigest()==reference
STAGES=['HP','ATK','DEF','SPEED','SPATK','SPDEF','ACC','EVASION']
# name, player species, level, move, player item, player HP %, target species, level, target item, seeds
CASES=[]
def case(name,move,check,player='MACHAMP',level=50,target='SNORLAX',tlevel=50,item='NONE',titem='NONE',hp=100,seeds=(1,),party2='NONE',tmove='SPLASH',thp=100,pstatus=0,move2='NONE',turns=1):
 CASES.append(dict(name=name,move=move,check=check,player=player,level=level,target=target,tlevel=tlevel,item=item,titem=titem,hp=hp,seeds=seeds,party2=party2,tmove=tmove,thp=thp,pstatus=pstatus,move2=move2,turns=turns))
dealt=lambda r:r['t_maxhp']-r['t_hp']
lost=lambda r:r['p_maxhp']-r['p_hp']
# Batch A controls
case('drain_punch_heals_half','DRAIN_PUNCH',lambda r:dealt(r)>0 and r['p_hp']-r['p_start']==max(1,dealt(r)//2),hp=10)
case('brave_bird_recoil_third','BRAVE_BIRD',lambda r:dealt(r)>0 and lost(r)==max(1,dealt(r)//3),player='STARAPTOR' if False else 'PIDGEOT')
case('draco_meteor_lowers_spatk_2','DRACO_METEOR',lambda r:r['p_SPATK']==4 and dealt(r)>0,player='SALAMENCE')
case('low_sweep_lowers_speed','LOW_SWEEP',lambda r:r['t_SPEED']==5 and dealt(r)>0)
case('moonblast_is_fairy_damage','MOONBLAST',lambda r:dealt(r)>0,player='CLEFABLE')
# Batch B1
case('close_combat_lowers_user_defenses','CLOSE_COMBAT',lambda r:r['p_DEF']==5 and r['p_SPDEF']==5 and dealt(r)>0,target='SLOWBRO')
case('hammer_arm_lowers_user_speed','HAMMER_ARM',lambda r:r['p_SPEED']==5 and dealt(r)>0,target='SLOWBRO')
case('flame_charge_raises_speed','FLAME_CHARGE',lambda r:r['p_SPEED']==7 and dealt(r)>0,player='RAPIDASH')
case('acid_spray_lowers_spdef_2','ACID_SPRAY',lambda r:r['t_SPDEF']==4 and dealt(r)>0,player='MUK')
case('head_smash_recoil_half','HEAD_SMASH',lambda r:dealt(r)==0 or lost(r)==max(1,dealt(r)//2),player='RHYDON',seeds=(1,2,3,4))
case('flare_blitz_recoil_third','FLARE_BLITZ',lambda r:dealt(r)>0 and lost(r)==max(1,dealt(r)//3),player='ARCANINE',seeds=(1,2,3))
case('frost_breath_always_crits','FROST_BREATH',lambda r:r['crit']==2 and dealt(r)>0,player='LAPRAS',seeds=(1,2,3,4))
case('storm_throw_always_crits','STORM_THROW',lambda r:r['crit']==2 and dealt(r)>0,target='SLOWBRO',seeds=(5,6))
case('draining_kiss_heals_three_quarters','DRAINING_KISS',lambda r:dealt(r)>0 and r['p_hp']-r['p_start']==max(1,dealt(r)*3//4),player='CLEFABLE',hp=10)
case('coil_raises_atk_def_acc','COIL',lambda r:(r['p_ATK'],r['p_DEF'],r['p_ACC'])==(7,7,7),player='ARBOK')
case('hone_claws_raises_atk_acc','HONE_CLAWS',lambda r:(r['p_ATK'],r['p_ACC'],r['p_DEF'])==(7,7,6),player='SNEASEL')
case('work_up_raises_atk_spatk','WORK_UP',lambda r:(r['p_ATK'],r['p_SPATK'])==(7,7),player='PERSIAN')
case('quiver_dance_raises_three','QUIVER_DANCE',lambda r:(r['p_SPATK'],r['p_SPDEF'],r['p_SPEED'])==(7,7,7),player='BUTTERFREE')
case('shell_smash_trades_defenses','SHELL_SMASH',lambda r:(r['p_DEF'],r['p_SPDEF'],r['p_ATK'],r['p_SPATK'],r['p_SPEED'])==(5,5,8,8,8),player='CLOYSTER')
case('cotton_guard_raises_def_3','COTTON_GUARD',lambda r:r['p_DEF']==9,player='JUMPLUFF')
case('noble_roar_lowers_atk_spatk','NOBLE_ROAR',lambda r:(r['t_ATK'],r['t_SPATK'])==(5,5),player='PERSIAN')
case('tearful_look_lowers_atk_spatk','TEARFUL_LOOK',lambda r:(r['t_ATK'],r['t_SPATK'])==(5,5),player='PERSIAN')
case('toxic_thread_poisons_and_slows','TOXIC_THREAD',lambda r:r['t_SPEED']==5 and r['t_status1']&8,player='ARIADOS',target='SLOWBRO')
case('toxic_thread_fails_on_steel','TOXIC_THREAD',lambda r:r['t_SPEED']==5 and r['t_status1']==0,player='ARIADOS',target='STEELIX')
case('play_nice_never_misses','PLAY_NICE',lambda r:r['t_ATK']==5,player='PERSIAN',seeds=(1,2,3))
case('switcheroo_swaps_items','SWITCHEROO',lambda r:r['p_item']=='ORAN_BERRY' and r['t_item']=='LEFTOVERS',player='KADABRA',item='LEFTOVERS',titem='ORAN_BERRY')
case('fire_fang_hits','FIRE_FANG',lambda r:True,player='ARCANINE',seeds=tuple(range(1,13)))
case('ice_fang_hits','ICE_FANG',lambda r:True,player='ARCANINE',seeds=tuple(range(20,26)))
case('thunder_fang_hits','THUNDER_FANG',lambda r:True,player='ARCANINE',seeds=tuple(range(30,36)))
case('charge_beam_damages','CHARGE_BEAM',lambda r:True,player='RAICHU',seeds=tuple(range(40,52)))
# Batch B2: each move runs in a real battle; power and stat sources are checked exactly in test_move_power.py.
for move,user in [('HEX','GENGAR'),('VENOSHOCK','MUK'),('BRINE','LAPRAS'),('ACROBATICS','PIDGEOT'),('ASSURANCE','HOUNDOOM'),('PAYBACK','HOUNDOOM'),
                  ('ELECTRO_BALL','ELECTRODE'),('GYRO_BALL','FORRETRESS'),('HEAVY_SLAM','STEELIX'),('HEAT_CRASH','MAGCARGO'),('HARD_PRESS','STEELIX'),
                  ('STORED_POWER','ALAKAZAM'),('POWER_TRIP','HOUNDOOM'),('FOUL_PLAY','HOUNDOOM'),('BODY_PRESS','STEELIX'),('PSYSHOCK','ALAKAZAM'),('PSYSTRIKE','MEWTWO')]:
 case(move.lower()+'_runs',move,lambda r:r['turns']>=1,player=user,target='SLOWBRO' if move=='HEX' else 'SNORLAX',tlevel=70,seeds=(1,2))
case('freeze_dry_vs_water','FREEZE_DRY',lambda r:dealt(r)>0,player='LAPRAS',target='VAPOREON',seeds=(7,))
case('ice_beam_vs_water','ICE_BEAM',lambda r:dealt(r)>0,player='LAPRAS',target='VAPOREON',seeds=(7,))
# Batch B3a
species=lambda n:SPECIES[n]
case('u_turn_switches_out','U_TURN',lambda r:dealt(r)>0 and r['p_species']==species('JOLTEON'),player='SCIZOR',party2='JOLTEON')
case('volt_switch_alone_stays','VOLT_SWITCH',lambda r:dealt(r)>0 and r['p_species']==species('RAICHU'),player='RAICHU')
case('dragon_tail_ends_wild_battle','DRAGON_TAIL',lambda r:r['outcome']!=0 and r['t_hp']<r['t_start'],player='DRAGONITE')
case('feint_breaks_protect','FEINT',lambda r:dealt(r)>0,player='PERSIAN',tmove='PROTECT')
case('tackle_blocked_by_protect','TACKLE',lambda r:dealt(r)==0,player='PERSIAN',tmove='PROTECT')
case('sucker_punch_fails_vs_status','SUCKER_PUNCH',lambda r:dealt(r)==0,player='HOUNDOOM')
case('sucker_punch_hits_vs_attack','SUCKER_PUNCH',lambda r:dealt(r)>0,player='HOUNDOOM',tmove='TACKLE')
case('upper_hand_vs_priority','UPPER_HAND',lambda r:dealt(r)>0 and r['p_hp']==r['p_maxhp'],player='HITMONTOP',tmove='QUICK_ATTACK')
case('upper_hand_fails_vs_normal','UPPER_HAND',lambda r:dealt(r)==0,player='HITMONTOP',tmove='TACKLE')
case('clear_smog_resets_stages','CLEAR_SMOG',lambda r:r['t_ATK']==6 and dealt(r)>0,player='MUK',target='JOLTEON',tmove='SWORDS_DANCE')
case('tackle_keeps_stages','TACKLE',lambda r:r['t_ATK']==8,player='MUK',target='JOLTEON',tmove='SWORDS_DANCE')
case('life_dew_heals_quarter','LIFE_DEW',lambda r:r['p_hp']-r['p_start']==r['p_maxhp']//4,player='LAPRAS',hp=50)
case('heal_pulse_heals_target_half','HEAL_PULSE',lambda r:r['t_hp']-r['t_start']==r['t_maxhp']//2,player='ALAKAZAM',thp=40)
case('rage_powder_runs','RAGE_POWDER',lambda r:r['turns']>=1,player='BUTTERFREE')
case('pollen_puff_damages_foe','POLLEN_PUFF',lambda r:dealt(r)>0,player='BUTTERFREE')
# Batch B3b
TYPE={n:int(v) for n,v in re.findall(r'^#define TYPE_(\w+)\s+(\d+)',(ROOT/'games/super-fire-red/include/constants/pokemon.h').read_text(),re.M)}
ABILITY={n:int(v) for n,v in re.findall(r'^#define ABILITY_(\w+)\s+(\d+)',(ROOT/'games/super-fire-red/include/constants/abilities.h').read_text(),re.M)}
case('infestation_traps','INFESTATION',lambda r:dealt(r)>0 and r['t_status2']&0xE000,player='BUTTERFREE')
case('meteor_beam_charges_spatk','METEOR_BEAM',lambda r:r['p_SPATK']==7 and dealt(r)==0,player='GOLEM')
case('fell_stinger_ko_raises_attack','FELL_STINGER',lambda r:r['outcome']!=0 and r['p_ATK']==9,player='BEEDRILL',thp=1)
case('steel_beam_costs_half_hp','STEEL_BEAM',lambda r:lost(r)==(r['p_maxhp']+1)//2,player='STEELIX',seeds=(1,2))
case('final_gambit_hp_damage','FINAL_GAMBIT',lambda r:r['outcome']!=0 and r['p_hp']==0 and r['t_start']-r['t_hp']==r['p_start'],player='MACHAMP',hp=40)
case('metal_burst_returns_1_5x','METAL_BURST',lambda r:lost(r)>0 and dealt(r)==lost(r)*3//2,player='STEELIX',target='JOLTEON',tmove='TACKLE')
case('triple_axel_hits','TRIPLE_AXEL',lambda r:True,player='SNEASEL',seeds=(1,2,3,4))
case('axe_kick_crash_on_ghost','AXE_KICK',lambda r:lost(r)==r['p_maxhp']//2 and dealt(r)==0,player='HITMONLEE',target='GENGAR')
case('hi_jump_kick_modern_crash','HI_JUMP_KICK',lambda r:lost(r)==r['p_maxhp']//2 and dealt(r)==0,player='HITMONLEE',target='GENGAR')
case('supercell_slam_crash_on_ground','SUPERCELL_SLAM',lambda r:lost(r)==r['p_maxhp']//2 and dealt(r)==0,player='RAICHU',target='GOLEM')
case('power_trick_swaps_atk_def','POWER_TRICK',lambda r:r['p_attack']<r['p_defense'],player='MACHAMP')
case('power_swap_takes_boost','POWER_SWAP',lambda r:r['p_ATK']==8 and r['t_ATK']==6,player='ALAKAZAM',target='JOLTEON',tmove='SWORDS_DANCE')
case('guard_swap_takes_boost','GUARD_SWAP',lambda r:r['p_DEF']==8 and r['t_DEF']==6,player='ALAKAZAM',target='JOLTEON',tmove='IRON_DEFENSE')
case('power_split_runs','POWER_SPLIT',lambda r:r['turns']>=1,player='ALAKAZAM')
case('guard_split_runs','GUARD_SPLIT',lambda r:r['turns']>=1,player='ALAKAZAM')
case('splash_reference','SPLASH',lambda r:True,player='ALAKAZAM')
case('soak_makes_water','SOAK',lambda r:r['t_types']==(TYPE['WATER'],TYPE['WATER']),player='LAPRAS')
case('reflect_type_copies','REFLECT_TYPE',lambda r:r['p_types']==r['t_types'],player='PORYGON2')
case('worry_seed_insomnia','WORRY_SEED',lambda r:r['t_ability']==ABILITY['INSOMNIA'],player='BRELOOM')
case('entrainment_copies_ability','ENTRAINMENT',lambda r:r['t_ability']==r['p_ability'],player='PERSIAN')
case('gastro_acid_suppresses','GASTRO_ACID',lambda r:r['t_ability']==0,player='MUK')
case('psycho_shift_moves_poison','PSYCHO_SHIFT',lambda r:r['t_status1']&8 and not r['p_status1'],player='ALAKAZAM',target='SLOWBRO',pstatus=8)
case('acupressure_raises_one_stat_by_2','ACUPRESSURE',lambda r:sum(r['p_'+s]-6 for s in ('ATK','DEF','SPEED','SPATK','SPDEF','ACC','EVASION'))==2,player='MACHAMP',seeds=(1,2,3))
# Batch B4a: the effect starts on turn 1 and is observed on turn 2
case('trick_room_slow_moves_first','TRICK_ROOM',lambda r:r['first']==0 and r['trick_room']>0,player='SNORLAX',target='JOLTEON',tmove='TACKLE',move2='TACKLE',turns=2)
case('no_trick_room_fast_moves_first','SPLASH',lambda r:r['first']==1,player='SNORLAX',target='JOLTEON',tmove='TACKLE',move2='TACKLE',turns=2)
case('tailwind_doubles_speed','TAILWIND',lambda r:r['first']==0,player='SNORLAX',target='GOLEM',tmove='TACKLE',move2='TACKLE',turns=2)
case('electric_terrain_blocks_sleep','ELECTRIC_TERRAIN',lambda r:r['terrain']==1 and not r['p_status1']&7,player='JOLTEON',target='PARAS',tmove='SPORE')
case('misty_terrain_blocks_status','MISTY_TERRAIN',lambda r:r['terrain']==3 and not r['p_status1'],player='JOLTEON',target='MUK',tmove='TOXIC')
case('grassy_terrain_heals','GRASSY_TERRAIN',lambda r:r['terrain']==2 and r['p_hp']-r['p_start']==r['p_maxhp']//16,player='SCEPTILE',hp=50)
case('psychic_terrain_blocks_priority','PSYCHIC_TERRAIN',lambda r:r['terrain']==4,player='ALAKAZAM',target='RATTATA',tmove='QUICK_ATTACK',move2='SPLASH',turns=2,seeds=(3,))
case('psychic_terrain_control','SPLASH',lambda r:True,player='ALAKAZAM',target='RATTATA',tmove='QUICK_ATTACK',move2='SPLASH',turns=2,seeds=(3,))
case('gravity_grounds_flying','GRAVITY',lambda r:dealt(r)>0,player='DUGTRIO',target='PIDGEOT',move2='EARTHQUAKE',turns=2)
case('magic_room_stops_leftovers','MAGIC_ROOM',lambda r:r['t_hp']==r['t_start'],player='ALAKAZAM',titem='LEFTOVERS',thp=50)
case('leftovers_control','SPLASH',lambda r:r['t_hp']>r['t_start'],player='ALAKAZAM',titem='LEFTOVERS',thp=50)
case('wonder_room_tackle','WONDER_ROOM',lambda r:dealt(r)>0,player='SNORLAX',target='CLOYSTER',move2='TACKLE',turns=2,seeds=(5,))
case('wonder_room_control','SPLASH',lambda r:dealt(r)>0,player='SNORLAX',target='CLOYSTER',move2='TACKLE',turns=2,seeds=(5,))
case('snowscape_no_damage','SNOWSCAPE',lambda r:r['t_hp']==r['t_maxhp'] and r['p_hp']==r['p_maxhp'],player='DEWGONG')
case('hail_control_damages','HAIL',lambda r:r['t_hp']<r['t_maxhp'],player='DEWGONG')
case('grassy_glide_priority','GRASSY_TERRAIN',lambda r:r['first']==0,player='SNORLAX',target='JOLTEON',tmove='TACKLE',move2='GRASSY_GLIDE',turns=2)
case('expanding_force_terrain','PSYCHIC_TERRAIN',lambda r:dealt(r)>0,player='ALAKAZAM',move2='EXPANDING_FORCE',turns=2,seeds=(6,))
case('expanding_force_control','SPLASH',lambda r:dealt(r)>0,player='ALAKAZAM',move2='EXPANDING_FORCE',turns=2,seeds=(6,))
case('ice_spinner_clears_terrain','GRASSY_TERRAIN',lambda r:r['terrain']==0 and dealt(r)>0,player='SNEASEL',move2='ICE_SPINNER',turns=2)
# Batch B4b: the target sets a hazard on turn 1; on turn 2 the player switches in its partner with U-turn
case('stealth_rock_hurts_rock_weak','SPLASH',lambda r:r['p_species']==species('CHARIZARD') and r['p_hp']==r['p_maxhp']-r['p_maxhp']//2,player='SCIZOR',party2='CHARIZARD',tmove='STEALTH_ROCK',move2='U_TURN',turns=2)
case('stealth_rock_resisted','SPLASH',lambda r:r['p_species']==species('STEELIX') and r['p_hp']==r['p_maxhp']-max(1,r['p_maxhp']//32),player='SCIZOR',party2='STEELIX',tmove='STEALTH_ROCK',move2='U_TURN',turns=2)
case('toxic_spikes_poison_switch_in','SPLASH',lambda r:r['p_species']==species('MACHAMP') and r['p_status1']&8,player='SCIZOR',party2='MACHAMP',tmove='TOXIC_SPIKES',move2='U_TURN',turns=2)
case('toxic_spikes_absorbed_by_poison','SPLASH',lambda r:r['p_species']==species('MUK') and r['p_status1']==0 and not r['tspikes'],player='SCIZOR',party2='MUK',target='JOLTEON',tmove='TOXIC_SPIKES',move2='U_TURN',turns=2)
case('toxic_spikes_skip_flying','SPLASH',lambda r:r['p_species']==species('PIDGEOT') and r['p_status1']==0,player='SCIZOR',party2='PIDGEOT',tmove='TOXIC_SPIKES',move2='U_TURN',turns=2)
case('sticky_web_slows_switch_in','SPLASH',lambda r:r['p_species']==species('SNORLAX') and r['p_SPEED']==5,player='SCIZOR',party2='SNORLAX',tmove='STICKY_WEB',move2='U_TURN',turns=2)
case('defog_clears_hazards','SPLASH',lambda r:not r['rocks'] and r['t_EVASION']==5,player='PIDGEOT',target='JOLTEON',tmove='STEALTH_ROCK',move2='DEFOG',turns=2)
case('rocks_stay_without_defog','SPLASH',lambda r:r['rocks']==1,player='PIDGEOT',tmove='STEALTH_ROCK',move2='SPLASH',turns=2)
case('rapid_spin_clears_hazards','SPLASH',lambda r:not r['rocks'],player='FORRETRESS',target='JOLTEON',tmove='STEALTH_ROCK',move2='RAPID_SPIN',turns=2)
PAIRS={'freeze_dry_super_effective_on_water':lambda R:dealt(R['freeze_dry_vs_water'][0])>2*dealt(R['ice_beam_vs_water'][0]),
       'power_split_changes_attack':lambda R:R['power_split_runs'][0]['p_attack']!=R['splash_reference'][0]['p_attack'],
       'guard_split_changes_defense':lambda R:R['guard_split_runs'][0]['p_defense']!=R['splash_reference'][0]['p_defense'],
       'psychic_terrain_halves_priority_damage':lambda R:lost(R['psychic_terrain_blocks_priority'][0])<lost(R['psychic_terrain_control'][0]),
       'wonder_room_swaps_defenses':lambda R:dealt(R['wonder_room_tackle'][0])>dealt(R['wonder_room_control'][0]),
       'expanding_force_stronger_on_terrain':lambda R:dealt(R['expanding_force_terrain'][0])*10>dealt(R['expanding_force_control'][0])*17}
# Statistical checks over the seeds of one case (chance effects must show up at least once).
AGGREGATE={'fire_fang_hits':lambda rs:any(r['t_status1']&16 for r in rs) and sum(dealt(r)>0 for r in rs)>=len(rs)*3//4,
 'ice_fang_hits':lambda rs:any(dealt(r)>0 for r in rs),'thunder_fang_hits':lambda rs:any(dealt(r)>0 for r in rs),'triple_axel_hits':lambda rs:any(dealt(r)>0 for r in rs),'charge_beam_damages':lambda rs:any(dealt(r)>0 and r['p_SPATK']==7 for r in rs) and all(r['p_SPATK']==6 for r in rs if dealt(r)==0),
 'head_smash_recoil_half':lambda rs:any(dealt(r)>0 for r in rs)}

rows=[]
for c in CASES:
 for seed in c['seeds']:rows.append((c,seed))
table=',\n'.join(f'    {{SPECIES_{c["player"]}, {c["level"]}, MOVE_{c["move"]}, ITEM_{c["item"]}, {c["hp"]}, SPECIES_{c["target"]}, {c["tlevel"]}, ITEM_{c["titem"]}, SPECIES_{c["party2"]}, MOVE_{c["tmove"]}, {c["thp"]}, MOVE_{c["move2"]}, {c["turns"]}, {c["pstatus"]}, {seed}}}' for c,seed in rows)
hook='''
#include "new_game.h"
#include "pokemon.h"
#include "battle.h"
#include "battle_main.h"
#include "overworld.h"
#include "random.h"
#include "string_util.h"
#include "nuzlocke.h"
#include "game_settings.h"
#include "constants/maps.h"
#include "constants/moves.h"
#include "constants/items.h"
#include "modern_battle.h"
extern void CB2_InitBattle(void);
struct MoveTestCase {u16 species, level, move, item, hpPercent, targetSpecies, targetLevel, targetItem, partner, targetMove, targetHpPercent, move2, turns; u32 status, seed;};
static const struct MoveTestCase sMoveTestCases[] = {
TABLE
};
EWRAM_DATA volatile u32 gMoveTestCase = 0, gMoveTestDone = 0, gMoveTestOut[16] = {0};
EWRAM_DATA volatile u32 gMoveTestCrit = 0;
EWRAM_DATA volatile u32 gMoveTestSecondSlot = 0, gMoveTestTurns = 1;
static void MoveTestIdle(void) {}
static void MoveTestOnlyMove(struct Pokemon *mon, u16 move)
{
    u16 none = MOVE_NONE; u8 i;
    for (i = 0; i < MAX_MON_MOVES; i++) SetMonData(mon, MON_DATA_MOVE1 + i, &none);
    SetMonMoveSlot(mon, move, 0);
}
static void MoveTestBoot(void)
{
    static const u8 name[] = _("TEST");
    const struct MoveTestCase *c;
    u16 hp;
    if (gMoveTestCase == 0) return;
    c = &sMoveTestCases[gMoveTestCase - 1];
    SetSaveBlocksPointers();StringCopy(gSaveBlock2Ptr->playerName,name);StringCopy(gSaveBlock1Ptr->rivalName,name);NewGameInitData();
    Nuzlocke_ChooseMode(GAME_MODE_NORMAL);
    gSaveBlock2Ptr->optionsTextSpeed = 2; gSaveBlock2Ptr->optionsBattleSceneOff = TRUE;
    gMapHeader = *Overworld_GetMapHeaderByGroupAndId(MAP_GROUP(MAP_ROUTE1), MAP_NUM(MAP_ROUTE1));
    ZeroPlayerPartyMons(); ZeroEnemyPartyMons();
    CreateMon(&gPlayerParty[0], c->species, c->level, 31, TRUE, 0, OT_ID_PLAYER_ID, 0);
    MoveTestOnlyMove(&gPlayerParty[0], c->move);
    if (c->move2 != MOVE_NONE) SetMonMoveSlot(&gPlayerParty[0], c->move2, 1);
    gMoveTestSecondSlot = c->move2 != MOVE_NONE; gMoveTestTurns = c->turns;
    SetMonData(&gPlayerParty[0], MON_DATA_HELD_ITEM, &c->item);
    hp = GetMonData(&gPlayerParty[0], MON_DATA_MAX_HP) * c->hpPercent / 100; if (hp == 0) hp = 1;
    SetMonData(&gPlayerParty[0], MON_DATA_HP, &hp);
    SetMonData(&gPlayerParty[0], MON_DATA_STATUS, &c->status);
    gMoveTestOut[11] = hp;
    if (c->partner != SPECIES_NONE)
    {
        CreateMon(&gPlayerParty[1], c->partner, c->level, 31, TRUE, 0, OT_ID_PLAYER_ID, 0);
        MoveTestOnlyMove(&gPlayerParty[1], MOVE_SPLASH);
    }
    CalculatePlayerPartyCount();
    CreateMon(&gEnemyParty[0], c->targetSpecies, c->targetLevel, 31, TRUE, 0, OT_ID_PLAYER_ID, 0);
    MoveTestOnlyMove(&gEnemyParty[0], c->targetMove);
    SetMonData(&gEnemyParty[0], MON_DATA_HELD_ITEM, &c->targetItem);
    hp = GetMonData(&gEnemyParty[0], MON_DATA_MAX_HP) * c->targetHpPercent / 100; if (hp == 0) hp = 1;
    SetMonData(&gEnemyParty[0], MON_DATA_HP, &hp);
    gMoveTestOut[13] = hp;
    SeedRng(c->seed);
    gBattleTypeFlags = 0; gMain.savedCallback = MoveTestIdle; SetMainCallback2(CB2_InitBattle);
}
static u32 MoveTestStages(u8 battler)
{
    u32 packed = 0; u8 i;
    for (i = 0; i < NUM_BATTLE_STATS; i++) packed |= (u32)(gBattleMons[battler].statStages[i] & 0xF) << (4 * i);
    return packed;
}
void MoveTestRecord(void)
{
    gMoveTestOut[0] = gBattleMons[0].hp | (gBattleMons[0].maxHP << 16);
    gMoveTestOut[1] = gBattleMons[1].hp | (gBattleMons[1].maxHP << 16);
    gMoveTestOut[2] = MoveTestStages(0); gMoveTestOut[3] = MoveTestStages(1);
    gMoveTestOut[4] = gBattleMons[0].status1; gMoveTestOut[5] = gBattleMons[1].status1;
    gMoveTestOut[6] = gBattleMons[0].status2; gMoveTestOut[7] = gBattleMons[1].status2;
    gMoveTestOut[8] = gMoveTestCrit;
    gMoveTestOut[9] = gBattleMons[0].item | (gBattleMons[1].item << 16);
    gMoveTestOut[10] = gBattleResults.battleTurnCounter | (gBattleMons[0].ability << 8) | (gBattleMons[1].ability << 16);
    gMoveTestOut[12] = gBattleOutcome | (gBattleMons[1].type1 << 8) | (gBattleMons[1].type2 << 16) | (gBattleMons[0].type1 << 24);
    gMoveTestOut[13] = (gMoveTestOut[13] & 0xFFFF) | (gBattleMons[0].type2 << 16);
    gMoveTestOut[15] = gBattleMons[0].attack | (gBattleMons[0].defense << 16);
    gMoveTestOut[11] = (gMoveTestOut[11] & 0xFFFF) | (gBattlerByTurnOrder[0] << 16);
    if (gModernBattle != NULL) gMoveTestOut[11] |= (gModernBattle->terrain << 20) | (gModernBattle->trickRoomTimer << 24)
        | (gModernBattle->stealthRock[0] << 28) | ((gModernBattle->toxicSpikes[0] != 0) << 29) | (gModernBattle->stickyWeb[0] << 30);
    gMoveTestOut[14] = gBattleMons[0].species | (gBattleMons[1].species << 16);
    gMoveTestDone = gMoveTestCase;
}
'''.replace('TABLE',table)
fixture=Path(tempfile.mkdtemp(prefix='move-effects-',dir=source.parent));shutil.copytree(source,fixture,dirs_exist_ok=True)
main=(source/'src/main.c').read_text()
main=main.replace('static void InitMainCallbacks(void)\n{',hook+'\nstatic void InitMainCallbacks(void)\n{').replace('SetMainCallback2(CB2_InitCopyrightScreenAfterBootup);','SetMainCallback2(MoveTestBoot);')
main=main.replace('#include "global.h"','#include "global.h"\n#include "battle.h"\nextern void HandleTurnActionSelectionState(void);\nextern volatile u32 gMoveTestCase, gMoveTestDone, gMoveTestTurns;\nextern void MoveTestRecord(void);',1)
main=main.replace('    ReadKeys();','    if (gMoveTestCase && gMoveTestDone != gMoveTestCase && ((gMain.inBattle && gBattleMainFunc == HandleTurnActionSelectionState && gBattleResults.battleTurnCounter >= gMoveTestTurns) || (gBattleOutcome && !gMain.inBattle))) MoveTestRecord();\n    ReadKeys();')
(fixture/'src/main.c').write_text(main)
battle_main=(source/'src/battle_main.c').read_text().replace('static void HandleTurnActionSelectionState(void)','void HandleTurnActionSelectionState(void)')
# Wild battles normally give the species' held item; cases set the target item themselves.
assert battle_main.count('        SetWildMonHeldItem();')==1
battle_main=battle_main.replace('        SetWildMonHeldItem();','        { extern volatile u32 gMoveTestCase; if (!gMoveTestCase) SetWildMonHeldItem(); }')
(fixture/'src/battle_main.c').write_text(battle_main)
# The crit multiplier is reset before every action, so record the player's own critcalc result.
commands=(source/'src/battle_script_commands.c').read_text()
anchor='    else\n        gCritMultiplier = 1;\n'
assert commands.count(anchor)==1
commands=commands.replace(anchor,anchor+'    if (gBattlerAttacker == 0) { extern volatile u32 gMoveTestCrit; gMoveTestCrit = gCritMultiplier; }\n')
(fixture/'src/battle_script_commands.c').write_text(commands)
player=(source/'src/battle_controller_player.c').read_text()
auto='''static void PlayerBufferExecCompleted(void);
static void MoveTestChooseAction(void){BtlController_EmitTwoReturnValues(1,B_ACTION_USE_MOVE,0);PlayerBufferExecCompleted();}
extern volatile u32 gMoveTestSecondSlot;
static void MoveTestChooseMove(void){BtlController_EmitTwoReturnValues(1,10,(gBattleResults.battleTurnCounter>=1?gMoveTestSecondSlot:0)|(1<<8));PlayerBufferExecCompleted();}
static void MoveTestChoosePokemon(void){BtlController_EmitChosenMonReturnValue(1,gBattlerPartyIndexes[gActiveBattler]==0?1:0,NULL);PlayerBufferExecCompleted();}
'''
player=player.replace('static void PlayerHandleGetMonData(void);',auto+'static void PlayerHandleGetMonData(void);',1)
for original,replacement in [('PlayerHandleChooseAction','MoveTestChooseAction'),('PlayerHandleChooseMove','MoveTestChooseMove'),('PlayerHandleChoosePokemon','MoveTestChoosePokemon')]:
 start=player.index('static void '+original+'(void)\n{');opening=player.index('{',start);depth=1;end=opening+1
 while depth:depth+=(player[end]=='{')-(player[end]=='}');end+=1
 player=player[:start]+'static void '+original+'(void)\n{\n    '+replacement+'();\n}'+player[end:]
(fixture/'src/battle_controller_player.c').write_text(player)
with (OUT/'build.log').open('w') as log:subprocess.run(['make','-j4'],cwd=fixture,stdout=log,stderr=subprocess.STDOUT,check=True)
symbols={p[2]:p[0] for line in subprocess.check_output(['arm-none-eabi-nm','pokefirered.elf'],cwd=fixture,text=True).splitlines() if len(p:=line.split())==3}
runner=fixture/'move-effects';subprocess.run(['gcc','-O2','-w','-I',str(ROOT/'games/super-fire-red/scripts'),str(ROOT/'games/super-fire-red/scripts/move_effects_smoke.c'),'-o',str(runner),'-lmgba','-lpng'],check=True)
run=subprocess.run([str(runner),str(fixture/'pokefirered.gba'),str(len(rows)),*['0x'+symbols[s] for s in ('gMoveTestCase','gMoveTestDone','gMoveTestOut')]],capture_output=True,text=True,timeout=1800)
(OUT/'emulator.log').write_text(run.stdout+run.stderr)
items={}
SPECIES={n:int(v) for n,v in re.findall(r'^#define SPECIES_(\w+)\s+(\d+)',(ROOT/'games/super-fire-red/include/constants/species.h').read_text(),re.M)}
for n,v in re.findall(r'^#define ITEM_(\w+)\s+(\d+)',(ROOT/'games/super-fire-red/include/constants/items.h').read_text(),re.M):items.setdefault(int(v),n)
parsed=[]
for line in run.stdout.splitlines():
 m=re.match(r'case=(\d+) done=(\d+) frames=(\d+) out=(.*)',line)
 if not m:continue
 o=list(map(int,m[4].split(',')));r={'done':int(m[2])==int(m[1]),'frames':int(m[3])}
 for side,hpword,stages,status1,status2 in (('p',o[0],o[2],o[4],o[6]),('t',o[1],o[3],o[5],o[7])):
  r[side+'_hp']=hpword&0xFFFF;r[side+'_maxhp']=hpword>>16;r[side+'_status1']=status1;r[side+'_status2']=status2
  for i,stat in enumerate(STAGES):r[f'{side}_{stat}']=(stages>>(4*i))&0xF
 r['crit']=o[8];r['p_item']=items.get(o[9]&0xFFFF,o[9]&0xFFFF);r['t_item']=items.get(o[9]>>16,o[9]>>16);r['turns']=o[10]&0xFF;r['p_ability']=(o[10]>>8)&0xFF;r['t_ability']=(o[10]>>16)&0xFF;r['p_start']=o[11]&0xFFFF;r['first']=(o[11]>>16)&0xF;r['terrain']=(o[11]>>20)&0xF;r['trick_room']=(o[11]>>24)&0xF;r['rocks']=(o[11]>>28)&1;r['tspikes']=(o[11]>>29)&1;r['web']=(o[11]>>30)&1;r['outcome']=o[12]&0xFF;r['t_types']=((o[12]>>8)&0xFF,(o[12]>>16)&0xFF);r['p_types']=(o[12]>>24,o[13]>>16);r['t_start']=o[13]&0xFFFF;r['p_attack']=o[15]&0xFFFF;r['p_defense']=o[15]>>16;r['p_species']=o[14]&0xFFFF;r['t_species']=o[14]>>16
 parsed.append(r)
assert len(parsed)==len(rows),run.stdout[-2000:]+run.stderr[-2000:]
results=[];failed=0
for name in dict.fromkeys(c['name'] for c in CASES):
 group=[(c,r) for (c,seed),r in zip(rows,parsed) if c['name']==name]
 ok=all(r['done'] and c['check'](r) for c,r in group)
 if name in AGGREGATE:ok=ok and AGGREGATE[name]([r for _,r in group])
 failed+=not ok
 results.append({'case':name,'move':group[0][0]['move'],'status':'passed' if ok else 'failed','runs':[r for _,r in group]})
 print(('PASS ' if ok else 'FAIL ')+name,flush=True)
by_name={res['case']:res['runs'] for res in results}
for name,check in PAIRS.items():
 ok=check(by_name);failed+=not ok;results.append({'case':name,'status':'passed' if ok else 'failed'});print(('PASS ' if ok else 'FAIL ')+name,flush=True)
(OUT/'results.json').write_text(json.dumps({'rom_sha256':reference,'cases':results},indent=2)+'\n')
if failed:sys.exit(1)
