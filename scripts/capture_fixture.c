// Disposable fixture. Tests real quota APIs and native ball/caught-mon commands.
#include "new_game.h"
#include "load_save.h"
#include "save.h"
#include "pokemon.h"
#include "pokedex.h"
#include "nuzlocke.h"
#include "battle.h"
#include "wild_encounter.h"
#include "event_data.h"
#include "overworld.h"
#include "string_util.h"
#include "constants/items.h"
#include "constants/maps.h"
extern bool8 CaptureTestThrow(bool8 blocked);
extern bool8 CaptureTestThrowWithBall(bool8 blocked, u16 ball);
extern void CaptureTestGiveCaughtMon(void);
EWRAM_DATA u32 gCaptureDone, gCapturePassed, gCaptureFailed;
EWRAM_DATA u32 gCaptureObserved[7];
#define RECORD(bit, condition) do {if(condition)gCapturePassed|=1u<<(bit);else gCaptureFailed|=1u<<(bit);}while(0)
static void CaptureIdle(void) {}
static void CaptureMap(u16 map)
{
    gSaveBlock1Ptr->location.mapGroup = MAP_GROUP(map);
    gSaveBlock1Ptr->location.mapNum = MAP_NUM(map);
}
static bool8 CaptureUsed(void)
{
    u8 area = GetCurrentRegionMapSectionId();
    return (gSaveBlock2Ptr->challenge.encounters[area / 8] & (1 << (area % 8))) != 0;
}
static void CaptureWild(u16 species, u32 flags)
{
    gBattleTypeFlags = flags;
    CreateMon(&gEnemyParty[0], species, 5, 15, TRUE, 500, OT_ID_PLAYER_ID, 0);
    Nuzlocke_OnBattleStart();
}
static void CaptureFresh(u16 map)
{
    Nuzlocke_ChooseMode(GAME_MODE_HARDCORE);
    Nuzlocke_Start();
    CaptureMap(map);
}
static void CaptureBoot(void)
{
    static const u8 name[] = _("TEST");
    u8 i;
    u16 h, j;
    bool8 good;
    SetMainCallback2(CaptureIdle);
    SetSaveBlocksPointers();
    if (LoadGameSave(SAVE_NORMAL) == SAVE_STATUS_OK)
    {
        CaptureMap(MAP_ROUTE1);
        CaptureWild(SPECIES_SPEAROW, 0);
        good = Nuzlocke_IsActive() && CaptureUsed() && !Nuzlocke_CanCatch();
        CaptureMap(MAP_ROUTE2);
        good = good && !CaptureUsed();
        CaptureWild(SPECIES_SPEAROW, 0);
        RECORD(21, good && Nuzlocke_CanCatch());
        gCaptureDone = 2;
        return;
    }
    StringCopy(gSaveBlock2Ptr->playerName, name);
    StringCopy(gSaveBlock1Ptr->rivalName, name);
    NewGameInitData();
    Nuzlocke_ChooseMode(GAME_MODE_HARDCORE);
    CaptureMap(MAP_ROUTE1);
    CaptureWild(SPECIES_RATTATA, 0);
    RECORD(0, Nuzlocke_CanCatch() && !CaptureUsed());
    Nuzlocke_Start();
    CaptureWild(SPECIES_RATTATA, 0);
    RECORD(1, Nuzlocke_CanCatch() && CaptureUsed());
    good = TRUE;
    for (i = 0; i < 5; i++)
        good = good && Nuzlocke_CanCatch() && Nuzlocke_CanUseBattleItem(ITEM_POKE_BALL);
    RECORD(2, good); // Several ball attempts within this same battle.
    gBattleOutcome = B_OUTCOME_RAN;
    CaptureWild(SPECIES_SPEAROW, 0);
    RECORD(3, !Nuzlocke_CanCatch());
    CaptureFresh(MAP_ROUTE1);
    CaptureWild(SPECIES_RATTATA, 0);
    gBattleOutcome = B_OUTCOME_WON;
    CaptureWild(SPECIES_SPEAROW, 0);
    RECORD(4, !Nuzlocke_CanCatch());
    CaptureMap(MAP_ROUTE2);
    CaptureWild(SPECIES_SPEAROW, 0);
    RECORD(5, Nuzlocke_CanCatch());
    CaptureMap(MAP_ROUTE1);
    CaptureWild(SPECIES_PIDGEY, 0);
    RECORD(6, !Nuzlocke_CanCatch());
    GetSetPokedexFlag(SpeciesToNationalPokedexNum(SPECIES_RATTATA), FLAG_SET_SEEN);
    GetSetPokedexFlag(SpeciesToNationalPokedexNum(SPECIES_RATTATA), FLAG_SET_CAUGHT);
    CaptureFresh(MAP_ROUTE2);
    CaptureWild(SPECIES_RATTATA, 0);
    RECORD(7, !Nuzlocke_CanCatch() && !CaptureUsed());
    CaptureWild(SPECIES_PIDGEY, 0);
    RECORD(8, Nuzlocke_CanCatch() && CaptureUsed());
    CaptureFresh(MAP_ROUTE2);
    CaptureWild(SPECIES_RATICATE, 0);
    RECORD(9, Nuzlocke_CanCatch()); // Exact species clause, not evolution family.
    CaptureFresh(MAP_ROUTE1);
    CreateMon(&gEnemyParty[0], SPECIES_SPEAROW, 5, 15, TRUE, 0, OT_ID_PRESET, 0);
    gBattleTypeFlags = 0;
    Nuzlocke_OnBattleStart();
    good = IsMonShiny(&gEnemyParty[0]) && Nuzlocke_CanCatch() && CaptureUsed();
    Nuzlocke_OnBattleStart();
    RECORD(10, good && !Nuzlocke_CanCatch());
    CaptureFresh(MAP_ROUTE1);
    CaptureWild(SPECIES_SPEAROW, BATTLE_TYPE_TRAINER);
    RECORD(11, !CaptureUsed());
    CaptureWild(SPECIES_SPEAROW, BATTLE_TYPE_LINK);
    RECORD(12, !CaptureUsed());
    CaptureWild(SPECIES_SPEAROW, BATTLE_TYPE_OLD_MAN_TUTORIAL);
    RECORD(13, !CaptureUsed());
    CaptureFresh(MAP_MT_MOON_1F);
    CaptureWild(SPECIES_ZUBAT, 0);
    CaptureMap(MAP_MT_MOON_B1F);
    CaptureWild(SPECIES_GEODUDE, 0);
    RECORD(14, !Nuzlocke_CanCatch());
    CaptureFresh(MAP_SAFARI_ZONE_CENTER);
    CaptureWild(SPECIES_NIDORAN_F, BATTLE_TYPE_SAFARI);
    good = Nuzlocke_CanCatch();
    CaptureMap(MAP_SAFARI_ZONE_EAST);
    CaptureWild(SPECIES_NIDORAN_M, BATTLE_TYPE_SAFARI);
    RECORD(15, good && !Nuzlocke_CanCatch() && CaptureTestThrowWithBall(TRUE, ITEM_SAFARI_BALL));
    CaptureFresh(MAP_ROUTE1);
    CaptureWild(SPECIES_SPEAROW, 0); // Grass, surf, rod and static share this section.
    CaptureWild(SPECIES_SNORLAX, BATTLE_TYPE_LEGENDARY_FRLG);
    RECORD(16, !Nuzlocke_CanCatch());
    CaptureFresh(MAP_POKEMON_TOWER_6F);
    CaptureWild(SPECIES_GASTLY, BATTLE_TYPE_GHOST);
    RECORD(17, !CaptureUsed());
    CaptureWild(SPECIES_MAROWAK, BATTLE_TYPE_GHOST | BATTLE_TYPE_GHOST_UNVEILED);
    RECORD(18, !CaptureUsed());
    // End-to-end native command path: authorized catch, give mon, then reject next.
    CaptureFresh(MAP_ROUTE1);
    ZeroPlayerPartyMons();
    CreateMon(&gPlayerParty[0], SPECIES_BULBASAUR, 5, 15, TRUE, 500, OT_ID_PRESET, 0);
    CalculatePlayerPartyCount();
    CaptureWild(SPECIES_CATERPIE, 0);
    GetSetPokedexFlag(SpeciesToNationalPokedexNum(SPECIES_CATERPIE), FLAG_SET_SEEN);
    gCaptureObserved[0] = Nuzlocke_CanCatch();
    good = CaptureTestThrow(FALSE);
    gCaptureObserved[1] = good;
    CaptureTestGiveCaughtMon();
    gCaptureObserved[2] = GetMonData(&gPlayerParty[1], MON_DATA_SPECIES);
    good = good && GetMonData(&gPlayerParty[1], MON_DATA_SPECIES) == SPECIES_CATERPIE;
    CaptureWild(SPECIES_WEEDLE, 0);
    gCaptureObserved[3] = Nuzlocke_CanCatch();
    gCaptureObserved[4] = CaptureTestThrow(TRUE);
    gCaptureObserved[5] = Nuzlocke_CanUseBattleItem(ITEM_MASTER_BALL);
    RECORD(19, good && !Nuzlocke_CanCatch() && CaptureTestThrow(TRUE)
        && !Nuzlocke_CanUseBattleItem(ITEM_MASTER_BALL)
        && CaptureTestThrowWithBall(TRUE, ITEM_MASTER_BALL));
    Nuzlocke_ChooseMode(GAME_MODE_NORMAL);
    CaptureWild(SPECIES_PIDGEY, 0);
    CaptureWild(SPECIES_PIDGEY, 0);
    RECORD(20, Nuzlocke_CanCatch() && !CaptureUsed());
    good = TRUE;
    for (h = 0; gWildMonHeaders[h].mapGroup != MAP_GROUP(MAP_UNDEFINED); h++)
    {
        for (j = 0; j < h; j++)
            if (gWildMonHeaders[j].mapGroup == gWildMonHeaders[h].mapGroup
                && gWildMonHeaders[j].mapNum == gWildMonHeaders[h].mapNum) break;
        if (j < h) continue; // Count each map once, including only one Altering Cave header.
        Nuzlocke_ChooseMode(GAME_MODE_HARDCORE);
        Nuzlocke_Start();
        gSaveBlock1Ptr->location.mapGroup = gWildMonHeaders[h].mapGroup;
        gSaveBlock1Ptr->location.mapNum = gWildMonHeaders[h].mapNum;
        CaptureWild(SPECIES_DODUO, 0);
        good = good && Nuzlocke_CanCatch() && CaptureUsed();
        CaptureWild(SPECIES_DODRIO, 0);
        good = good && !Nuzlocke_CanCatch();
        gCaptureObserved[6]++;
    }
    RECORD(22, good && gCaptureObserved[6] > 100);
    CaptureFresh(MAP_ROUTE1);
    CaptureWild(SPECIES_SPEAROW, 0);
    Save_ResetSaveCounters();
    if (TrySavingData(SAVE_NORMAL) != SAVE_STATUS_OK)
        gCaptureFailed |= 1u << 21;
    gCaptureDone = 1;
}
