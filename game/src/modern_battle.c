// Generation-nine field effects: terrains, rooms, Tailwind, Gravity and snow.
#include "global.h"
#include "battle.h"
#include "battle_anim.h"
#include "battle_scripts.h"
#include "battle_util.h"
#include "battle_main.h"
#include "modern_battle.h"
#include "constants/abilities.h"
#include "constants/battle_move_effects.h"
#include "constants/battle_string_ids.h"
#include "constants/moves.h"
#include "constants/items.h"
#include "constants/hold_effects.h"
#include "battle_controllers.h"
#include "item.h"
#include "pokemon.h"
#include "random.h"
#include "util.h"

EWRAM_DATA struct ModernBattleState *gModernBattle = NULL;

bool32 IsModernTerrainActive(u8 terrain)
{
    return gModernBattle != NULL && gModernBattle->terrain == terrain;
}

// Grounded Pokémon are affected by terrains; Gravity grounds everyone.
bool32 IsBattlerGroundedModern(u8 battler)
{
    if (gModernBattle != NULL && (gModernBattle->gravityTimer || gModernBattle->smackedDown[battler]))
        return TRUE;
    if (gModernBattle != NULL && gModernBattle->magnetRiseTimer[battler])
        return FALSE;
    if (gStatuses3[battler] & (STATUS3_ON_AIR | STATUS3_UNDERGROUND | STATUS3_UNDERWATER))
        return FALSE;
    if (gBattleMons[battler].type1 == TYPE_FLYING || gBattleMons[battler].type2 == TYPE_FLYING)
        return FALSE;
    if (gBattleMons[battler].ability == ABILITY_LEVITATE)
        return FALSE;
    return TRUE;
}

s8 GetModernMovePriority(u8 battler, u16 move)
{
    s8 priority = gBattleMoves[move].priority;
    if (move == MOVE_GRASSY_GLIDE && IsModernTerrainActive(TERRAIN_GRASSY) && IsBattlerGroundedModern(battler))
        priority++;
    return priority;
}

bool32 IsModernMoveBannedByGravity(u16 move)
{
    if (gModernBattle == NULL || !gModernBattle->gravityTimer)
        return FALSE;
    switch (move)
    {
    case MOVE_FLY:
    case MOVE_BOUNCE:
    case MOVE_JUMP_KICK:
    case MOVE_HI_JUMP_KICK:
    case MOVE_SPLASH:
    case MOVE_MAGNET_RISE:
        return TRUE;
    }
    return FALSE;
}

// Messages printed by trysetfieldeffect, indexed by the multistring chooser.
const u16 gModernFieldStartStringIds[] =
{
    STRINGID_ELECTRICTERRAINSTART, STRINGID_GRASSYTERRAINSTART, STRINGID_MISTYTERRAINSTART, STRINGID_PSYCHICTERRAINSTART,
    STRINGID_TRICKROOMSTART, STRINGID_TRICKROOMEND, STRINGID_TAILWINDSTART, STRINGID_GRAVITYSTART,
    STRINGID_MAGICROOMSTART, STRINGID_WONDERROOMSTART, STRINGID_SNOWSTART, STRINGID_MAGICROOMEND,
    STRINGID_WONDERROOMEND,
};

const u16 gModernHazardStringIds[] = {STRINGID_STEALTHROCKSET, STRINGID_TOXICSPIKESSET, STRINGID_STICKYWEBSET};

// Messages printed when a field effect runs out, indexed by the multistring chooser.
const u16 gModernFieldEndStringIds[] =
{
    STRINGID_TERRAINEND, STRINGID_TRICKROOMEND, STRINGID_TAILWINDEND, STRINGID_GRAVITYEND,
    STRINGID_MAGICROOMEND, STRINGID_WONDERROOMEND, STRINGID_SNOWEND,
};

