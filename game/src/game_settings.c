#include "global.h"
#include "game_settings.h"
#include "random.h"
#define SETTINGS_MAGIC 0x31544553

u8 GameSettings_GetLanguage(void)
{
    if (gSaveBlock2Ptr != NULL && gSaveBlock2Ptr->challenge.settingsMagic == SETTINGS_MAGIC
        && gSaveBlock2Ptr->challenge.language == GAME_LANGUAGE_PTBR)
        return GAME_LANGUAGE_PTBR;
    return GAME_LANGUAGE_EN;
}
void GameSettings_SetLanguage(u8 language)
{
    if (gSaveBlock2Ptr->challenge.settingsMagic != SETTINGS_MAGIC)
    {
        gSaveBlock2Ptr->challenge.randomizer = FALSE;
        gSaveBlock2Ptr->challenge.randomizerSeed = 0;
    }
    gSaveBlock2Ptr->challenge.settingsMagic = SETTINGS_MAGIC;
    gSaveBlock2Ptr->challenge.language = language == GAME_LANGUAGE_PTBR ? GAME_LANGUAGE_PTBR : GAME_LANGUAGE_EN;
}
void GameSettings_ChooseRandomizer(bool8 enabled)
{
    GameSettings_SetLanguage(GameSettings_GetLanguage());
    gSaveBlock2Ptr->challenge.randomizer = enabled != FALSE;
    gSaveBlock2Ptr->challenge.randomizerSeed = enabled ? Random32() | 1 : 0;
}
bool8 GameSettings_IsRandomizer(void)
{
    return gSaveBlock2Ptr != NULL && gSaveBlock2Ptr->challenge.settingsMagic == SETTINGS_MAGIC
        && gSaveBlock2Ptr->challenge.randomizer == TRUE && gSaveBlock2Ptr->challenge.randomizerSeed != 0;
}
