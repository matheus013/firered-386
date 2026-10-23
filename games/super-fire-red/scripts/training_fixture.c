// Disposable fixture: native game APIs, actual save write and fresh-instance reload.
#include "training.h"
#include "nuzlocke.h"
#include "new_game.h"
#include "save.h"
#include "load_save.h"
#include "pokemon.h"
#include "event_data.h"
#include "item.h"
#include "overworld.h"
#include "string_util.h"
#include "constants/items.h"
#include "constants/maps.h"
#include "constants/pokemon.h"

EWRAM_DATA u32 gHardcoreTestResult = 0;
#define CHECK(condition, error) if (!(condition)) {gHardcoreTestResult = error; return;}
static void TrainingTestIdle(void) {}
static void Center(u16 map)
{
    gSaveBlock1Ptr->location.mapGroup = MAP_GROUP(map);
    gSaveBlock1Ptr->location.mapNum = MAP_NUM(map);
}
static void TrainingTestBoot(void)
{
    static const u8 name[] = _("TEST");
    static const u16 stock[] = {ITEM_POTION, ITEM_ANTIDOTE, ITEM_NONE};
    u8 evs[6] = {252, 0, 4, 0, 252, 0};
    u8 ivs[6] = {31, 0, 31, 0, 31, 31};
    u8 bad[6] = {252, 252, 252, 0, 0, 0};
    u16 buffer[5], hp = 1;
    u8 i, ability;
    struct Pokemon restored;
    SetMainCallback2(TrainingTestIdle);
    SetSaveBlocksPointers();
    gHardcoreTestResult = 100;
    CHECK(sizeof(struct SaveBlock2) == 0xF24 && offsetof(struct SaveBlock2, encryptionKey) == 0xF20, 10);
    CHECK(sizeof(struct BoxPokemon) == 80 && sizeof(struct Pokemon) == 100, 11);
    if (LoadGameSave(SAVE_NORMAL) == SAVE_STATUS_OK)
    {
        CHECK(Nuzlocke_IsHardcore(), 12);
        CHECK(GetNature(&gPlayerParty[0]) == NATURE_MODEST, 13);
        CHECK(IsMonShiny(&gPlayerParty[0]) && GetMonData(&gPlayerParty[0], MON_DATA_PERSONALITY) == 0, 14);
        CHECK(GetMonData(&gPlayerParty[0], MON_DATA_HP_EV) == 252 && GetMonData(&gPlayerParty[0], MON_DATA_SPATK_IV) == 31, 15);
        Center(MAP_VIRIDIAN_CITY_POKEMON_CENTER_1F);
        CHECK(Training_GetCenterUses() == 3, 16);
        Training_CheckCenter();
        CHECK(gSpecialVar_Result == 2, 17);
        Center(MAP_PEWTER_CITY_POKEMON_CENTER_1F);
        CHECK(Training_GetCenterUses() == 1, 18);
        CHECK(ItemId_GetPrice(ITEM_RARE_CANDY) == 1, 19);
        gHardcoreTestResult = 2;
        return;
    }
    StringCopy(gSaveBlock2Ptr->playerName, name);
    StringCopy(gSaveBlock1Ptr->rivalName, name);
    NewGameInitData();
    Nuzlocke_ChooseMode(GAME_MODE_NORMAL);
    CreateMon(&gPlayerParty[0], SPECIES_BULBASAUR, 5, 15, TRUE, 0, OT_ID_PRESET, 0);
    gPlayerPartyCount = 1;
    CHECK(Training_IsUnlocked() && !Training_AreAdvancedEditsUnlocked() && !Training_SetNature(&gPlayerParty[0], NATURE_MODEST), 20);
    CHECK(Training_SetEVs(&gPlayerParty[0], evs) && !Training_SetIVs(&gPlayerParty[0], ivs), 21);
    CHECK(ItemId_GetPrice(ITEM_RARE_CANDY) == 4800, 22);
    CHECK(Training_CopyShopStock(stock, buffer) == 2 && buffer[2] == ITEM_NONE, 23);
    FlagSet(FLAG_SYS_GAME_CLEAR);
    CHECK(Training_IsUnlocked() && Training_AreAdvancedEditsUnlocked() && Training_SetNature(&gPlayerParty[0], NATURE_BOLD) && Training_SetIVs(&gPlayerParty[0], ivs), 24);
    FlagClear(FLAG_SYS_GAME_CLEAR);
    Nuzlocke_ChooseMode(GAME_MODE_HARDCORE);
    CHECK(Training_IsUnlocked() && !Nuzlocke_IsActive(), 25);
    ability = GetMonAbility(&gPlayerParty[0]);
    SetMonData(&gPlayerParty[0], MON_DATA_HP, &hp);
    CHECK(Training_SetNature(&gPlayerParty[0], NATURE_MODEST), 26);
    CHECK(GetNature(&gPlayerParty[0]) == NATURE_MODEST && GetMonAbility(&gPlayerParty[0]) == ability, 27);
    CHECK(IsMonShiny(&gPlayerParty[0]) && GetMonData(&gPlayerParty[0], MON_DATA_PERSONALITY) == 0, 28);
    CHECK(Training_SetEVs(&gPlayerParty[0], evs) && Training_SetIVs(&gPlayerParty[0], ivs), 29);
    CHECK(GetMonData(&gPlayerParty[0], MON_DATA_HP) == 1, 30);
    CHECK(!Training_SetEVs(&gPlayerParty[0], bad), 31);
    CHECK(GetMonData(&gPlayerParty[0], MON_DATA_HP_EV) == 252, 32);
    bad[0] = 32;
    CHECK(!Training_SetIVs(&gPlayerParty[0], bad), 33);
    CHECK(!Training_SetNature(&gPlayerParty[0], 25), 34);
    BoxMonToMon(&gPlayerParty[0].box, &restored);
    CHECK(GetNature(&restored) == NATURE_MODEST && IsMonShiny(&restored), 35);
    CHECK(GetMonData(&restored, MON_DATA_HP_EV) == 252 && GetMonData(&restored, MON_DATA_SPATK_IV) == 31, 36);
    CreateMon(&gPlayerParty[1], SPECIES_CHARMANDER, 5, 15, FALSE, 0, OT_ID_PLAYER_ID, 0);
    gPlayerPartyCount = 2;
    Nuzlocke_Start();
    Nuzlocke_MarkFainted(&gPlayerParty[1]);
    CHECK(!Training_SetNature(&gPlayerParty[1], NATURE_MODEST) && !Training_SetIVs(&gPlayerParty[1], ivs), 37);
    CHECK(!GetMonData(&gPlayerParty[1], MON_DATA_HP), 38);
    CHECK(Training_CopyShopStock(stock, buffer) == 3 && buffer[2] == ITEM_RARE_CANDY && buffer[3] == ITEM_NONE, 39);
    CHECK(Training_CopyShopStock(buffer, buffer) == 3, 40);
    CHECK(ItemId_GetPrice(ITEM_RARE_CANDY) == 1 && ItemId_GetPrice(ITEM_RARE_CANDY) / 2 == 0, 41);
    Center(MAP_VIRIDIAN_CITY_POKEMON_CENTER_1F);
    for (i = 0; i < 3; i++)
    {
        Training_CheckCenter();
        CHECK(gSpecialVar_Result == 1 && Training_GetCenterUses() == i, 42);
        Training_UseCenter();
    }
    Training_CheckCenter();
    CHECK(gSpecialVar_Result == 2 && Training_GetCenterUses() == 3, 43);
    Training_UseCenter();
    CHECK(Training_GetCenterUses() == 3, 44);
    Center(MAP_PEWTER_CITY_POKEMON_CENTER_1F);
    CHECK(Training_GetCenterUses() == 0, 45);
    Training_UseCenter();
    CHECK(Training_GetCenterUses() == 1, 46);
    Save_ResetSaveCounters();
    CHECK(TrySavingData(SAVE_NORMAL) == SAVE_STATUS_OK, 47);
    gHardcoreTestResult = 1;
}