// trysetfieldeffect failPtr: starts the field effect of the current move.
void Cmd_trysetfieldeffect(void)
{
    const u8 *failPtr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    u8 side = GetBattlerSide(gBattlerAttacker);
    bool8 failed = FALSE;
    u8 message = 0;

    switch (gCurrentMove)
    {
    case MOVE_ELECTRIC_TERRAIN:
    case MOVE_GRASSY_TERRAIN:
    case MOVE_MISTY_TERRAIN:
    case MOVE_PSYCHIC_TERRAIN:
    {
        u8 terrain = gCurrentMove == MOVE_ELECTRIC_TERRAIN ? TERRAIN_ELECTRIC
                   : gCurrentMove == MOVE_GRASSY_TERRAIN ? TERRAIN_GRASSY
                   : gCurrentMove == MOVE_MISTY_TERRAIN ? TERRAIN_MISTY : TERRAIN_PSYCHIC;
        if (gModernBattle->terrain == terrain)
            failed = TRUE;
        gModernBattle->terrain = terrain;
        gModernBattle->terrainTimer = 5;
        message = terrain - 1;
        break;
    }
    case MOVE_TRICK_ROOM:
        // Using it again while active twists the dimensions back.
        if (gModernBattle->trickRoomTimer)
        {
            gModernBattle->trickRoomTimer = 0;
            message = 5;
        }
        else
        {
            gModernBattle->trickRoomTimer = 5;
            message = 4;
        }
        break;
    case MOVE_TAILWIND:
        if (gModernBattle->tailwindTimer[side])
            failed = TRUE;
        gModernBattle->tailwindTimer[side] = 4;
        message = 6;
        break;
    case MOVE_GRAVITY:
        if (gModernBattle->gravityTimer)
            failed = TRUE;
        gModernBattle->gravityTimer = 5;
        message = 7;
        break;
    case MOVE_MAGIC_ROOM:
        // Like Trick Room, using it again ends it.
        message = gModernBattle->magicRoomTimer ? 11 : 8;
        gModernBattle->magicRoomTimer = gModernBattle->magicRoomTimer ? 0 : 5;
        break;
    case MOVE_WONDER_ROOM:
        message = gModernBattle->wonderRoomTimer ? 12 : 9;
        gModernBattle->wonderRoomTimer = gModernBattle->wonderRoomTimer ? 0 : 5;
        break;
    case MOVE_SNOWSCAPE:
        if (gModernBattle->snow && gBattleWeather & B_WEATHER_HAIL)
            failed = TRUE;
        gBattleWeather = B_WEATHER_HAIL_TEMPORARY;
        gWishFutureKnock.weatherDuration = 5;
        gModernBattle->snow = TRUE;
        message = 10;
        break;
    default:
        failed = TRUE;
        break;
    }

    gBattleCommunication[MULTISTRING_CHOOSER] = message;
    if (failed)
        gBattlescriptCurrInstr = failPtr;
    else
        gBattlescriptCurrInstr += 5;
}

// End of turn, once: count every field timer down and queue the messages of those that ran out.
void DecrementModernFieldTimers(void)
{
    u8 side;

    UpdateModernBattlerStatesEndTurn();

    if (gModernBattle->terrainTimer && --gModernBattle->terrainTimer == 0)
    {
        gModernBattle->terrain = TERRAIN_NONE;
        gModernBattle->pendingEnds |= 1 << 0;
    }
    if (gModernBattle->trickRoomTimer && --gModernBattle->trickRoomTimer == 0)
        gModernBattle->pendingEnds |= 1 << 1;
    for (side = 0; side < 2; side++)
        if (gModernBattle->tailwindTimer[side] && --gModernBattle->tailwindTimer[side] == 0)
            gModernBattle->pendingEnds |= 1 << (2 + side);
    if (gModernBattle->gravityTimer && --gModernBattle->gravityTimer == 0)
        gModernBattle->pendingEnds |= 1 << 4;
    if (gModernBattle->magicRoomTimer && --gModernBattle->magicRoomTimer == 0)
        gModernBattle->pendingEnds |= 1 << 5;
    if (gModernBattle->wonderRoomTimer && --gModernBattle->wonderRoomTimer == 0)
        gModernBattle->pendingEnds |= 1 << 6;
}

// Prints one queued "wore off" message; returns FALSE when none is left.
bool32 TryPrintModernFieldEnd(void)
{
    static const u8 sEndMessage[] = {0, 1, 2, 2, 3, 4, 5};
    u8 bit;

    for (bit = 0; bit < ARRAY_COUNT(sEndMessage); bit++)
    {
        if (gModernBattle->pendingEnds & (1 << bit))
        {
            gModernBattle->pendingEnds &= ~(1 << bit);
            if (bit == 2 || bit == 3)
                gBattlerAttacker = bit - 2; // a battler of that side, for the team prefix
            gBattleCommunication[MULTISTRING_CHOOSER] = sEndMessage[bit];
            BattleScriptExecute(BattleScript_ModernFieldEnds);
            return TRUE;
        }
    }
    return FALSE;
}

// Stealth Rock deals 1/8 of the max HP, scaled by the Rock-type effectiveness against the battler.
u32 GetStealthRockDamage(u8 battler)
{
    u32 multiplier = 100, i; // hundredths, so two resistances give exactly 1/4
    u32 damage;

    for (i = 0; gTypeEffectiveness[i] != TYPE_ENDTABLE; i += 3)
    {
        if (gTypeEffectiveness[i] == TYPE_FORESIGHT || gTypeEffectiveness[i] != TYPE_ROCK)
            continue;
        if (gTypeEffectiveness[i + 1] == gBattleMons[battler].type1)
            multiplier = multiplier * gTypeEffectiveness[i + 2] / 10;
        if (gTypeEffectiveness[i + 1] == gBattleMons[battler].type2 && gBattleMons[battler].type1 != gBattleMons[battler].type2)
            multiplier = multiplier * gTypeEffectiveness[i + 2] / 10;
    }
    damage = gBattleMons[battler].maxHP * multiplier / 800;
    return damage ? damage : 1;
}

