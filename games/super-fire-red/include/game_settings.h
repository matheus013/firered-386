#ifndef GUARD_GAME_SETTINGS_H
#define GUARD_GAME_SETTINGS_H
#include "global.h"
#define GAME_LANGUAGE_EN 0
#define GAME_LANGUAGE_PTBR 1
struct Pokemon;
u8 GameSettings_GetLanguage(void);
void GameSettings_SetLanguage(u8 language);
void GameSettings_ChooseRandomizer(bool8 enabled);
bool8 GameSettings_IsRandomizer(void);
u16 Randomizer_Species(u16 species);
u16 Randomizer_Starter(u8 slot);
void Randomizer_PrepareStarter(void);
void Randomizer_TrainerMon(struct Pokemon *mon, u16 trainer, u8 slot);
#endif
