#include "global.h"
#include "pokemon.h"
#include "training.h"
#include "nuzlocke.h"
#include "event_data.h"
#include "string_util.h"
#include "constants/maps.h"
#include "constants/pokemon.h"
#include "constants/items.h"

#include "data/training_centers.h"

bool8 Training_IsUnlocked(void)
{
    gSpecialVar_Result = TRUE;
    return gSpecialVar_Result;
}

bool8 Training_AreAdvancedEditsUnlocked(void)
{
    return Nuzlocke_IsHardcore() || FlagGet(FLAG_SYS_GAME_CLEAR);
}

bool8 Training_CanEdit(struct Pokemon *mon)
{
    return Training_IsUnlocked() && GetMonData(mon, MON_DATA_SPECIES)
        && !GetMonData(mon, MON_DATA_IS_EGG) && !GetMonData(mon, MON_DATA_SANITY_IS_BAD_EGG)
        && !Nuzlocke_IsDead(mon) && !Nuzlocke_IsOverCap(mon);
}

static void RecalculateWithoutHealing(struct Pokemon *mon)
{
    u16 hp = mon->hp;
    CalculateMonStats(mon);
    if (hp > mon->maxHP)
        hp = mon->maxHP;
    SetMonData(mon, MON_DATA_HP, &hp);
}

bool8 Training_SetNature(struct Pokemon *mon, u8 nature)
{
    u16 tagged = 0xA500 | nature;
    if (nature >= NUM_NATURES || !Training_AreAdvancedEditsUnlocked() || !Training_CanEdit(mon))
        return FALSE;
    SetMonData(mon, MON_DATA_TRAINING_NATURE, &tagged);
    RecalculateWithoutHealing(mon);
    return TRUE;
}

bool8 Training_SetEVs(struct Pokemon *mon, const u8 *values)
{
    u16 total = 0;
    u8 i;
    if (!Training_CanEdit(mon))
        return FALSE;
    for (i = 0; i < NUM_STATS; i++)
    {
        if (values[i] > 252)
            return FALSE;
        total += values[i];
    }
    if (total > MAX_TOTAL_EVS)
        return FALSE;
    for (i = 0; i < NUM_STATS; i++)
        SetMonData(mon, MON_DATA_HP_EV + i, &values[i]);
    RecalculateWithoutHealing(mon);
    return TRUE;
}

bool8 Training_SetIVs(struct Pokemon *mon, const u8 *values)
{
    u8 i;
    if (!Training_AreAdvancedEditsUnlocked() || !Training_CanEdit(mon))
        return FALSE;
    for (i = 0; i < NUM_STATS; i++)
        if (values[i] > MAX_PER_STAT_IVS)
            return FALSE;
    for (i = 0; i < NUM_STATS; i++)
        SetMonData(mon, MON_DATA_HP_IV + i, &values[i]);
    RecalculateWithoutHealing(mon);
    return TRUE;
}

static s8 CenterIndex(void)
{
    u16 map = (gSaveBlock1Ptr->location.mapGroup << 8) | (u8)gSaveBlock1Ptr->location.mapNum;
    u8 i;
    for (i = 0; i < ARRAY_COUNT(sTrainingCenters); i++)
        if (sTrainingCenters[i] == map)
            return i;
    return -1;
}

static void InitCenterUses(void)
{
    if (gSaveBlock2Ptr->challenge.careMagic != 0x31455243)
    {
        memset(gSaveBlock2Ptr->challenge.centerHeals, 0, sizeof(gSaveBlock2Ptr->challenge.centerHeals));
        gSaveBlock2Ptr->challenge.careMagic = 0x31455243;
    }
}

u8 Training_GetCenterUses(void)
{
    s8 index = CenterIndex();
    if (!Nuzlocke_IsHardcore() || index < 0)
        return 0;
    InitCenterUses();
    return gSaveBlock2Ptr->challenge.centerHeals[index];
}

void Training_CheckCenter(void)
{
    u8 used;
    gSpecialVar_Result = 0; // Normal or a different healing service.
    if (!Nuzlocke_IsHardcore() || CenterIndex() < 0)
        return;
    used = Training_GetCenterUses();
    if (used >= 3)
        gSpecialVar_Result = 2;
    else
    {
        ConvertIntToDecimalStringN(gStringVar1, 3 - used, STR_CONV_MODE_LEFT_ALIGN, 1);
        gSpecialVar_Result = 1;
    }
}

void Training_UseCenter(void)
{
    s8 index = CenterIndex();
    if (Nuzlocke_IsHardcore() && index >= 0)
    {
        InitCenterUses();
        if (gSaveBlock2Ptr->challenge.centerHeals[index] < 3)
            gSaveBlock2Ptr->challenge.centerHeals[index]++;
    }
}

u16 Training_CopyShopStock(const u16 *source, u16 *dest)
{
    u16 count = 0;
    bool8 found = FALSE;
    while (source[count] != ITEM_NONE)
    {
        dest[count] = source[count];
        if (source[count] == ITEM_RARE_CANDY) found = TRUE;
        count++;
    }
    if (Nuzlocke_IsHardcore() && !found) dest[count++] = ITEM_RARE_CANDY;
    dest[count] = ITEM_NONE;
    return count;
}
