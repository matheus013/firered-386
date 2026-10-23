#ifndef GUARD_TRAINING_H
#define GUARD_TRAINING_H
#include "global.h"
struct Pokemon;
bool8 Training_IsUnlocked(void);
bool8 Training_AreAdvancedEditsUnlocked(void);
bool8 Training_CanEdit(struct Pokemon *mon);
bool8 Training_SetNature(struct Pokemon *mon, u8 nature);
bool8 Training_SetEVs(struct Pokemon *mon, const u8 *values);
bool8 Training_SetIVs(struct Pokemon *mon, const u8 *values);
void OpenTrainingMenu(void);
void Training_CheckCenter(void);
void Training_UseCenter(void);
u8 Training_GetCenterUses(void);
u16 Training_CopyShopStock(const u16 *source, u16 *dest);
#endif
