// Appended only to a disposable copy of wild_encounter.c.
#include "new_game.h"
#include "load_save.h"
#include "string_util.h"
#include "battle.h"
#include "constants/pokedex.h"
EWRAM_DATA u32 gEncounterTestResult = 0;
EWRAM_DATA u32 gEncounterTestSpecies = 0;
EWRAM_DATA u32 gEncounterTestSlots = 0;
static bool8 sEncounterSeen[NATIONAL_DEX_COUNT + 1];
static bool8 AuditEncounterSlots(const struct WildPokemonInfo *info, u8 count)
{
    u8 i, level;
    u16 dex;
    if (info == NULL) return TRUE;
    for (i = 0; i < count; i++)
    {
        GenerateWildMon(info->wildPokemon[i].species, info->wildPokemon[i].minLevel, i);
        if (GetMonData(&gEnemyParty[0], MON_DATA_SPECIES) != info->wildPokemon[i].species) return FALSE;
        level = GetMonData(&gEnemyParty[0], MON_DATA_LEVEL);
        if (level < info->wildPokemon[i].minLevel || level > info->wildPokemon[i].maxLevel) return FALSE;
        if (GetMonData(&gEnemyParty[0], MON_DATA_SANITY_IS_BAD_EGG)) return FALSE;
        if ((info->wildPokemon[i].species == SPECIES_MEW || info->wildPokemon[i].species == SPECIES_DEOXYS)
            && !GetMonData(&gEnemyParty[0], MON_DATA_MODERN_FATEFUL_ENCOUNTER)) return FALSE;
        dex = SpeciesToNationalPokedexNum(GetMonData(&gEnemyParty[0], MON_DATA_SPECIES));
        if (!dex || dex > NATIONAL_DEX_COUNT) return FALSE;
        sEncounterSeen[dex] = TRUE;
        gEncounterTestSlots++;
    }
    return TRUE;
}
static void EncounterTestIdle(void) {}
void EncounterTestBoot(void)
{
    u16 h, species, number;
    u8 legends = 0;
    static const u8 name[] = _("TEST");
    extern void CB2_InitBattle(void);
    SetSaveBlocksPointers();
    StringCopy(gSaveBlock2Ptr->playerName, name);
    StringCopy(gSaveBlock1Ptr->rivalName, name);
    NewGameInitData();
    gSaveBlock2Ptr->optionsTextSpeed = 2;
    gSaveBlock2Ptr->optionsBattleSceneOff = TRUE;
    gEncounterTestResult = 100;
    FlagClear(FLAG_SYS_GAME_CLEAR);
    for (species = 1; species < NUM_SPECIES; species++)
    {
        if (!IsPostgameWildLegend(species)) continue;
        legends++;
        GenerateWildMon(species, 90, 0);
        if (GetMonData(&gEnemyParty[0], MON_DATA_SPECIES) != SPECIES_ZUBAT
            || GetMonData(&gEnemyParty[0], MON_DATA_LEVEL) != 12) {gEncounterTestResult=10;return;}
    }
    if (legends != 21) {gEncounterTestResult=13;return;}
    FlagSet(FLAG_SYS_GAME_CLEAR);
    FlagSet(FLAG_SYS_UNLOCKED_TANOBY_RUINS);
    VarSet(VAR_ALTERING_CAVE_WILD_SET, 0);
    for (h = 0; gWildMonHeaders[h].mapGroup != MAP_GROUP(MAP_UNDEFINED); h++)
    {
        const struct WildPokemonHeader *header = &gWildMonHeaders[h];
        gSaveBlock1Ptr->location.mapGroup = header->mapGroup;
        gSaveBlock1Ptr->location.mapNum = header->mapNum;
        if (GetCurrentMapWildMonHeaderId() != h) continue; // Exclude inactive Altering Cave tables.
        if (header->mapGroup == MAP_GROUP(MAP_SSANNE_EXTERIOR) && header->mapNum == MAP_NUM(MAP_SSANNE_EXTERIOR)) continue;
        if (!(header->mapGroup == MAP_GROUP(MAP_ROUTE21_SOUTH) && header->mapNum == MAP_NUM(MAP_ROUTE21_SOUTH)))
            if (!AuditEncounterSlots(header->landMonsInfo, LAND_WILD_COUNT)) {gEncounterTestResult=11;return;}
        if (!AuditEncounterSlots(header->waterMonsInfo, WATER_WILD_COUNT)
            || !AuditEncounterSlots(header->rockSmashMonsInfo, ROCK_WILD_COUNT)
            || !AuditEncounterSlots(header->fishingMonsInfo, FISH_WILD_COUNT)) {gEncounterTestResult=12;return;}
    }
    for (number = 1; number <= NATIONAL_DEX_COUNT; number++)
    {
        if (!sEncounterSeen[number]) {gEncounterTestResult=1000+number;return;}
        gEncounterTestSpecies++;
    }
    // Actual new postgame wild battle after auditing every compiled slot.
    gSaveBlock1Ptr->location.mapGroup = MAP_GROUP(MAP_FIVE_ISLAND_LOST_CAVE_ROOM10);
    gSaveBlock1Ptr->location.mapNum = MAP_NUM(MAP_FIVE_ISLAND_LOST_CAVE_ROOM10);
    gMapHeader = *Overworld_GetMapHeaderByGroupAndId(MAP_GROUP(MAP_FIVE_ISLAND_LOST_CAVE_ROOM10), MAP_NUM(MAP_FIVE_ISLAND_LOST_CAVE_ROOM10));
    GenerateWildMon(SPECIES_MEW, 85, 11);
    CreateMon(&gPlayerParty[0], SPECIES_SNORLAX, 100, 31, FALSE, 0, OT_ID_PLAYER_ID, 0);
    CalculatePlayerPartyCount();
    gBattleTypeFlags = 0;
    gMain.savedCallback = EncounterTestIdle;
    gEncounterTestResult = 1;
    SetMainCallback2(CB2_InitBattle);
}