// ---------------------------------------------------------------- per-battler state (batch B4c-e)

#define CURRENT_TURN (gBattleResults.battleTurnCounter)

static u8 ModernPartyIndex(u8 battler)
{
    return gBattlerPartyIndexes[battler] % PARTY_SIZE;
}

// Clears the state of the battler slot when a new Pokémon comes in; Baton Pass keeps Aqua Ring and Magnet Rise.
void ClearModernBattlerState(u8 battler, bool32 batonPass)
{
    if (gModernBattle == NULL)
        return;
    gModernBattle->roosted[battler] = 0;
    if (!batonPass)
    {
        gModernBattle->aquaRing[battler] = 0;
        gModernBattle->magnetRiseTimer[battler] = 0;
    }
    gModernBattle->laserFocusTurn[battler] = 0;
    gModernBattle->throatChopTurn[battler] = 0;
    gModernBattle->healBlockTurn[battler] = 0;
    gModernBattle->smackedDown[battler] = 0;
    gModernBattle->lastMoveFailed[battler] = 0;
    gModernBattle->movesUsed[battler] = 0;
    gModernBattle->weightLoss[battler] = 0;
    gModernBattle->critBoost[battler] = 0;
    gModernBattle->statsRaisedTurn[battler] = 0;
    gModernBattle->statsLoweredTurn[battler] = 0;
    gModernBattle->spikyShieldTurn[battler] = 0;
}

// End of turn: Roost types come back; timed states count down.
void UpdateModernBattlerStatesEndTurn(void)
{
    u8 battler;

    for (battler = 0; battler < gBattlersCount; battler++)
    {
        if (gModernBattle->roosted[battler])
        {
            gBattleMons[battler].type1 = gModernBattle->roostTypes[battler][0];
            gBattleMons[battler].type2 = gModernBattle->roostTypes[battler][1];
            gModernBattle->roosted[battler] = 0;
        }
        if (gModernBattle->magnetRiseTimer[battler])
            gModernBattle->magnetRiseTimer[battler]--;
    }
}

bool32 IsModernStateActiveThisTurn(u8 marker)
{
    // Markers store the last turn (plus one) during which a state is still active.
    return marker != 0 && CURRENT_TURN + 1 <= marker;
}

bool32 IsBattlerMagnetRisen(u8 battler)
{
    return gModernBattle != NULL && gModernBattle->magnetRiseTimer[battler] && !gModernBattle->smackedDown[battler] && !gModernBattle->gravityTimer;
}

// Gravity and Smack Down let Ground moves hit Flying types.
bool32 IsBattlerForcedGrounded(u8 battler)
{
    return gModernBattle != NULL && (gModernBattle->gravityTimer || gModernBattle->smackedDown[battler]);
}

bool32 IsModernSoundMove(u16 move)
{
    return (gBattleMoves[move].flags & FLAG_SOUND) != 0;
}

bool32 IsModernHealingMove(u16 move)
{
    switch (gBattleMoves[move].effect)
    {
    case EFFECT_RESTORE_HP:
    case EFFECT_SOFTBOILED:
    case EFFECT_MORNING_SUN:
    case EFFECT_SYNTHESIS:
    case EFFECT_MOONLIGHT:
    case EFFECT_REST:
    case EFFECT_SWALLOW:
    case EFFECT_WISH:
    case EFFECT_LIFE_DEW:
        return TRUE;
    }
    return move == MOVE_ROOST || move == MOVE_AQUA_RING || move == MOVE_HEALING_WISH;
}

// Moves that a battler cannot use now (Throat Chop, Psychic Noise, Gravity).
bool32 IsModernMoveBlocked(u8 battler, u16 move)
{
    if (gModernBattle == NULL)
        return FALSE;
    if (IsModernStateActiveThisTurn(gModernBattle->throatChopTurn[battler]) && IsModernSoundMove(move))
        return TRUE;
    if (IsModernStateActiveThisTurn(gModernBattle->healBlockTurn[battler]) && IsModernHealingMove(move))
        return TRUE;
    if (move == MOVE_BELCH && !(gModernBattle->ateBerry[GetBattlerSide(battler)] & (1 << ModernPartyIndex(battler))))
        return TRUE; // Belch needs a Berry eaten first
    return IsModernMoveBannedByGravity(move);
}

