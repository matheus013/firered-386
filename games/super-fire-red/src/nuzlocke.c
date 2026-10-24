#include "global.h"
#include "game_settings.h"
#include "nuzlocke.h"
#include "pokemon.h"
#include "battle.h"
#include "event_data.h"
#include "item.h"
#include "overworld.h"
#include "pokedex.h"
#include "constants/items.h"
#include "constants/trainers.h"

#define CHALLENGE_MAGIC 0x31435A4E // NZC1; old saves have no matching version tag.
#define NUZLOCKE_DEAD_BIT 1 // Reserved BoxPokemon header bit; survives PC transfers.

static EWRAM_DATA bool8 sCanCatch = TRUE;

static bool8 IsPracticeBattle(void)
{
    return (gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_OLD_MAN_TUTORIAL
        | BATTLE_TYPE_POKEDUDE | BATTLE_TYPE_TRAINER_TOWER | BATTLE_TYPE_BATTLE_TOWER)) != 0;
}

static bool8 IsPartyMon(struct Pokemon *mon)
{
    u8 i;
    for (i = 0; i < PARTY_SIZE; i++)
        if (mon == &gPlayerParty[i])
            return TRUE;
    return FALSE;
}

bool8 Nuzlocke_IsHardcore(void)
{
    return gSaveBlock2Ptr != NULL
        && gSaveBlock2Ptr->challenge.magic == CHALLENGE_MAGIC
        && gSaveBlock2Ptr->challenge.mode == GAME_MODE_HARDCORE;
}

void Nuzlocke_ChooseMode(u8 mode)
{
    u8 language = GameSettings_GetLanguage();
    memset(&gSaveBlock2Ptr->challenge, 0, sizeof(gSaveBlock2Ptr->challenge));
    GameSettings_SetLanguage(language);
    gSaveBlock2Ptr->challenge.magic = CHALLENGE_MAGIC;
    gSaveBlock2Ptr->challenge.mode = mode == GAME_MODE_HARDCORE ? GAME_MODE_HARDCORE : GAME_MODE_NORMAL;
    if (Nuzlocke_IsHardcore())
        gSaveBlock2Ptr->optionsBattleStyle = OPTIONS_BATTLE_STYLE_SET;
    sCanCatch = TRUE;
}

void Nuzlocke_Start(void)
{
    if (Nuzlocke_IsHardcore())
        gSaveBlock2Ptr->challenge.started = TRUE;
}

bool8 Nuzlocke_IsActive(void)
{
    return Nuzlocke_IsHardcore() && gSaveBlock2Ptr->challenge.started;
}

void Nuzlocke_OnBattleStart(void)
{
    u8 area;
    u16 species;
    sCanCatch = TRUE;
    if (!Nuzlocke_IsActive() || IsPracticeBattle() || (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
        // Both unidentified ghosts and the revealed Marowak reject all balls.
        || (gBattleTypeFlags & BATTLE_TYPE_GHOST))
        return;

    sCanCatch = FALSE;
    species = GetMonData(&gEnemyParty[0], MON_DATA_SPECIES);
    // Already caught species do not consume a new area's eligible encounter.
    if (species == SPECIES_NONE || GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_GET_CAUGHT))
        return;

    area = GetCurrentRegionMapSectionId();
    if (!(gSaveBlock2Ptr->challenge.encounters[area / 8] & (1 << (area % 8))))
    {
        gSaveBlock2Ptr->challenge.encounters[area / 8] |= 1 << (area % 8);
        sCanCatch = TRUE;
    }
}

bool8 Nuzlocke_CanCatch(void)
{
    return !Nuzlocke_IsActive() || IsPracticeBattle() || sCanCatch;
}

bool8 Nuzlocke_CanUseBattleItem(u16 item)
{
    if (!Nuzlocke_IsHardcore() || IsPracticeBattle())
        return TRUE;
    return ItemId_GetPocket(item) == POCKET_POKE_BALLS && Nuzlocke_CanCatch();
}

bool8 Nuzlocke_IsDead(struct Pokemon *mon)
{
    return Nuzlocke_IsHardcore() && (mon->box.unused & NUZLOCKE_DEAD_BIT);
}

