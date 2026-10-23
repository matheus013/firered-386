#ifndef GUARD_NUZLOCKE_H
#define GUARD_NUZLOCKE_H

#include "global.h"

#define GAME_MODE_NORMAL 0
#define GAME_MODE_HARDCORE 1

struct Pokemon;
bool8 Nuzlocke_IsHardcore(void);
bool8 Nuzlocke_IsActive(void);
void Nuzlocke_ChooseMode(u8 mode);
void Nuzlocke_Start(void);
void Nuzlocke_OnBattleStart(void);
bool8 Nuzlocke_CanCatch(void);
bool8 Nuzlocke_CanUseBattleItem(u16 item);
bool8 Nuzlocke_IsDead(struct Pokemon *mon);
void Nuzlocke_MarkFainted(struct Pokemon *mon);
u8 Nuzlocke_GetLevelCap(void);
bool8 Nuzlocke_IsOverCap(struct Pokemon *mon);
u32 Nuzlocke_ClampExperience(struct Pokemon *mon, u32 experience);
u32 Nuzlocke_ClampTrainingExperience(struct Pokemon *mon, u32 experience);
u16 Nuzlocke_LimitExpGain(struct Pokemon *mon, u32 gain);
void CB2_NuzlockeGameOver(void);

#endif