// Levitate and Magnet Rise dodge Ground moves unless Gravity or Smack Down grounds the battler.
bool32 IsModernLevitating(u8 battler)
{
    if (IsBattlerForcedGrounded(battler))
        return FALSE;
    return gBattleMons[battler].ability == ABILITY_LEVITATE || IsBattlerMagnetRisen(battler);
}

void RecordModernStatChange(u8 battler, bool32 raised)
{
    if (gModernBattle == NULL)
        return;
    if (raised)
        gModernBattle->statsRaisedTurn[battler] = CURRENT_TURN + 1;
    else
        gModernBattle->statsLoweredTurn[battler] = CURRENT_TURN + 1;
}

void RecordModernFaint(u8 battler)
{
    if (gModernBattle != NULL)
        gModernBattle->allyFaintedTurn[GetBattlerSide(battler)] = CURRENT_TURN + 1;
}

void RecordModernHitTaken(u8 battler)
{
    u8 side = GetBattlerSide(battler), slot = ModernPartyIndex(battler);
    if (gModernBattle != NULL && gModernBattle->rageFistHits[side][slot] < 6)
        gModernBattle->rageFistHits[side][slot]++;
}

void RecordModernBerryEaten(u8 battler)
{
    if (gModernBattle != NULL)
        gModernBattle->ateBerry[GetBattlerSide(battler)] |= 1 << ModernPartyIndex(battler);
}

// End of a move: failure flag (Stomping Tantrum), moves used (Last Resort), last move (Copycat), Echoed Voice and Round.
void RecordModernMoveUse(u8 battler, u16 chosenMove, u16 move, u8 resultFlags)
{
    u8 i;

    if (gModernBattle == NULL || move == MOVE_NONE || move == MOVE_UNAVAILABLE)
        return;
    gModernBattle->lastMoveFailed[battler] = (resultFlags & MOVE_RESULT_NO_EFFECT) != 0;
    for (i = 0; i < MAX_MON_MOVES; i++)
        if (gBattleMons[battler].moves[i] == chosenMove)
            gModernBattle->movesUsed[battler] |= 1 << i;
    if (move != MOVE_COPYCAT && move != MOVE_STRUGGLE)
        gModernBattle->lastMoveUsed = move;
    if (move == MOVE_ECHOED_VOICE)
    {
        if (gModernBattle->echoedVoiceTurn == CURRENT_TURN && gModernBattle->echoedVoiceCount < 5)
            gModernBattle->echoedVoiceCount++;
        else if (gModernBattle->echoedVoiceTurn != CURRENT_TURN + 1)
            gModernBattle->echoedVoiceCount = 1;
        gModernBattle->echoedVoiceTurn = CURRENT_TURN + 1;
    }
    if (move == MOVE_ROUND)
        gModernBattle->roundTurn[GetBattlerSide(battler)] = CURRENT_TURN + 1;
}

// Power of the moves that remember what happened this turn or earlier; 0 keeps the listed power.
u16 GetModernMemoryMovePower(u16 move, u8 battlerAtk)
{
    u8 side = GetBattlerSide(battlerAtk);
    u8 mult;

    if (gModernBattle == NULL)
        return 0;
    switch (move)
    {
    case MOVE_STOMPING_TANTRUM:
    case MOVE_TEMPER_FLARE:
        return gModernBattle->lastMoveFailed[battlerAtk] ? gBattleMoves[move].power * 2 : 0;
    case MOVE_LASH_OUT:
        return gModernBattle->statsLoweredTurn[battlerAtk] == CURRENT_TURN + 1 ? gBattleMoves[move].power * 2 : 0;
    case MOVE_RETALIATE:
        return gModernBattle->allyFaintedTurn[side] == CURRENT_TURN ? gBattleMoves[move].power * 2 : 0;
    case MOVE_ROUND:
        return gModernBattle->roundTurn[side] == CURRENT_TURN + 1 ? gBattleMoves[move].power * 2 : 0;
    case MOVE_ECHOED_VOICE:
        if (gModernBattle->echoedVoiceTurn == CURRENT_TURN)
            mult = gModernBattle->echoedVoiceCount + 1;
        else if (gModernBattle->echoedVoiceTurn == CURRENT_TURN + 1)
            mult = gModernBattle->echoedVoiceCount;
        else
            mult = 1;
        if (mult > 5)
            mult = 5;
        return gBattleMoves[move].power * mult;
    case MOVE_RAGE_FIST:
        return gBattleMoves[move].power * (1 + gModernBattle->rageFistHits[side][ModernPartyIndex(battlerAtk)]);
    }
    return 0;
}

bool32 IsModernLaserFocused(u8 battler)
{
    return gModernBattle != NULL && IsModernStateActiveThisTurn(gModernBattle->laserFocusTurn[battler]);
}