void Nuzlocke_MarkFainted(struct Pokemon *mon)
{
    if (!Nuzlocke_IsActive() || (gMain.inBattle && IsPracticeBattle())
        || !GetMonData(mon, MON_DATA_SPECIES) || GetMonData(mon, MON_DATA_IS_EGG))
        return;
    mon->box.unused |= NUZLOCKE_DEAD_BIT;
    mon->hp = 0;
}

static u8 GetTrainerLevel(u16 trainerId)
{
    const struct Trainer *trainer = &gTrainers[trainerId];
    u8 i, level, highest = 1;
    for (i = 0; i < trainer->partySize; i++)
    {
        switch (trainer->partyFlags)
        {
        case 0: level = trainer->party.NoItemDefaultMoves[i].lvl; break;
        case F_TRAINER_PARTY_CUSTOM_MOVESET: level = trainer->party.NoItemCustomMoves[i].lvl; break;
        case F_TRAINER_PARTY_HELD_ITEM: level = trainer->party.ItemDefaultMoves[i].lvl; break;
        default: level = trainer->party.ItemCustomMoves[i].lvl; break;
        }
        if (level > highest)
            highest = level;
    }
    return highest;
}

u8 Nuzlocke_GetLevelCap(void)
{
    static const u16 leaders[] = {TRAINER_LEADER_BROCK, TRAINER_LEADER_MISTY,
        TRAINER_LEADER_LT_SURGE, TRAINER_LEADER_ERIKA, TRAINER_LEADER_KOGA,
        TRAINER_LEADER_SABRINA, TRAINER_LEADER_BLAINE, TRAINER_LEADER_GIOVANNI};
    u8 i;
    if (FlagGet(FLAG_SYS_GAME_CLEAR))
        return MAX_LEVEL;
    for (i = 0; i < ARRAY_COUNT(leaders); i++)
        if (!FlagGet(FLAG_BADGE01_GET + i))
            return GetTrainerLevel(leaders[i]);
    if (!FlagGet(FLAG_DEFEATED_LORELEI))
        return GetTrainerLevel(TRAINER_ELITE_FOUR_LORELEI);
    if (!FlagGet(FLAG_DEFEATED_BRUNO))
        return GetTrainerLevel(TRAINER_ELITE_FOUR_BRUNO);
    if (!FlagGet(FLAG_DEFEATED_AGATHA))
        return GetTrainerLevel(TRAINER_ELITE_FOUR_AGATHA);
    if (!FlagGet(FLAG_DEFEATED_LANCE))
        return GetTrainerLevel(TRAINER_ELITE_FOUR_LANCE);
    return GetTrainerLevel(TRAINER_CHAMPION_FIRST_SQUIRTLE);
}

bool8 Nuzlocke_IsOverCap(struct Pokemon *mon)
{
    return Nuzlocke_IsHardcore() && IsPartyMon(mon) && GetMonData(mon, MON_DATA_LEVEL) > Nuzlocke_GetLevelCap();
}

u32 Nuzlocke_ClampExperience(struct Pokemon *mon, u32 experience)
{
    // Generic creation/stat code also handles enemies and received over-level mons.
    if (!IsPartyMon(mon))
        return experience;
    return Nuzlocke_ClampTrainingExperience(mon, experience);
}

u32 Nuzlocke_ClampTrainingExperience(struct Pokemon *mon, u32 experience)
{
    u16 species;
    u8 cap;
    u32 limit, current;
    current = GetMonData(mon, MON_DATA_EXP);
    if (Nuzlocke_IsDead(mon))
        return current;
    species = GetMonData(mon, MON_DATA_SPECIES);
    cap = Nuzlocke_GetLevelCap();
    if (!species || cap == MAX_LEVEL)
        return experience;
    limit = gExperienceTables[gSpeciesInfo[species].growthRate][cap + 1] - 1;
    // An acquired over-level mon keeps its level, but cannot gain more EXP.
    if (current > limit)
        limit = current;
    return experience > limit ? limit : experience;
}

u16 Nuzlocke_LimitExpGain(struct Pokemon *mon, u32 gain)
{
    u32 current = GetMonData(mon, MON_DATA_EXP);
    return Nuzlocke_ClampExperience(mon, current + gain) - current;
}
