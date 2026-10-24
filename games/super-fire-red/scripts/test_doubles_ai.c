#include <assert.h>
#include <stdio.h>
#include "doubles_ai.h"

int main(void)
{
    struct DoublesAIContext state = {1, 1, 1, 1, 0, 0};
    assert(DoublesAI_AdjustScore(100, DOUBLES_MOVE_HELPING_HAND, &state) > 100);
    state.partnerCanAttack = 0;
    assert(DoublesAI_AdjustScore(100, DOUBLES_MOVE_HELPING_HAND, &state) < 100);
    state.partnerCanAttack = 1;
    state.partnerAlive = 0;
    assert(DoublesAI_AdjustScore(100, DOUBLES_MOVE_HELPING_HAND, &state) < 100);
    state.partnerAlive = 1;
    assert(DoublesAI_AdjustScore(0, DOUBLES_MOVE_HELPING_HAND, &state) == 0);
    assert(DoublesAI_AdjustScore(100, DOUBLES_MOVE_FRIENDLY_FIRE, &state) < 100);
    state.partnerImmune = 1;
    assert(DoublesAI_AdjustScore(100, DOUBLES_MOVE_FRIENDLY_FIRE, &state) == 100);
    assert(DoublesAI_AdjustScore(100, DOUBLES_MOVE_SCREEN, &state) > 100);
    state.effectAlreadyActive = 1;
    assert(DoublesAI_AdjustScore(100, DOUBLES_MOVE_SCREEN, &state) < 100);
    assert(DoublesAI_AdjustScore(100, DOUBLES_MOVE_WEATHER, &state) < 100);
    state.effectAlreadyActive = 0;
    assert(DoublesAI_AdjustScore(100, DOUBLES_MOVE_WEATHER, &state) > 100);
    state.effectUseful = 0;
    assert(DoublesAI_AdjustScore(100, DOUBLES_MOVE_WEATHER, &state) < 100);
    assert(DoublesAI_AdjustScore(100, DOUBLES_MOVE_SPEED_CONTROL, &state) < 100);
    assert(DoublesAI_AdjustScore(254, DOUBLES_MOVE_SCREEN, &state) == 255);
    assert(DoublesAI_AdjustScore(1, DOUBLES_MOVE_WEATHER, &state) == 0);
    assert(DoublesAI_AdjustScore(100, DOUBLES_MOVE_SPREAD, &state) > 100);
    state.twoOpponents = 0;
    assert(DoublesAI_AdjustScore(100, DOUBLES_MOVE_SPREAD, &state) == 100);
    state.effectUseful = 0;
    assert(DoublesAI_AdjustScore(100, DOUBLES_MOVE_DAMAGING_SPEED_CONTROL, &state) == 100);
    state.effectUseful = 1;
    assert(DoublesAI_AdjustScore(100, DOUBLES_MOVE_DAMAGING_SPEED_CONTROL, &state) > 100);
    state.effectAlreadyActive = 1;
    assert(DoublesAI_AdjustScore(110, DOUBLES_MOVE_PROTECT, &state) < 100);
    state.effectAlreadyActive = 0;
    state.effectUseful = 0;
    assert(DoublesAI_AdjustScore(100, DOUBLES_MOVE_RECEIVE_HELPING_HAND, &state) < 100);
    state.effectUseful = 1;
    assert(DoublesAI_AdjustScore(100, DOUBLES_MOVE_RECEIVE_HELPING_HAND, &state) > 100);
    assert(DoublesAI_AdjustScore(0, DOUBLES_MOVE_RECEIVE_HELPING_HAND, &state) == 0);
    puts("PASS: partner support, inactive effects, weather, friendly fire, disabled moves and score bounds");
    return 0;
}