u8 GetModernCritBoost(u8 battler)
{
    return gModernBattle != NULL ? gModernBattle->critBoost[battler] : 0;
}

u32 GetModernWeightLoss(u8 battler)
{
    return gModernBattle != NULL ? gModernBattle->weightLoss[battler] * 1000 : 0; // 100 kg per Autotomize, in tenths of kg
}

// ---------------------------------------------------------------- modernstatusmove modes 10+ (batch B4c-e)

// Messages printed with printfromtable after a mode, indexed by the multistring chooser.
const u16 gModernMiscStringIds[] =
{
    [MODERN_MSG_SIDE_GUARD]    = STRINGID_MODERNSIDEGUARD,
    [MODERN_MSG_AFTER_YOU]     = STRINGID_MODERNAFTERYOU,
    [MODERN_MSG_QUASH]         = STRINGID_MODERNQUASH,
    [MODERN_MSG_AQUA_RING]     = STRINGID_MODERNAQUARING,
    [MODERN_MSG_MAGNET_RISE]   = STRINGID_MODERNMAGNETRISE,
    [MODERN_MSG_LASER_FOCUS]   = STRINGID_MODERNLASERFOCUS,
    [MODERN_MSG_SMACK_DOWN]    = STRINGID_MODERNSMACKDOWN,
    [MODERN_MSG_HEAL_BLOCK]    = STRINGID_MODERNHEALBLOCK,
    [MODERN_MSG_INCINERATE]    = STRINGID_MODERNINCINERATE,
    [MODERN_MSG_BUG_BITE]      = STRINGID_MODERNBUGBITE,
    [MODERN_MSG_DRAGON_CHEER]  = STRINGID_MODERNDRAGONCHEER,
    [MODERN_MSG_AUTOTOMIZE]    = STRINGID_MODERNAUTOTOMIZE,
};

static bool32 IsModernBerry(u16 item)
{
    return item != ITEM_NONE && ItemId_GetPocket(item) == POCKET_BERRY_POUCH;
}

static void SetModernHeldItem(u8 battler, u16 item)
{
    gBattleMons[battler].item = item;
    gActiveBattler = battler;
    BtlController_EmitSetMonData(BUFFER_A, REQUEST_HELDITEM_BATTLE, 0, sizeof(item), &gBattleMons[battler].item);
    MarkBattlerForControllerExec(battler);
}

static void SetModernStatus1(u8 battler, u32 status)
{
    gBattleMons[battler].status1 = status;
    gActiveBattler = battler;
    BtlController_EmitSetMonData(BUFFER_A, REQUEST_STATUS_BATTLE, 0, sizeof(status), &gBattleMons[battler].status1);
    MarkBattlerForControllerExec(battler);
}

static u8 GetModernPartner(u8 battler)
{
    u8 partner = battler ^ BIT_FLANK;
    if (!(gBattleTypeFlags & BATTLE_TYPE_DOUBLE) || gAbsentBattlerFlags & gBitTable[partner] || gBattleMons[partner].hp == 0)
        return MAX_BATTLERS_COUNT;
    return partner;
}

// Moves the target's pending action next (After You) or last (Quash) in the turn order.
static bool32 ModernChangeTurnOrder(bool32 last)
{
    u8 i, from = MAX_BATTLERS_COUNT, to, battler, action;

    for (i = gCurrentTurnActionNumber + 1; i < gBattlersCount; i++)
        if (gBattlerByTurnOrder[i] == gBattlerTarget && gActionsByTurnOrder[i] == B_ACTION_USE_MOVE)
            from = i;
    if (from == MAX_BATTLERS_COUNT)
        return FALSE;
    to = last ? gBattlersCount - 1 : gCurrentTurnActionNumber + 1;
    battler = gBattlerByTurnOrder[from];
    action = gActionsByTurnOrder[from];
    for (; from < to; from++)
    {
        gBattlerByTurnOrder[from] = gBattlerByTurnOrder[from + 1];
        gActionsByTurnOrder[from] = gActionsByTurnOrder[from + 1];
    }
    for (; from > to; from--)
    {
        gBattlerByTurnOrder[from] = gBattlerByTurnOrder[from - 1];
        gActionsByTurnOrder[from] = gActionsByTurnOrder[from - 1];
    }
    gBattlerByTurnOrder[to] = battler;
    gActionsByTurnOrder[to] = action;
    return TRUE;
}

