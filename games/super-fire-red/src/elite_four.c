#include "global.h"
#include "pokemon.h"
#include "battle.h"
#include "elite_four.h"
#include "constants/opponents.h"
#include "constants/pokemon.h"
#include "constants/trainers.h"
#include "constants/moves.h"

bool8 EliteFour_IsChallenge(u16 trainer)
{
    u8 trainerClass = gTrainers[trainer].trainerClass;
    if (trainerClass == TRAINER_CLASS_LEADER || trainerClass == TRAINER_CLASS_ELITE_FOUR
        || trainerClass == TRAINER_CLASS_CHAMPION || trainerClass == TRAINER_CLASS_RIVAL_EARLY
        || trainerClass == TRAINER_CLASS_RIVAL_LATE || trainerClass == TRAINER_CLASS_BOSS)
        return TRUE;
    switch (trainer)
    {
    case TRAINER_BLACK_BELT_KOICHI:
    case TRAINER_BIKER_GOON: case TRAINER_BIKER_GOON_2: case TRAINER_BIKER_GOON_3:
    case TRAINER_CUE_BALL_PAXTON:
    case TRAINER_TEAM_ROCKET_ADMIN: case TRAINER_TEAM_ROCKET_ADMIN_2:
    case TRAINER_SCIENTIST_GIDEON:
        return TRUE;
    }
    return FALSE;
}

static void ApplyRoleTraining(struct Pokemon *mon)
{
    u8 i,stat,evs[NUM_STATS] = {0};
    u16 species = GetMonData(mon, MON_DATA_SPECIES);
    u16 physical = 0, special = 0, nature;
    bool8 fast = gSpeciesInfo[species].baseSpeed >= 85 || species == SPECIES_SHEDINJA;
    for (i = 0; i < MAX_MON_MOVES; i++)
    {
        u16 move = GetMonData(mon, MON_DATA_MOVE1 + i);
        // Hidden Power is special and has fixed modern base power 60.
        if (move == MOVE_HIDDEN_POWER) { special += 60; continue; }
        if (gBattleMoves[move].power <= 1) continue;
        if (IS_MOVE_PHYSICAL(move)) physical += gBattleMoves[move].power;
        else special += gBattleMoves[move].power;
    }
    if (physical == 0 && special == 0)
    {
        evs[STAT_HP] = 252;
        evs[STAT_DEF] = 128;
        evs[STAT_SPDEF] = 128;
        nature = 0xA500 | (gSpeciesInfo[species].baseDefense < gSpeciesInfo[species].baseSpDefense ? NATURE_BOLD : NATURE_CALM);
        SetMonData(mon, MON_DATA_TRAINING_NATURE, &nature);
        for (i = 0; i < NUM_STATS; i++) SetMonData(mon, MON_DATA_HP_EV + i, &evs[i]);
        return;
    }
    if (physical == special)
    {
        physical = gSpeciesInfo[species].baseAttack;
        special = gSpeciesInfo[species].baseSpAttack;
    }
    stat = physical > special ? STAT_ATK : STAT_SPATK;
    evs[stat] = 252;
    evs[fast ? STAT_SPEED : STAT_HP] = 252;
    evs[species == SPECIES_SHEDINJA ? STAT_SPDEF : fast ? STAT_HP : STAT_DEF] = 4;
    nature = 0xA500 | (stat == STAT_ATK ? (fast ? NATURE_JOLLY : NATURE_ADAMANT) : (fast ? NATURE_TIMID : NATURE_MODEST));
    SetMonData(mon, MON_DATA_TRAINING_NATURE, &nature);
    for (i = 0; i < NUM_STATS; i++) SetMonData(mon, MON_DATA_HP_EV + i, &evs[i]);
}

struct EliteFourTraining
{
    u16 species;
    u8 nature;
    u8 abilityNum;
    u8 evs[NUM_STATS];
};

#include "data/elite_four_sets.h"

static const struct EliteFourTraining *GetTraining(u16 trainer, u8 slot)
{
    u8 index;
    if (slot >= PARTY_SIZE)
        return NULL;
    switch (trainer)
    {
    case TRAINER_LEADER_BROCK: return &sGymTraining[0][slot];
    case TRAINER_LEADER_MISTY: return &sGymTraining[1][slot];
    case TRAINER_LEADER_LT_SURGE: return &sGymTraining[2][slot];
    case TRAINER_LEADER_ERIKA: return &sGymTraining[3][slot];
    case TRAINER_LEADER_KOGA: return &sGymTraining[4][slot];
    case TRAINER_LEADER_SABRINA: return &sGymTraining[5][slot];
    case TRAINER_LEADER_BLAINE: return &sGymTraining[6][slot];
    case TRAINER_LEADER_GIOVANNI: return &sGymTraining[7][slot];
    case TRAINER_ELITE_FOUR_LORELEI:
    case TRAINER_ELITE_FOUR_LORELEI_2:
        index = 0;
        break;
    case TRAINER_ELITE_FOUR_BRUNO:
    case TRAINER_ELITE_FOUR_BRUNO_2:
        index = 1;
        break;
    case TRAINER_ELITE_FOUR_AGATHA:
    case TRAINER_ELITE_FOUR_AGATHA_2:
        index = 2;
        break;
    case TRAINER_ELITE_FOUR_LANCE:
    case TRAINER_ELITE_FOUR_LANCE_2:
        index = 3;
        break;
    case TRAINER_CHAMPION_FIRST_SQUIRTLE:
        return &sChampionTraining[0][slot];
    case TRAINER_CHAMPION_FIRST_BULBASAUR:
        return &sChampionTraining[1][slot];
    case TRAINER_CHAMPION_FIRST_CHARMANDER:
        return &sChampionTraining[2][slot];
    case TRAINER_CHAMPION_REMATCH_SQUIRTLE:
        return &sChampionRematchTraining[0][slot];
    case TRAINER_CHAMPION_REMATCH_BULBASAUR:
        return &sChampionRematchTraining[1][slot];
    case TRAINER_CHAMPION_REMATCH_CHARMANDER:
        return &sChampionRematchTraining[2][slot];
    default:
        return NULL;
    }
    return &sEliteFourTraining[index][slot];
}

u32 EliteFour_GetPersonality(u16 trainer, u8 slot, u32 personality)
{
    const struct EliteFourTraining *training = GetTraining(trainer, slot);
    if (training != NULL)
    {
        // Nature and the ability slot share personality bits in this engine.
        // At most 50 successive values are needed to satisfy both constraints.
        while (personality % NUM_NATURES != training->nature
            || (personality & 1) != training->abilityNum)
            personality++;
    }
    return personality;
}

void EliteFour_ApplyTraining(struct Pokemon *mon, u16 trainer, u8 slot)
{
    const struct EliteFourTraining *training = GetTraining(trainer, slot);
    u32 ivs = 0x3FFFFFFF;
    u8 stat;
    if (!EliteFour_IsChallenge(trainer))
        return;
    SetMonData(mon, MON_DATA_IVS, &ivs);
    if (training != NULL && GetMonData(mon, MON_DATA_SPECIES) == training->species)
        for (stat = 0; stat < NUM_STATS; stat++)
            SetMonData(mon, MON_DATA_HP_EV + stat, &training->evs[stat]);
    else
        ApplyRoleTraining(mon);
    CalculateMonStats(mon);
}

void EliteFour_ApplyFacilityTraining(struct Pokemon *mon)
{
    u32 ivs=0x3FFFFFFF;
    SetMonData(mon,MON_DATA_IVS,&ivs);
    ApplyRoleTraining(mon);
    CalculateMonStats(mon);
}
