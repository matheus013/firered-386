// Injected ONLY into a temporary test build, never the delivered ROM.
#include "nuzlocke.h"
#include "new_game.h"
#include "save.h"
#include "load_save.h"
#include "pokemon.h"
#include "pokedex.h"
#include "event_data.h"
#include "overworld.h"
#include "script_pokemon_util.h"
#include "string_util.h"
#include "constants/items.h"
#include "constants/maps.h"

EWRAM_DATA u32 gHardcoreTestResult = 0;
#define CHECK_HC(condition, error) if (!(condition)) {gHardcoreTestResult = error; return;}
static void HardcoreTestIdle(void) {}
static void HardcoreTestRoute(u8 route)
{
    gSaveBlock1Ptr->location.mapGroup = route == 1 ? MAP_GROUP(MAP_ROUTE1) : MAP_GROUP(MAP_ROUTE2);
    gSaveBlock1Ptr->location.mapNum = route == 1 ? MAP_NUM(MAP_ROUTE1) : MAP_NUM(MAP_ROUTE2);
}
static void HardcoreTestBoot(void)
{
    static const u8 name[] = _("TEST");
    u32 experience;
    u16 hp = 50;
    u8 i, area;
    struct BoxPokemon deadBox;
    struct Pokemon restored;
    SetMainCallback2(HardcoreTestIdle);
    SetSaveBlocksPointers();
    CHECK_HC(sizeof(struct SaveBlock2) == 0xF24, 10);
    CHECK_HC(offsetof(struct SaveBlock2, encryptionKey) == 0xF20, 11);
    gHardcoreTestResult = 100;
    if (LoadGameSave(SAVE_NORMAL) == SAVE_STATUS_OK)
    {
        CHECK_HC(Nuzlocke_IsHardcore() && Nuzlocke_IsActive(), 12);
        CHECK_HC(Nuzlocke_IsDead(&gPlayerParty[0]) && !GetMonData(&gPlayerParty[0], MON_DATA_HP), 13);
        HardcoreTestRoute(1);
        area = GetCurrentRegionMapSectionId();
        CHECK_HC(gSaveBlock2Ptr->challenge.encounters[area / 8] & (1 << (area % 8)), 14);
        HealPlayerParty();
        CHECK_HC(!GetMonData(&gPlayerParty[0], MON_DATA_HP), 15);
        gHardcoreTestResult = 2;
        return;
    }
    StringCopy(gSaveBlock2Ptr->playerName, name);
    StringCopy(gSaveBlock1Ptr->rivalName, name);
    NewGameInitData();
    gSaveBlock2Ptr->challenge.magic = 0;
    gSaveBlock2Ptr->challenge.mode = GAME_MODE_HARDCORE;
    CHECK_HC(!Nuzlocke_IsHardcore(), 16); // An old save's unused bytes are not a valid mode.
    Nuzlocke_ChooseMode(GAME_MODE_NORMAL);
    CHECK_HC(!Nuzlocke_IsHardcore() && Nuzlocke_GetLevelCap() == MAX_LEVEL, 17);
    CHECK_HC(Nuzlocke_CanUseBattleItem(ITEM_POTION), 18);
    Nuzlocke_ChooseMode(GAME_MODE_HARDCORE);
    NewGameInitData(); // The introduction initializes the world after choosing a mode.
    CHECK_HC(Nuzlocke_IsHardcore() && !Nuzlocke_IsActive(), 19);
    CHECK_HC(gSaveBlock2Ptr->optionsBattleStyle == OPTIONS_BATTLE_STYLE_SET, 20);
    CreateMon(&gPlayerParty[0], SPECIES_BULBASAUR, 5, 15, FALSE, 0, OT_ID_PLAYER_ID, 0);
    gPlayerPartyCount = 1;
    Nuzlocke_MarkFainted(&gPlayerParty[0]);
    CHECK_HC(!Nuzlocke_IsDead(&gPlayerParty[0]), 21);
    Nuzlocke_Start();
    CHECK_HC(Nuzlocke_IsActive(), 22);
    gBattleTypeFlags = 0;
    HardcoreTestRoute(1);
    CreateMon(&gEnemyParty[0], SPECIES_RATTATA, 5, 15, FALSE, 0, OT_ID_PLAYER_ID, 0);
    Nuzlocke_OnBattleStart();
    CHECK_HC(Nuzlocke_CanCatch(), 23);
    CHECK_HC(Nuzlocke_CanUseBattleItem(ITEM_POKE_BALL) && !Nuzlocke_CanUseBattleItem(ITEM_POTION), 24);
    Nuzlocke_OnBattleStart();
    CHECK_HC(!Nuzlocke_CanCatch() && !Nuzlocke_CanUseBattleItem(ITEM_MASTER_BALL), 25);
    HardcoreTestRoute(2);
    GetSetPokedexFlag(SpeciesToNationalPokedexNum(SPECIES_RATTATA), FLAG_SET_SEEN);
    GetSetPokedexFlag(SpeciesToNationalPokedexNum(SPECIES_RATTATA), FLAG_SET_CAUGHT);
    Nuzlocke_OnBattleStart();
    CHECK_HC(!Nuzlocke_CanCatch(), 26);
    CreateMon(&gEnemyParty[0], SPECIES_SPEAROW, 5, 15, TRUE, 0, OT_ID_PRESET, 0);
    CHECK_HC(IsMonShiny(&gEnemyParty[0]), 27);
    Nuzlocke_OnBattleStart();
    CHECK_HC(Nuzlocke_CanCatch(), 28); // Duplicate did not consume this area.
    Nuzlocke_OnBattleStart();
    CHECK_HC(!Nuzlocke_CanCatch(), 29); // Shiny does not bypass a spent encounter.
    gBattleTypeFlags = BATTLE_TYPE_LINK;
    gMain.inBattle = TRUE;
    Nuzlocke_OnBattleStart();
    Nuzlocke_MarkFainted(&gPlayerParty[0]);
    CHECK_HC(!Nuzlocke_IsDead(&gPlayerParty[0]) && Nuzlocke_CanCatch(), 30);
    gMain.inBattle = FALSE;
    gBattleTypeFlags = 0;
    Nuzlocke_MarkFainted(&gPlayerParty[0]);
    CHECK_HC(Nuzlocke_IsDead(&gPlayerParty[0]), 31);
    SetMonData(&gPlayerParty[0], MON_DATA_HP, &hp);
    HealPlayerParty();
    CHECK_HC(!GetMonData(&gPlayerParty[0], MON_DATA_HP), 32);
    CHECK_HC(PokemonUseItemEffects(&gPlayerParty[0], ITEM_REVIVE, 0, 0, FALSE), 33);
    CHECK_HC(PokemonItemUseNoEffect(&gPlayerParty[0], ITEM_MAX_REVIVE, 0, 0), 34);
    deadBox = gPlayerParty[0].box;
    BoxMonToMon(&deadBox, &restored);
    CHECK_HC(Nuzlocke_IsDead(&restored) && !GetMonData(&restored, MON_DATA_HP), 35);
    CreateMon(&gPlayerParty[1], SPECIES_CHARMANDER, 5, 15, FALSE, 0, OT_ID_PLAYER_ID, 0);
    gPlayerPartyCount = 2;
    CHECK_HC(Nuzlocke_GetLevelCap() == 14, 36);
    experience = 1000000;
    SetMonData(&gPlayerParty[1], MON_DATA_EXP, &experience);
    CalculateMonStats(&gPlayerParty[1]);
    CHECK_HC(GetMonData(&gPlayerParty[1], MON_DATA_LEVEL) == 14, 37);
    CHECK_HC(Nuzlocke_LimitExpGain(&gPlayerParty[1], 1000) == 0, 38);
    FlagSet(FLAG_BADGE01_GET);
    CHECK_HC(Nuzlocke_GetLevelCap() == 21 && Nuzlocke_LimitExpGain(&gPlayerParty[1], 1000) > 0, 39);
    FlagClear(FLAG_BADGE01_GET);
    CreateMon(&gPlayerParty[2], SPECIES_EEVEE, 25, 15, FALSE, 0, OT_ID_PLAYER_ID, 0);
    CHECK_HC(Nuzlocke_IsOverCap(&gPlayerParty[2]) && !GetMonData(&gPlayerParty[2], MON_DATA_HP), 40);
    for (i = 0; i < 8; i++) FlagSet(FLAG_BADGE01_GET + i);
    CHECK_HC(Nuzlocke_GetLevelCap() == 60 && GetMonData(&gPlayerParty[2], MON_DATA_HP) > 0, 41);
    FlagSet(FLAG_SYS_GAME_CLEAR);
    CHECK_HC(Nuzlocke_GetLevelCap() == MAX_LEVEL, 42);
    FlagClear(FLAG_SYS_GAME_CLEAR);
    for (i = 0; i < 8; i++) FlagClear(FLAG_BADGE01_GET + i);
    Nuzlocke_ChooseMode(GAME_MODE_NORMAL);
    CHECK_HC(!Nuzlocke_IsDead(&gPlayerParty[0]) && Nuzlocke_GetLevelCap() == MAX_LEVEL, 43);
    HealPlayerParty();
    CHECK_HC(GetMonData(&gPlayerParty[0], MON_DATA_HP) > 0, 44);
    // A new challenge starts clean; the production selector is only in New Game.
    Nuzlocke_ChooseMode(GAME_MODE_HARDCORE);
    Nuzlocke_Start();
    HardcoreTestRoute(1);
    CreateMon(&gEnemyParty[0], SPECIES_ZUBAT, 5, 15, FALSE, 0, OT_ID_PLAYER_ID, 0);
    Nuzlocke_OnBattleStart();
    Nuzlocke_MarkFainted(&gPlayerParty[0]);
    Save_ResetSaveCounters();
    gHardcoreTestResult = 101;
    CHECK_HC(TrySavingData(SAVE_NORMAL) == SAVE_STATUS_OK, 45);
    gHardcoreTestResult = 1;
}