// Bug Bite and Pluck: the user eats the target's Berry and gets its effect whatever its HP.
static bool32 ModernEatTargetBerry(void)
{
    u8 atk = gBattlerAttacker, i;
    u16 berry = gBattleMons[gBattlerTarget].item;
    u8 holdEffect = ItemId_GetHoldEffect(berry), param = ItemId_GetHoldEffectParam(berry);
    struct BattlePokemon *mon = &gBattleMons[atk];
    u32 status = mon->status1;
    s32 heal = 0;

    if (!IsModernBerry(berry) || gBattleMons[atk].hp == 0)
        return FALSE;
    gLastUsedItem = berry;
    SetModernHeldItem(gBattlerTarget, ITEM_NONE);
    RecordModernBerryEaten(atk);
    switch (holdEffect)
    {
    case HOLD_EFFECT_RESTORE_HP:
        heal = param;
        break;
    case HOLD_EFFECT_CONFUSE_SPICY:
    case HOLD_EFFECT_CONFUSE_DRY:
    case HOLD_EFFECT_CONFUSE_SWEET:
    case HOLD_EFFECT_CONFUSE_BITTER:
    case HOLD_EFFECT_CONFUSE_SOUR:
        heal = mon->maxHP / param;
        if (heal == 0)
            heal = 1;
        if (GetFlavorRelationByPersonality(mon->personality, holdEffect - HOLD_EFFECT_CONFUSE_SPICY) < 0)
            mon->status2 |= STATUS2_CONFUSION_TURN(((Random()) % 4) + 2);
        break;
    case HOLD_EFFECT_CURE_PAR:
        status &= ~STATUS1_PARALYSIS;
        break;
    case HOLD_EFFECT_CURE_SLP:
        status &= ~STATUS1_SLEEP;
        mon->status2 &= ~STATUS2_NIGHTMARE;
        break;
    case HOLD_EFFECT_CURE_PSN:
        status &= ~(STATUS1_PSN_ANY | STATUS1_TOXIC_COUNTER);
        break;
    case HOLD_EFFECT_CURE_BRN:
        status &= ~STATUS1_BURN;
        break;
    case HOLD_EFFECT_CURE_FRZ:
        status &= ~STATUS1_FREEZE;
        break;
    case HOLD_EFFECT_CURE_CONFUSION:
        mon->status2 &= ~STATUS2_CONFUSION;
        break;
    case HOLD_EFFECT_CURE_STATUS:
        status = 0;
        mon->status2 &= ~(STATUS2_CONFUSION | STATUS2_NIGHTMARE);
        break;
    case HOLD_EFFECT_RESTORE_PP:
        for (i = 0; i < MAX_MON_MOVES; i++)
        {
            u8 maxPP = CalculatePPWithBonus(mon->moves[i], mon->ppBonuses, i);
            if (mon->moves[i] != MOVE_NONE && mon->pp[i] < maxPP)
            {
                mon->pp[i] = mon->pp[i] + param > maxPP ? maxPP : mon->pp[i] + param;
                gActiveBattler = atk;
                BtlController_EmitSetMonData(BUFFER_A, REQUEST_PPMOVE1_BATTLE + i, 0, 1, &mon->pp[i]);
                MarkBattlerForControllerExec(atk);
                break;
            }
        }
        break;
    case HOLD_EFFECT_ATTACK_UP:
    case HOLD_EFFECT_DEFENSE_UP:
    case HOLD_EFFECT_SPEED_UP:
    case HOLD_EFFECT_SP_ATTACK_UP:
    case HOLD_EFFECT_SP_DEFENSE_UP:
        i = STAT_ATK + holdEffect - HOLD_EFFECT_ATTACK_UP;
        if (mon->statStages[i] < MAX_STAT_STAGE)
            mon->statStages[i]++;
        break;
    case HOLD_EFFECT_CRITICAL_UP:
        mon->status2 |= STATUS2_FOCUS_ENERGY;
        break;
    case HOLD_EFFECT_RANDOM_STAT_UP:
        i = STAT_ATK + Random() % (NUM_STATS - 1);
        mon->statStages[i] = mon->statStages[i] + 2 > MAX_STAT_STAGE ? MAX_STAT_STAGE : mon->statStages[i] + 2;
        break;
    }
    if (status != mon->status1)
        SetModernStatus1(atk, status);
    if (heal > mon->maxHP - mon->hp)
        heal = mon->maxHP - mon->hp;
    gBattleMoveDamage = -heal;
    gBattleCommunication[MULTISTRING_CHOOSER] = MODERN_MSG_BUG_BITE;
    return TRUE;
}

// Checks done before the hit; returning FALSE makes the move fail.
static bool32 ModernBeforeHit(void)
{
    u8 i, known = 0, used = 0;

    switch (gCurrentMove)
    {
    case MOVE_LAST_RESORT: // every other known move must have been used since switching in
        for (i = 0; i < MAX_MON_MOVES; i++)
        {
            if (gBattleMons[gBattlerAttacker].moves[i] == MOVE_NONE || gBattleMons[gBattlerAttacker].moves[i] == MOVE_LAST_RESORT)
                continue;
            known++;
            if (gModernBattle->movesUsed[gBattlerAttacker] & (1 << i))
                used++;
        }
        return known != 0 && known == used;
    case MOVE_POLTERGEIST:
        return gBattleMons[gBattlerTarget].item != ITEM_NONE;
    }
    return TRUE;
}

