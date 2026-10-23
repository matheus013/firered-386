#include "doubles_ai.h"

int DoublesAI_AdjustScore(int score, enum DoublesMoveKind kind, const struct DoublesAIContext *context)
{
    // A disabled move must never be resurrected by a support bonus.
    if (score == 0)
        return 0;

    switch (kind)
    {
    case DOUBLES_MOVE_HELPING_HAND:
        score += context->partnerAlive && context->partnerCanAttack ? 3 : -40;
        break;
    case DOUBLES_MOVE_SPREAD:
        if (context->twoOpponents)
            score += 2;
        break;
    case DOUBLES_MOVE_SPEED_CONTROL:
        score += context->effectUseful ? 2 : -20;
        break;
    case DOUBLES_MOVE_DAMAGING_SPEED_CONTROL:
        // Rock Tomb / Icy Wind still deal damage at minimum Speed.
        score += context->effectUseful ? 2 : 0;
        break;
    case DOUBLES_MOVE_PROTECT:
        score += context->effectAlreadyActive ? -30 : context->effectUseful ? 1 : -3;
        break;
    case DOUBLES_MOVE_RECEIVE_HELPING_HAND:
        score += context->effectUseful ? 4 : -12;
        break;
    case DOUBLES_MOVE_SCREEN:
        if (context->effectAlreadyActive)
            score -= 40;
        else if (context->partnerAlive)
            score += 3;
        break;
    case DOUBLES_MOVE_WEATHER:
        score += context->effectAlreadyActive ? -40 : context->effectUseful ? 3 : -15;
        break;
    case DOUBLES_MOVE_FRIENDLY_FIRE:
        if (context->partnerAlive && !context->partnerImmune)
            score -= 40;
        break;
    default:
        break;
    }
    if (score < 0)
        score = 0;
    if (score > 255)
        score = 255;
    return score;
}
