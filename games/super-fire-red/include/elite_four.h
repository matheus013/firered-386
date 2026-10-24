#ifndef GUARD_ELITE_FOUR_H
#define GUARD_ELITE_FOUR_H

#include "global.h"

struct Pokemon;
bool8 EliteFour_IsChallenge(u16 trainer);
u32 EliteFour_GetPersonality(u16 trainer, u8 slot, u32 personality);
void EliteFour_ApplyTraining(struct Pokemon *mon, u16 trainer, u8 slot);
void EliteFour_ApplyFacilityTraining(struct Pokemon *mon);

#endif