// Effects applied after a damaging hit; returning TRUE prints the message picked in the multistring chooser.
static bool32 ModernAfterHit(void)
{
    u8 target = gBattlerTarget;

    switch (gCurrentMove)
    {
    case MOVE_THROAT_CHOP:
        gModernBattle->throatChopTurn[target] = CURRENT_TURN + 2;
        return FALSE;
    case MOVE_PSYCHIC_NOISE:
        if (gBattleMons[target].hp == 0)
            return FALSE;
        gModernBattle->healBlockTurn[target] = CURRENT_TURN + 2;
        gBattleCommunication[MULTISTRING_CHOOSER] = MODERN_MSG_HEAL_BLOCK;
        return TRUE;
    case MOVE_SMACK_DOWN:
    {
        bool32 airborne = !IsBattlerGroundedModern(target);
        if (gBattleMons[target].hp == 0)
            return FALSE;
        gModernBattle->smackedDown[target] = TRUE;
        gModernBattle->magnetRiseTimer[target] = 0;
        gBattleCommunication[MULTISTRING_CHOOSER] = MODERN_MSG_SMACK_DOWN;
        return airborne;
    }
    case MOVE_INCINERATE:
        if (!IsModernBerry(gBattleMons[target].item))
            return FALSE;
        gLastUsedItem = gBattleMons[target].item;
        SetModernHeldItem(target, ITEM_NONE);
        gBattleCommunication[MULTISTRING_CHOOSER] = MODERN_MSG_INCINERATE;
        return TRUE;
    case MOVE_BURNING_JEALOUSY:
    case MOVE_ALLURING_VOICE:
        // Only targets whose stats rose during this turn are burned or confused.
        if (gModernBattle->statsRaisedTurn[target] == CURRENT_TURN + 1)
            gBattleCommunication[MOVE_EFFECT_BYTE] = MOVE_EFFECT_CERTAIN
                | (gCurrentMove == MOVE_BURNING_JEALOUSY ? MOVE_EFFECT_BURN : MOVE_EFFECT_CONFUSION);
        return FALSE;
    }
    return FALSE;
}

// Finds the next Pokémon with Plus or Minus on the user's side, from the user onwards.
static bool32 ModernNextMagneticFlux(void)
{
    u8 candidates[2];

    candidates[0] = gBattlerAttacker;
    candidates[1] = GetModernPartner(gBattlerAttacker);

    for (; gModernBattle->scriptIter < 2; gModernBattle->scriptIter++)
    {
        u8 battler = candidates[gModernBattle->scriptIter];
        if (battler < MAX_BATTLERS_COUNT && (gBattleMons[battler].ability == ABILITY_PLUS || gBattleMons[battler].ability == ABILITY_MINUS))
        {
            gBattlerTarget = battler;
            gModernBattle->scriptIter++;
            return TRUE;
        }
    }
    return FALSE;
}

