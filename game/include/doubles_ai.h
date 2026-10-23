#ifndef GUARD_DOUBLES_AI_H
#define GUARD_DOUBLES_AI_H

enum DoublesMoveKind
{
    DOUBLES_MOVE_OTHER,
    DOUBLES_MOVE_HELPING_HAND,
    DOUBLES_MOVE_SPREAD,
    DOUBLES_MOVE_SPEED_CONTROL,
    DOUBLES_MOVE_SCREEN,
    DOUBLES_MOVE_WEATHER,
    DOUBLES_MOVE_FRIENDLY_FIRE,
    DOUBLES_MOVE_DAMAGING_SPEED_CONTROL,
    DOUBLES_MOVE_PROTECT,
    DOUBLES_MOVE_RECEIVE_HELPING_HAND,
};

// Only the AI's own partner and publicly visible battle state are inputs.
struct DoublesAIContext
{
    unsigned char partnerAlive;
    unsigned char partnerCanAttack;
    unsigned char twoOpponents;
    unsigned char effectUseful;
    unsigned char effectAlreadyActive;
    unsigned char partnerImmune;
};

int DoublesAI_AdjustScore(int score, enum DoublesMoveKind kind, const struct DoublesAIContext *context);

#endif