// Modes of modernstatusmove that belong to batch B4c-e. Returns FALSE when the move fails
// (or, for the after-hit modes, when there is nothing to print).
bool32 DoModernStatusMode(u8 mode)
{
    u8 atk = gBattlerAttacker, side = GetBattlerSide(gBattlerAttacker), partner;

    switch (mode)
    {
    case MSM_SIDE_GUARD:
        gModernBattle->sideGuardTurn[side][gCurrentMove == MOVE_WIDE_GUARD] = CURRENT_TURN + 1;
        gBattleCommunication[MULTISTRING_CHOOSER] = MODERN_MSG_SIDE_GUARD;
        return TRUE;
    case MSM_TURN_ORDER:
        gBattleCommunication[MULTISTRING_CHOOSER] = gCurrentMove == MOVE_QUASH ? MODERN_MSG_QUASH : MODERN_MSG_AFTER_YOU;
        return ModernChangeTurnOrder(gCurrentMove == MOVE_QUASH);
    case MSM_ROOST:
        // The user loses its Flying type until the end of the turn; a pure Flying type becomes Normal.
        if (!IS_BATTLER_OF_TYPE(atk, TYPE_FLYING))
            return TRUE;
        gModernBattle->roosted[atk] = TRUE;
        gModernBattle->roostTypes[atk][0] = gBattleMons[atk].type1;
        gModernBattle->roostTypes[atk][1] = gBattleMons[atk].type2;
        if (gBattleMons[atk].type1 == TYPE_FLYING && gBattleMons[atk].type2 == TYPE_FLYING)
            gBattleMons[atk].type1 = gBattleMons[atk].type2 = TYPE_NORMAL;
        else if (gBattleMons[atk].type1 == TYPE_FLYING)
            gBattleMons[atk].type1 = gBattleMons[atk].type2;
        else
            gBattleMons[atk].type2 = gBattleMons[atk].type1;
        return TRUE;
    case MSM_AQUA_RING:
        if (gModernBattle->aquaRing[atk])
            return FALSE;
        gModernBattle->aquaRing[atk] = TRUE;
        gBattleCommunication[MULTISTRING_CHOOSER] = MODERN_MSG_AQUA_RING;
        return TRUE;
    case MSM_MAGNET_RISE:
        if (gModernBattle->magnetRiseTimer[atk] || gModernBattle->smackedDown[atk] || gModernBattle->gravityTimer
         || gStatuses3[atk] & STATUS3_ROOTED)
            return FALSE;
        gModernBattle->magnetRiseTimer[atk] = 5;
        gBattleCommunication[MULTISTRING_CHOOSER] = MODERN_MSG_MAGNET_RISE;
        return TRUE;
    case MSM_LASER_FOCUS:
        gModernBattle->laserFocusTurn[atk] = CURRENT_TURN + 2; // this turn and the next one
        gBattleCommunication[MULTISTRING_CHOOSER] = MODERN_MSG_LASER_FOCUS;
        return TRUE;
    case MSM_BEFORE_HIT:
        return ModernBeforeHit();
    case MSM_AFTER_HIT:
        return ModernAfterHit();
    case MSM_HEALING_WISH:
        gModernBattle->healingWish[side] = TRUE;
        return TRUE;
    case MSM_BUG_BITE:
        return ModernEatTargetBerry();
    case MSM_ALLY_TARGET:
        partner = GetModernPartner(atk);
        if (partner == MAX_BATTLERS_COUNT)
            return FALSE;
        gBattlerTarget = partner;
        if (gCurrentMove == MOVE_DRAGON_CHEER)
        {
            if (gModernBattle->critBoost[partner] || gBattleMons[partner].status2 & STATUS2_FOCUS_ENERGY)
                return FALSE;
            gModernBattle->critBoost[partner] = IS_BATTLER_OF_TYPE(partner, TYPE_DRAGON) ? 2 : 1;
            gBattleCommunication[MULTISTRING_CHOOSER] = MODERN_MSG_DRAGON_CHEER;
        }
        return TRUE;
    case MSM_MAGNETIC_FLUX:
        gModernBattle->scriptIter = 0;
        return ModernNextMagneticFlux();
    case MSM_MAGNETIC_FLUX_NEXT:
        return ModernNextMagneticFlux();
    case MSM_AUTOTOMIZE:
        if (gModernBattle->weightLoss[atk] < 0xFF)
            gModernBattle->weightLoss[atk]++;
        gBattleCommunication[MULTISTRING_CHOOSER] = MODERN_MSG_AUTOTOMIZE;
        return TRUE;
    }
    return FALSE;
}

// EFFECT_MODERN_MISC holds several moves; moderndispatch jumps to the script of each one.
const u8 *GetModernDispatchScript(u16 move)
{
    switch (move)
    {
    case MOVE_QUICK_GUARD:
    case MOVE_WIDE_GUARD:
        return BattleScript_EffectSideGuard;
    case MOVE_AFTER_YOU:
    case MOVE_QUASH:
        return BattleScript_EffectTurnOrder;
    case MOVE_ROOST:
        return BattleScript_EffectRoost;
    case MOVE_AQUA_RING:
        return BattleScript_EffectAquaRing;
    case MOVE_MAGNET_RISE:
        return BattleScript_EffectMagnetRise;
    case MOVE_LASER_FOCUS:
        return BattleScript_EffectLaserFocus;
    case MOVE_HEALING_WISH:
        return BattleScript_EffectHealingWish;
    case MOVE_COPYCAT:
        return BattleScript_EffectCopycat;
    case MOVE_SCALE_SHOT:
        return BattleScript_EffectScaleShot;
    case MOVE_BUG_BITE:
    case MOVE_PLUCK:
        return BattleScript_EffectBugBite;
    case MOVE_COACHING:
        return BattleScript_EffectCoaching;
    case MOVE_DRAGON_CHEER:
        return BattleScript_EffectDragonCheer;
    case MOVE_MAGNETIC_FLUX:
        return BattleScript_EffectMagneticFlux;
    case MOVE_AUTOTOMIZE:
        return BattleScript_EffectAutotomize;
    }
    // Throat Chop, Psychic Noise, Smack Down, Incinerate, Burning Jealousy, Alluring Voice, Last Resort, Poltergeist
    return BattleScript_EffectModernHit;
}
