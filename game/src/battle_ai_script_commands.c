#include "global.h"
#include "battle.h"
#include "battle_anim.h"
#include "util.h"
#include "item.h"
#include "random.h"
#include "battle_ai_script_commands.h"
#include "doubles_ai.h"
#include "constants/abilities.h"
#include "constants/battle_ai.h"
#include "constants/battle_move_effects.h"
#include "constants/moves.h"

#define AI_ACTION_DONE          0x0001
#define AI_ACTION_FLEE          0x0002
#define AI_ACTION_WATCH         0x0004
#define AI_ACTION_DO_NOT_ATTACK 0x0008
#define AI_ACTION_UNK5          0x0010
#define AI_ACTION_UNK6          0x0020
#define AI_ACTION_UNK7          0x0040
#define AI_ACTION_UNK8          0x0080

#define AI_THINKING_STRUCT (gBattleResources->ai)
#define BATTLE_HISTORY (gBattleResources->battleHistory)

// AI states
enum
{
    AIState_SettingUp,
    AIState_Processing,
    AIState_FinishedProcessing,
    AIState_DoNotProcess
};

/*
sAIScriptPtr is a pointer to the next battle AI cmd command to read.
when a command finishes processing, sAIScriptPtr is incremented by
the number of bytes that the current command had reserved for arguments
in order to read the next command correctly. refer to battle_ai_scripts.s for the
AI scripts.
*/

static EWRAM_DATA const u8 *sAIScriptPtr = NULL;
extern u8 *gBattleAI_ScriptsTable[];

static void Cmd_if_random_less_than(void);
static void Cmd_if_random_greater_than(void);
static void Cmd_if_random_equal(void);
static void Cmd_if_random_not_equal(void);
static void Cmd_score(void);
static void Cmd_if_hp_less_than(void);
static void Cmd_if_hp_more_than(void);
static void Cmd_if_hp_equal(void);
static void Cmd_if_hp_not_equal(void);
static void Cmd_if_status(void);
static void Cmd_if_not_status(void);
static void Cmd_if_status2(void);
static void Cmd_if_not_status2(void);
static void Cmd_if_status3(void);
static void Cmd_if_not_status3(void);
static void Cmd_if_side_affecting(void);
static void Cmd_if_not_side_affecting(void);
static void Cmd_if_less_than(void);
static void Cmd_if_more_than(void);
static void Cmd_if_equal(void);
static void Cmd_if_not_equal(void);
static void Cmd_if_less_than_ptr(void);
static void Cmd_if_more_than_ptr(void);
static void Cmd_if_equal_ptr(void);
static void Cmd_if_not_equal_ptr(void);
static void Cmd_if_move(void);
static void Cmd_if_not_move(void);
static void Cmd_if_in_bytes(void);
static void Cmd_if_not_in_bytes(void);
static void Cmd_if_in_hwords(void);
static void Cmd_if_not_in_hwords(void);
static void Cmd_if_user_has_attacking_move(void);
static void Cmd_if_user_has_no_attacking_moves(void);
static void Cmd_get_turn_count(void);
static void Cmd_get_type(void);
static void Cmd_get_considered_move_power(void);
static void Cmd_get_how_powerful_move_is(void);
static void Cmd_get_last_used_battler_move(void);
static void Cmd_if_equal_(void);
static void Cmd_if_not_equal_(void);
static void Cmd_if_would_go_first(void);
static void Cmd_if_would_not_go_first(void);
static void Cmd_nullsub_2A(void);
static void Cmd_nullsub_2B(void);
static void Cmd_count_alive_pokemon(void);
static void Cmd_get_considered_move(void);
static void Cmd_get_considered_move_effect(void);
static void Cmd_get_ability(void);
static void Cmd_get_highest_type_effectiveness(void);
static void Cmd_if_type_effectiveness(void);
static void Cmd_nullsub_32(void);
static void Cmd_nullsub_33(void);
static void Cmd_if_status_in_party(void);
static void Cmd_if_status_not_in_party(void);
static void Cmd_get_weather(void);
static void Cmd_if_effect(void);
static void Cmd_if_not_effect(void);
static void Cmd_if_stat_level_less_than(void);
static void Cmd_if_stat_level_more_than(void);
static void Cmd_if_stat_level_equal(void);
static void Cmd_if_stat_level_not_equal(void);
static void Cmd_if_can_faint(void);
static void Cmd_if_cant_faint(void);
static void Cmd_if_has_move(void);
static void Cmd_if_doesnt_have_move(void);
static void Cmd_if_has_move_with_effect(void);
static void Cmd_if_doesnt_have_move_with_effect(void);
static void Cmd_if_any_move_disabled_or_encored(void);
static void Cmd_if_curr_move_disabled_or_encored(void);
static void Cmd_flee(void);
static void Cmd_if_random_safari_flee(void);
static void Cmd_watch(void);
static void Cmd_get_hold_effect(void);
static void Cmd_get_gender(void);
static void Cmd_is_first_turn_for(void);
static void Cmd_get_stockpile_count(void);
static void Cmd_is_double_battle(void);
static void Cmd_get_used_held_item(void);
static void Cmd_get_move_type_from_result(void);
static void Cmd_get_move_power_from_result(void);
static void Cmd_get_move_effect_from_result(void);
static void Cmd_get_protect_count(void);
static void Cmd_nullsub_52(void);
static void Cmd_nullsub_53(void);
static void Cmd_nullsub_54(void);
static void Cmd_nullsub_55(void);
static void Cmd_nullsub_56(void);
static void Cmd_nullsub_57(void);
static void Cmd_call(void);
static void Cmd_goto(void);
static void Cmd_end(void);
static void Cmd_if_level_compare(void);
static void Cmd_if_target_taunted(void);
static void Cmd_if_target_not_taunted(void);

static void RecordLastUsedMoveByTarget(void);
static void AISyncBattlerHistory(u8 battler);
static void BattleAI_DoAIProcessing(void);
static void AIStackPushVar(const u8 *ptr);
static bool8 AIStackPop(void);

typedef void (*BattleAICmdFunc)(void);

static const BattleAICmdFunc sBattleAICmdTable[] =
{
    Cmd_if_random_less_than,              // 0x0
    Cmd_if_random_greater_than,           // 0x1
    Cmd_if_random_equal,                  // 0x2
    Cmd_if_random_not_equal,              // 0x3
    Cmd_score,                            // 0x4
    Cmd_if_hp_less_than,                  // 0x5
    Cmd_if_hp_more_than,                  // 0x6
    Cmd_if_hp_equal,                      // 0x7
    Cmd_if_hp_not_equal,                  // 0x8
    Cmd_if_status,                        // 0x9
    Cmd_if_not_status,                    // 0xA
    Cmd_if_status2,                       // 0xB
    Cmd_if_not_status2,                   // 0xC
    Cmd_if_status3,                       // 0xD
    Cmd_if_not_status3,                   // 0xE
    Cmd_if_side_affecting,                // 0xF
    Cmd_if_not_side_affecting,            // 0x10
    Cmd_if_less_than,                     // 0x11
    Cmd_if_more_than,                     // 0x12
    Cmd_if_equal,                         // 0x13
    Cmd_if_not_equal,                     // 0x14
    Cmd_if_less_than_ptr,                 // 0x15
    Cmd_if_more_than_ptr,                 // 0x16
    Cmd_if_equal_ptr,                     // 0x17
    Cmd_if_not_equal_ptr,                 // 0x18
    Cmd_if_move,                          // 0x19
    Cmd_if_not_move,                      // 0x1A
    Cmd_if_in_bytes,                      // 0x1B
    Cmd_if_not_in_bytes,                  // 0x1C
    Cmd_if_in_hwords,                     // 0x1D
    Cmd_if_not_in_hwords,                 // 0x1E
    Cmd_if_user_has_attacking_move,       // 0x1F
    Cmd_if_user_has_no_attacking_moves,   // 0x20
    Cmd_get_turn_count,                   // 0x21
    Cmd_get_type,                         // 0x22
    Cmd_get_considered_move_power,        // 0x23
    Cmd_get_how_powerful_move_is,         // 0x24
    Cmd_get_last_used_battler_move,       // 0x25
    Cmd_if_equal_,                        // 0x26
    Cmd_if_not_equal_,                    // 0x27
    Cmd_if_would_go_first,                // 0x28
    Cmd_if_would_not_go_first,            // 0x29
    Cmd_nullsub_2A,                       // 0x2A
    Cmd_nullsub_2B,                       // 0x2B
    Cmd_count_alive_pokemon,              // 0x2C
    Cmd_get_considered_move,              // 0x2D
    Cmd_get_considered_move_effect,       // 0x2E
    Cmd_get_ability,                      // 0x2F
    Cmd_get_highest_type_effectiveness,   // 0x30
    Cmd_if_type_effectiveness,            // 0x31
    Cmd_nullsub_32,                       // 0x32
    Cmd_nullsub_33,                       // 0x33
    Cmd_if_status_in_party,               // 0x34
    Cmd_if_status_not_in_party,           // 0x35
    Cmd_get_weather,                      // 0x36
    Cmd_if_effect,                        // 0x37
    Cmd_if_not_effect,                    // 0x38
    Cmd_if_stat_level_less_than,          // 0x39
    Cmd_if_stat_level_more_than,          // 0x3A
    Cmd_if_stat_level_equal,              // 0x3B
    Cmd_if_stat_level_not_equal,          // 0x3C
    Cmd_if_can_faint,                     // 0x3D
    Cmd_if_cant_faint,                    // 0x3E
    Cmd_if_has_move,                      // 0x3F
    Cmd_if_doesnt_have_move,              // 0x40
    Cmd_if_has_move_with_effect,          // 0x41
    Cmd_if_doesnt_have_move_with_effect,  // 0x42
    Cmd_if_any_move_disabled_or_encored,  // 0x43
    Cmd_if_curr_move_disabled_or_encored, // 0x44
    Cmd_flee,                             // 0x45
    Cmd_if_random_safari_flee,            // 0x46
    Cmd_watch,                            // 0x47
    Cmd_get_hold_effect,                  // 0x48
    Cmd_get_gender,                       // 0x49
    Cmd_is_first_turn_for,                // 0x4A
    Cmd_get_stockpile_count,              // 0x4B
    Cmd_is_double_battle,                 // 0x4C
    Cmd_get_used_held_item,               // 0x4D
    Cmd_get_move_type_from_result,        // 0x4E
    Cmd_get_move_power_from_result,       // 0x4F
    Cmd_get_move_effect_from_result,      // 0x50
    Cmd_get_protect_count,                // 0x51
    Cmd_nullsub_52,                       // 0x52
    Cmd_nullsub_53,                       // 0x53
    Cmd_nullsub_54,                       // 0x54
    Cmd_nullsub_55,                       // 0x55
    Cmd_nullsub_56,                       // 0x56
    Cmd_nullsub_57,                       // 0x57
    Cmd_call,                             // 0x58
    Cmd_goto,                             // 0x59
    Cmd_end,                              // 0x5A
    Cmd_if_level_compare,                 // 0x5B
    Cmd_if_target_taunted,                // 0x5C
    Cmd_if_target_not_taunted,            // 0x5D
};

static const u16 sDiscouragedPowerfulMoveEffects[] =
{
    EFFECT_EXPLOSION,
    EFFECT_DREAM_EATER,
    EFFECT_RAZOR_WIND,
    EFFECT_SKY_ATTACK,
    EFFECT_RECHARGE,
    EFFECT_SKULL_BASH,
    EFFECT_SOLAR_BEAM,
    EFFECT_SPIT_UP,
    EFFECT_FOCUS_PUNCH,
    EFFECT_SUPERPOWER,
    EFFECT_ERUPTION,
    EFFECT_OVERHEAT,
    0xFFFF
};

void BattleAI_HandleItemUseBeforeAISetup(void)
{
    s32 i;
    u8 *data = (u8 *)BATTLE_HISTORY;

    for (i = 0; i < sizeof(struct BattleHistory); i++)
        data[i] = 0;

    // Items are allowed to use in ONLY trainer battles.
    if ((gBattleTypeFlags & BATTLE_TYPE_TRAINER)
        && (gTrainerBattleOpponent_A != TRAINER_SECRET_BASE)
        && !(gBattleTypeFlags & (BATTLE_TYPE_TRAINER_TOWER | BATTLE_TYPE_EREADER_TRAINER | BATTLE_TYPE_BATTLE_TOWER | BATTLE_TYPE_SAFARI | BATTLE_TYPE_LINK))
        )
    {
        for (i = 0; i < MAX_TRAINER_ITEMS; i++)
        {
            if (gTrainers[gTrainerBattleOpponent_A].items[i] != 0)
            {
                BATTLE_HISTORY->trainerItems[BATTLE_HISTORY->itemsNo] = gTrainers[gTrainerBattleOpponent_A].items[i];
                BATTLE_HISTORY->itemsNo++;
            }
        }
    }

    BattleAI_SetupAIData();
}

void BattleAI_SetupAIData(void)
{
    s32 i;
    u8 *data = (u8 *)AI_THINKING_STRUCT;
    u8 moveLimitations;

    for (i = 0; i < gBattlersCount; i++)
        AISyncBattlerHistory(i);
    // Clear AI data.
    for (i = 0; i < sizeof(struct AI_ThinkingStruct); i++)
        data[i] = 0;

    for (i = 0; i < MAX_MON_MOVES; i++)
        AI_THINKING_STRUCT->score[i] = 100;

    moveLimitations = CheckMoveLimitations(gActiveBattler, 0, 0xFF);

    // Ignore moves that aren't possible to use.
    for (i = 0; i < MAX_MON_MOVES; i++)
    {
        if (gBitTable[i] & moveLimitations)
            AI_THINKING_STRUCT->score[i] = 0;

        AI_THINKING_STRUCT->simulatedRNG[i] = 100 - (Random() % 16);
    }

    gBattleResources->AI_ScriptsStack->size = 0;
    gBattlerAttacker = gActiveBattler;

    // Decide a random target battlerId in doubles.
    if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
    {
        gBattlerTarget = (Random() & BIT_FLANK) + BATTLE_OPPOSITE(GetBattlerSide(gActiveBattler));


        if (gAbsentBattlerFlags & gBitTable[gBattlerTarget])
            gBattlerTarget ^= BIT_FLANK;
    }
    // There's only one choice in single battles.
    else
    {
        gBattlerTarget = BATTLE_OPPOSITE(gBattlerAttacker);
    }

    // Choose proper trainer ai scripts.
    // Fire Red, why all the returns?!?
    if (gBattleTypeFlags & BATTLE_TYPE_SAFARI)
    {
        AI_THINKING_STRUCT->aiFlags = AI_SCRIPT_SAFARI;
        return;
    }
    else if (gBattleTypeFlags & BATTLE_TYPE_ROAMER)
    {
        AI_THINKING_STRUCT->aiFlags = AI_SCRIPT_ROAMING;
        return;
    }
    else if (!(gBattleTypeFlags & (BATTLE_TYPE_TRAINER_TOWER | BATTLE_TYPE_EREADER_TRAINER | BATTLE_TYPE_BATTLE_TOWER)) && (gTrainerBattleOpponent_A != TRAINER_SECRET_BASE))
    {
        if (gBattleTypeFlags & BATTLE_TYPE_WILD_SCRIPTED)
        {
            AI_THINKING_STRUCT->aiFlags = AI_SCRIPT_CHECK_BAD_MOVE;
            return;
        }
        else if (gBattleTypeFlags & BATTLE_TYPE_LEGENDARY_FRLG)
        {
            AI_THINKING_STRUCT->aiFlags = (AI_SCRIPT_CHECK_BAD_MOVE | AI_SCRIPT_TRY_TO_FAINT | AI_SCRIPT_CHECK_VIABILITY);
            return;
        }
    }
    else
    {
        AI_THINKING_STRUCT->aiFlags = (AI_SCRIPT_CHECK_BAD_MOVE | AI_SCRIPT_TRY_TO_FAINT | AI_SCRIPT_CHECK_VIABILITY);
        return;
    }
    AI_THINKING_STRUCT->aiFlags = gTrainers[gTrainerBattleOpponent_A].aiFlags;
}

// Avoid AbilityBattleEffects here: evaluation must not trigger ability messages.
static bool8 AIWeatherHasEffect(void)
{
    u8 i;
    for (i = 0; i < gBattlersCount; i++)
        if (!(gAbsentBattlerFlags & gBitTable[i]) && gBattleMons[i].hp
         && (gBattleMons[i].ability == ABILITY_CLOUD_NINE || gBattleMons[i].ability == ABILITY_AIR_LOCK))
            return FALSE;
    return TRUE;
}

static bool8 AIInflictsStatus(u16 move)
{
    switch (gBattleMoves[move].effect)
    {
    case EFFECT_SLEEP:
    case EFFECT_POISON:
    case EFFECT_TOXIC:
    case EFFECT_PARALYZE:
    case EFFECT_WILL_O_WISP:
    case EFFECT_YAWN:
        return TRUE;
    }
    return FALSE;
}

static void ApplyDoublesScores(void)
{
    s32 i, j;
    u8 partner = BATTLE_PARTNER(gBattlerAttacker);
    struct DoublesAIContext context = {0};
    enum DoublesMoveKind kind;
    u16 move, partnerMove;
    u8 partnerLimitations;

    if (!(gBattleTypeFlags & BATTLE_TYPE_DOUBLE))
        return;

    context.partnerAlive = !(gAbsentBattlerFlags & gBitTable[partner]) && gBattleMons[partner].hp != 0;
    if (context.partnerAlive && !(gBattleMons[partner].status1 & (STATUS1_SLEEP | STATUS1_FREEZE))
     && !(gBattleMons[partner].ability == ABILITY_TRUANT && gDisableStructs[partner].truantCounter))
    {
        partnerLimitations = CheckMoveLimitations(partner, 0, 0xFF);
        for (j = 0; j < MAX_MON_MOVES; j++)
            if (gBattleMoves[gBattleMons[partner].moves[j]].power != 0
             && !(partnerLimitations & gBitTable[j]))
                context.partnerCanAttack = TRUE;
        // Own-side plans are available; never inspect the player's queued action.
        partnerMove = gChosenMoveByBattler[partner];
        if ((gChosenActionByBattler[partner] != B_ACTION_NONE
          && gChosenActionByBattler[partner] != B_ACTION_USE_MOVE)
         || (partnerMove != MOVE_NONE && gBattleMoves[partnerMove].power == 0))
            context.partnerCanAttack = FALSE;
    }
    context.twoOpponents = !(gAbsentBattlerFlags & gBitTable[BATTLE_PARTNER(gBattlerTarget)])
                       && gBattleMons[BATTLE_PARTNER(gBattlerTarget)].hp != 0;

    for (i = 0; i < MAX_MON_MOVES; i++)
    {
        move = gBattleMons[gBattlerAttacker].moves[i];
        kind = DOUBLES_MOVE_OTHER;
        context.effectUseful = FALSE;
        context.effectAlreadyActive = FALSE;
        context.partnerImmune = FALSE;
        switch (move)
        {
        case MOVE_HELPING_HAND:
            kind = DOUBLES_MOVE_HELPING_HAND;
            break;
        case MOVE_REFLECT:
        case MOVE_LIGHT_SCREEN:
            kind = DOUBLES_MOVE_SCREEN;
            context.effectAlreadyActive = (gSideStatuses[GetBattlerSide(gBattlerAttacker)]
                & (move == MOVE_REFLECT ? SIDE_STATUS_REFLECT : SIDE_STATUS_LIGHTSCREEN)) != 0;
            break;
        case MOVE_STRING_SHOT:
            kind = DOUBLES_MOVE_SPEED_CONTROL;
            context.effectUseful = gBattleMons[gBattlerTarget].statStages[STAT_SPEED] > MIN_STAT_STAGE;
            break;
        case MOVE_ICY_WIND:
        case MOVE_ROCK_TOMB:
            kind = DOUBLES_MOVE_DAMAGING_SPEED_CONTROL;
            context.effectUseful = gBattleMons[gBattlerTarget].statStages[STAT_SPEED] > MIN_STAT_STAGE;
            break;
        case MOVE_PROTECT:
        case MOVE_DETECT:
            kind = DOUBLES_MOVE_PROTECT;
            context.effectAlreadyActive = gDisableStructs[gBattlerAttacker].protectUses != 0;
            context.effectUseful = (gBattleMons[gBattlerTarget].status1 & (STATUS1_POISON | STATUS1_TOXIC_POISON | STATUS1_BURN))
                || (gStatuses3[gBattlerTarget] & STATUS3_LEECHSEED)
                || (context.partnerCanAttack && gBattleMons[gBattlerAttacker].hp * 2 < gBattleMons[gBattlerAttacker].maxHP);
            break;
        case MOVE_RAIN_DANCE:
            kind = DOUBLES_MOVE_WEATHER;
            context.effectAlreadyActive = (gBattleWeather & B_WEATHER_RAIN) != 0;
            context.effectUseful = IS_BATTLER_OF_TYPE(gBattlerAttacker, TYPE_WATER)
                || (context.partnerAlive && IS_BATTLER_OF_TYPE(partner, TYPE_WATER));
            break;
        case MOVE_SUNNY_DAY:
            kind = DOUBLES_MOVE_WEATHER;
            context.effectAlreadyActive = (gBattleWeather & B_WEATHER_SUN) != 0;
            context.effectUseful = IS_BATTLER_OF_TYPE(gBattlerAttacker, TYPE_FIRE)
                || gBattleMons[gBattlerAttacker].ability == ABILITY_CHLOROPHYLL
                || (context.partnerAlive && (IS_BATTLER_OF_TYPE(partner, TYPE_FIRE)
                    || gBattleMons[partner].ability == ABILITY_CHLOROPHYLL));
            break;
        case MOVE_SANDSTORM:
            kind = DOUBLES_MOVE_WEATHER;
            context.effectAlreadyActive = (gBattleWeather & B_WEATHER_SANDSTORM) != 0;
            context.effectUseful = IS_BATTLER_OF_TYPE(gBattlerAttacker, TYPE_ROCK)
                || IS_BATTLER_OF_TYPE(gBattlerAttacker, TYPE_GROUND)
                || IS_BATTLER_OF_TYPE(gBattlerAttacker, TYPE_STEEL);
            break;
        case MOVE_HAIL:
            kind = DOUBLES_MOVE_WEATHER;
            context.effectAlreadyActive = (gBattleWeather & B_WEATHER_HAIL) != 0;
            context.effectUseful = IS_BATTLER_OF_TYPE(gBattlerAttacker, TYPE_ICE);
            break;
        default:
            if (gBattleMoves[move].target == MOVE_TARGET_FOES_AND_ALLY)
            {
                kind = DOUBLES_MOVE_FRIENDLY_FIRE;
                context.partnerImmune = gBattleMoves[move].type == TYPE_GROUND
                    && (IS_BATTLER_OF_TYPE(partner, TYPE_FLYING) || gBattleMons[partner].ability == ABILITY_LEVITATE);
                // Protect/Detect are priority +4; ordinary spread attacks follow.
                if (context.partnerAlive && gChosenActionByBattler[partner] == B_ACTION_USE_MOVE
                 && (gChosenMoveByBattler[partner] == MOVE_PROTECT || gChosenMoveByBattler[partner] == MOVE_DETECT)
                 && !(gBattleMons[partner].status1 & (STATUS1_SLEEP | STATUS1_FREEZE))
                 && !(gBattleMons[partner].ability == ABILITY_TRUANT && gDisableStructs[partner].truantCounter)
                 && gDisableStructs[partner].protectUses == 0)
                    context.partnerImmune = TRUE;
            }
            break;
        }
        if (context.partnerAlive && gChosenMoveByBattler[partner] == move
         && (kind == DOUBLES_MOVE_SCREEN || kind == DOUBLES_MOVE_WEATHER))
            context.effectAlreadyActive = TRUE;
        if (kind == DOUBLES_MOVE_WEATHER && !AIWeatherHasEffect())
            context.effectUseful = FALSE;
        AI_THINKING_STRUCT->score[i] = DoublesAI_AdjustScore(AI_THINKING_STRUCT->score[i], kind, &context);
        // Two major statuses cannot coexist. Spread the own side's status
        // attempts across targets instead of spending both actions on one.
        if (context.partnerAlive && AIInflictsStatus(move)
         && gChosenActionByBattler[partner] == B_ACTION_USE_MOVE
         && AIInflictsStatus(gChosenMoveByBattler[partner])
         && gBattleStruct->moveTarget[partner] == gBattlerTarget)
        {
            s32 score = AI_THINKING_STRUCT->score[i];
            AI_THINKING_STRUCT->score[i] = score > 12 ? score - 12 : 0;
        }
        if (gBattleMoves[move].power != 0 && gBattleMoves[move].target == MOVE_TARGET_BOTH)
            AI_THINKING_STRUCT->score[i] = DoublesAI_AdjustScore(AI_THINKING_STRUCT->score[i], DOUBLES_MOVE_SPREAD, &context);
        if (context.partnerAlive && gChosenMoveByBattler[partner] == MOVE_HELPING_HAND)
        {
            context.effectUseful = gBattleMoves[move].power != 0;
            AI_THINKING_STRUCT->score[i] = DoublesAI_AdjustScore(AI_THINKING_STRUCT->score[i], DOUBLES_MOVE_RECEIVE_HELPING_HAND, &context);
        }
    }
}

// Accuracy-weighted fraction of current HP, capped at a knockout. Use only
// visible combatants and our own moves; never read the player's chosen action.
static s32 AIDamageValue(u16 move, u8 target)
{
    s32 damage, value, savedDamage = gBattleMoveDamage;
    u16 savedMove = gCurrentMove, savedPower = gDynamicBasePower, savedMovePower = gBattleMovePower;
    u8 savedType = gBattleStruct->dynamicMoveType, savedCrit = gCritMultiplier;
    u8 savedMultiplier = gBattleScripting.dmgMultiplier, savedFlags = gMoveResultFlags;
    u8 flags, accuracy = gBattleMoves[move].accuracy;

    if (!gBattleMoves[move].power || !gBattleMons[target].hp)
        return 0;
    gCurrentMove = move;
    gDynamicBasePower = 0;
    gBattleStruct->dynamicMoveType = 0;
    gCritMultiplier = 1;
    gBattleScripting.dmgMultiplier = 1;
    gMoveResultFlags = 0;
    if (gBattleMoves[move].effect == EFFECT_RETURN)
        gDynamicBasePower = 10 * gBattleMons[gBattlerAttacker].friendship / 25;
    else if (gBattleMoves[move].effect == EFFECT_FRUSTRATION)
        gDynamicBasePower = 10 * (255 - gBattleMons[gBattlerAttacker].friendship) / 25;
    else if (gBattleMoves[move].effect == EFFECT_ERUPTION)
    {
        gDynamicBasePower = gBattleMons[gBattlerAttacker].hp * gBattleMoves[move].power / gBattleMons[gBattlerAttacker].maxHP;
        if (!gDynamicBasePower) gDynamicBasePower = 1;
    }
    AI_CalcDmg(gBattlerAttacker, target);
    flags = TypeCalc(move, gBattlerAttacker, target);
    damage = gBattleMoveDamage;
    switch (gBattleMoves[move].effect)
    {
    case EFFECT_LEVEL_DAMAGE: damage = gBattleMons[gBattlerAttacker].level; break;
    case EFFECT_DRAGON_RAGE: damage = 40; break;
    case EFFECT_SONICBOOM: damage = 20; break;
    case EFFECT_SUPER_FANG: damage = gBattleMons[target].hp / 2; break;
    // These require a future hit or random power, not the power-1 placeholder.
    case EFFECT_COUNTER:
    case EFFECT_MIRROR_COAT: damage = 0; break;
    }
    if ((flags & MOVE_RESULT_NO_EFFECT)
     || (gBattleMoves[move].type == TYPE_WATER && gBattleMons[target].ability == ABILITY_WATER_ABSORB)
     || (gBattleMoves[move].type == TYPE_ELECTRIC && gBattleMons[target].ability == ABILITY_VOLT_ABSORB)
     || (gBattleMoves[move].type == TYPE_FIRE && gBattleMons[target].ability == ABILITY_FLASH_FIRE))
        damage = 0;
    if (move == MOVE_THUNDER && AIWeatherHasEffect())
    {
        if (gBattleWeather & B_WEATHER_RAIN) accuracy = 100;
        else if (gBattleWeather & B_WEATHER_SUN) accuracy = 50;
    }
    value = damage * 1000 / gBattleMons[target].hp;
    if (value > 1000) value = 1000;
    if (accuracy) value = value * accuracy / 100;
    gCurrentMove = savedMove;
    gDynamicBasePower = savedPower;
    gBattleStruct->dynamicMoveType = savedType;
    gCritMultiplier = savedCrit;
    gBattleScripting.dmgMultiplier = savedMultiplier;
    gMoveResultFlags = savedFlags;
    gBattleMoveDamage = savedDamage;
    gBattleMovePower = savedMovePower;
    return value;
}

static s32 AIMoveDamageValue(u16 move)
{
    s32 value = AIDamageValue(move, gBattlerTarget);
    u8 other = BATTLE_PARTNER(gBattlerTarget);
    if ((gBattleMoves[move].target == MOVE_TARGET_BOTH || gBattleMoves[move].target == MOVE_TARGET_FOES_AND_ALLY)
     && !(gAbsentBattlerFlags & gBitTable[other]) && gBattleMons[other].hp)
        value += AIDamageValue(move, other);
    return value;
}

static u8 ChooseMoveForTarget(void)
{
    u8 bestMove = 0, bestScore = 0, ties = 0;
    u8 limitations;
    s32 i;
    s32 damage, bestDamage = -1;

    RecordLastUsedMoveByTarget();
    while (AI_THINKING_STRUCT->aiFlags != 0)
    {
        if (AI_THINKING_STRUCT->aiFlags & 1)
        {
            AI_THINKING_STRUCT->aiState = AIState_SettingUp;
            BattleAI_DoAIProcessing();
        }
        AI_THINKING_STRUCT->aiFlags >>= 1;
        AI_THINKING_STRUCT->aiLogicId++;
        AI_THINKING_STRUCT->movesetIndex = 0;
    }

    // special flee or watch cases for safari.
    if (AI_THINKING_STRUCT->aiAction & AI_ACTION_FLEE)
        return AI_CHOICE_FLEE;
    if (AI_THINKING_STRUCT->aiAction & AI_ACTION_WATCH)
        return AI_CHOICE_WATCH;

    ApplyDoublesScores();

    limitations = CheckMoveLimitations(gBattlerAttacker, 0, 0xFF);
    for (i = 0; i < MAX_MON_MOVES; i++)
    {
        if (limitations & gBitTable[i])
            continue;
        damage = (gBattleTypeFlags & BATTLE_TYPE_DOUBLE) ? AIMoveDamageValue(gBattleMons[gBattlerAttacker].moves[i]) : 0;
        if (ties == 0 || bestScore < AI_THINKING_STRUCT->score[i]
         || (bestScore == AI_THINKING_STRUCT->score[i] && damage > bestDamage))
        {
            bestScore = AI_THINKING_STRUCT->score[i];
            bestMove = i;
            bestDamage = damage;
            ties = 1;
        }
        else if (bestScore == AI_THINKING_STRUCT->score[i] && damage == bestDamage && Random() % ++ties == 0)
            bestMove = i;
    }
    return bestMove; // All moves blocked is handled as Struggle by the controller.
}

u8 BattleAI_ChooseMoveOrAction(void)
{
    u8 target, move, bestMove = 0, bestTarget = gBattlerTarget;
    s32 score, bestScore = -1, damage, bestDamage = -1;
    u8 ties = 0;

    if ((gBattleTypeFlags & (BATTLE_TYPE_TRAINER | BATTLE_TYPE_DOUBLE)) != (BATTLE_TYPE_TRAINER | BATTLE_TYPE_DOUBLE))
        return ChooseMoveForTarget();

    // Compare both visible opponents using the same existing move-evaluation scripts.
    // Never inspect the player's queued action or reserve team. Native damage
    // evaluation retains the original engine's knowledge/ability assumptions.
    for (target = 0; target < gBattlersCount; target++)
    {
        if (GetBattlerSide(target) == GetBattlerSide(gActiveBattler)
         || (gAbsentBattlerFlags & gBitTable[target]) || gBattleMons[target].hp == 0)
            continue;
        BattleAI_SetupAIData();
        gBattlerTarget = target;
        move = ChooseMoveForTarget();
        if (move >= MAX_MON_MOVES)
            return move;
        score = AI_THINKING_STRUCT->score[move];
        damage = AIMoveDamageValue(gBattleMons[gBattlerAttacker].moves[move]);
        // Native script scores often tie across targets even for a resisted hit.
        // Prefer effective damage within that tie; do not override support scores.
        if (score > bestScore || (score == bestScore && damage > bestDamage))
        {
            bestScore = score;
            bestDamage = damage;
            bestMove = move;
            bestTarget = target;
            ties = 1;
        }
        else if (score == bestScore && damage == bestDamage && Random() % ++ties == 0)
        {
            bestMove = move;
            bestTarget = target;
        }
    }
    gBattlerTarget = bestTarget;
    return bestMove;
}

static void BattleAI_DoAIProcessing(void)
{
    while (AI_THINKING_STRUCT->aiState != AIState_FinishedProcessing)
    {
        switch (AI_THINKING_STRUCT->aiState)
        {
        case AIState_DoNotProcess: // Needed to match.
            break;
        case AIState_SettingUp:
            sAIScriptPtr = gBattleAI_ScriptsTable[AI_THINKING_STRUCT->aiLogicId];

            if (gBattleMons[gBattlerAttacker].pp[AI_THINKING_STRUCT->movesetIndex] == 0)
            {
                AI_THINKING_STRUCT->moveConsidered = 0; // Don't consider moves with no PP
            }
            else
            {
                AI_THINKING_STRUCT->moveConsidered = gBattleMons[gBattlerAttacker].moves[AI_THINKING_STRUCT->movesetIndex];
            }
            AI_THINKING_STRUCT->aiState++;
            break;
        case AIState_Processing:
            if (AI_THINKING_STRUCT->moveConsidered != 0)
            {
                sBattleAICmdTable[*sAIScriptPtr](); // Run AI command.
            }
            else
            {
                AI_THINKING_STRUCT->score[AI_THINKING_STRUCT->movesetIndex] = 0;
                AI_THINKING_STRUCT->aiAction |= AI_ACTION_DONE;
            }
            if (AI_THINKING_STRUCT->aiAction & AI_ACTION_DONE)
            {
                AI_THINKING_STRUCT->movesetIndex++;

                if (AI_THINKING_STRUCT->movesetIndex < MAX_MON_MOVES && (AI_THINKING_STRUCT->aiAction & AI_ACTION_DO_NOT_ATTACK) == 0)
                    AI_THINKING_STRUCT->aiState = AIState_SettingUp; // as long as their are more moves to process, keep setting this to setup state.
                else
                    AI_THINKING_STRUCT->aiState++; // done processing.
                AI_THINKING_STRUCT->aiAction &= (AI_ACTION_FLEE | AI_ACTION_WATCH | AI_ACTION_DO_NOT_ATTACK |
                AI_ACTION_UNK5 | AI_ACTION_UNK6 | AI_ACTION_UNK7 | AI_ACTION_UNK8); // disable AI_ACTION_DONE.
            }
            break;
        }
    }
}

static void AISyncBattlerHistory(u8 battler)
{
    u8 slot = gBattlerPartyIndexes[battler] + 1;
    if (BATTLE_HISTORY->partyIndexes[battler] != slot)
    {
        memset(&BATTLE_HISTORY->usedMoves[battler], 0, sizeof(struct UsedMoves));
        BATTLE_HISTORY->abilities[battler] = 0;
        BATTLE_HISTORY->itemEffects[battler] = 0;
        BATTLE_HISTORY->partyIndexes[battler] = slot;
    }
}

static void RecordLastUsedMoveByTarget(void)
{
    s32 i;

    AISyncBattlerHistory(gBattlerTarget);
    if (gLastMoves[gBattlerTarget] == MOVE_NONE || gLastMoves[gBattlerTarget] >= MOVES_COUNT)
        return;
    for (i = 0; i < MAX_MON_MOVES; i++)
    {
        if (BATTLE_HISTORY->usedMoves[gBattlerTarget].moves[i] == gLastMoves[gBattlerTarget])
            break;

        if (BATTLE_HISTORY->usedMoves[gBattlerTarget].moves[i] == MOVE_NONE)
        {
            BATTLE_HISTORY->usedMoves[gBattlerTarget].moves[i] = gLastMoves[gBattlerTarget];
            break;
        }
    }

}

// not used
static void ClearBattlerMoveHistory(u8 battlerId)
{
    s32 i;

    for (i = 0; i < MAX_MON_MOVES; i++)
        BATTLE_HISTORY->usedMoves[battlerId].moves[i] = MOVE_NONE;

}

void RecordAbilityBattle(u8 battlerId, u8 abilityId)
{
    AISyncBattlerHistory(battlerId);
    BATTLE_HISTORY->abilities[battlerId] = abilityId;

}

void RecordItemEffectBattle(u8 battlerId, u8 itemEffect)
{
    AISyncBattlerHistory(battlerId);
    BATTLE_HISTORY->itemEffects[battlerId] = itemEffect;

}

static void Cmd_if_random_less_than(void)
{
    if (Random() % 256 < sAIScriptPtr[1])
        sAIScriptPtr = T1_READ_PTR(sAIScriptPtr + 2);
    else
        sAIScriptPtr += 6;
}

static void Cmd_if_random_greater_than(void)
{
    if (Random() % 256 > sAIScriptPtr[1])
        sAIScriptPtr = T1_READ_PTR(sAIScriptPtr + 2);
    else
        sAIScriptPtr += 6;
}

static void Cmd_if_random_equal(void)
{
    if (Random() % 256 == sAIScriptPtr[1])
        sAIScriptPtr = T1_READ_PTR(sAIScriptPtr + 2);
    else
        sAIScriptPtr += 6;
}

static void Cmd_if_random_not_equal(void)
{
    if (Random() % 256 != sAIScriptPtr[1])
        sAIScriptPtr = T1_READ_PTR(sAIScriptPtr + 2);
    else
        sAIScriptPtr += 6;
}

static void Cmd_score(void)
{
    AI_THINKING_STRUCT->score[AI_THINKING_STRUCT->movesetIndex] += sAIScriptPtr[1]; // add the result to the array of the move consider's score.

    if (AI_THINKING_STRUCT->score[AI_THINKING_STRUCT->movesetIndex] < 0) // if the score is negative, flatten it to 0.
        AI_THINKING_STRUCT->score[AI_THINKING_STRUCT->movesetIndex] = 0;

    sAIScriptPtr += 2; // AI return.
}

static void Cmd_if_hp_less_than(void)
{
    u16 battlerId;

    if (sAIScriptPtr[1] == AI_USER)
        battlerId = gBattlerAttacker;
    else
        battlerId = gBattlerTarget;

    if ((u32)(100 * gBattleMons[battlerId].hp / gBattleMons[battlerId].maxHP) < sAIScriptPtr[2])
        sAIScriptPtr = T1_READ_PTR(sAIScriptPtr + 3);
    else
        sAIScriptPtr += 7;
}

static void Cmd_if_hp_more_than(void)
{
    u16 battlerId;

    if (sAIScriptPtr[1] == AI_USER)
        battlerId = gBattlerAttacker;
    else
        battlerId = gBattlerTarget;

    if ((u32)(100 * gBattleMons[battlerId].hp / gBattleMons[battlerId].maxHP) > sAIScriptPtr[2])
        sAIScriptPtr = T1_READ_PTR(sAIScriptPtr + 3);
    else
        sAIScriptPtr += 7;
}

static void Cmd_if_hp_equal(void)
{
    u16 battlerId;

    if (sAIScriptPtr[1] == AI_USER)
        battlerId = gBattlerAttacker;
    else
        battlerId = gBattlerTarget;

    if ((u32)(100 * gBattleMons[battlerId].hp / gBattleMons[battlerId].maxHP) == sAIScriptPtr[2])
        sAIScriptPtr = T1_READ_PTR(sAIScriptPtr + 3);
    else
        sAIScriptPtr += 7;
}

static void Cmd_if_hp_not_equal(void)
{
    u16 battlerId;

    if (sAIScriptPtr[1] == AI_USER)
        battlerId = gBattlerAttacker;
    else
        battlerId = gBattlerTarget;

    if ((u32)(100 * gBattleMons[battlerId].hp / gBattleMons[battlerId].maxHP) != sAIScriptPtr[2])
        sAIScriptPtr = T1_READ_PTR(sAIScriptPtr + 3);
    else
        sAIScriptPtr += 7;
}

static void Cmd_if_status(void)
{
    u16 battlerId;
    u32 status;

    if (sAIScriptPtr[1] == AI_USER)
        battlerId = gBattlerAttacker;
    else
        battlerId = gBattlerTarget;

    status = T1_READ_32(sAIScriptPtr + 2);

    if (gBattleMons[battlerId].status1 & status)
        sAIScriptPtr = T1_READ_PTR(sAIScriptPtr + 6);
    else
        sAIScriptPtr += 10;
}

static void Cmd_if_not_status(void)
{
    u16 battlerId;
    u32 status;

    if (sAIScriptPtr[1] == AI_USER)
        battlerId = gBattlerAttacker;
    else
        battlerId = gBattlerTarget;

    status = T1_READ_32(sAIScriptPtr + 2);

    if (!(gBattleMons[battlerId].status1 & status))
        sAIScriptPtr = T1_READ_PTR(sAIScriptPtr + 6);
    else
        sAIScriptPtr += 10;
}

static void Cmd_if_status2(void)
{
    u16 battlerId;
    u32 status;

    if (sAIScriptPtr[1] == AI_USER)
        battlerId = gBattlerAttacker;
    else
        battlerId = gBattlerTarget;

    status = T1_READ_32(sAIScriptPtr + 2);

    if ((gBattleMons[battlerId].status2 & status))
        sAIScriptPtr = T1_READ_PTR(sAIScriptPtr + 6);
    else
        sAIScriptPtr += 10;
}

static void Cmd_if_not_status2(void)
{
    u16 battlerId;
    u32 status;

    if (sAIScriptPtr[1] == AI_USER)
        battlerId = gBattlerAttacker;
    else
        battlerId = gBattlerTarget;

    status = T1_READ_32(sAIScriptPtr + 2);

    if (!(gBattleMons[battlerId].status2 & status))
        sAIScriptPtr = T1_READ_PTR(sAIScriptPtr + 6);
    else
        sAIScriptPtr += 10;
}

static void Cmd_if_status3(void)
{
    u16 battlerId;
    u32 status;

    if (sAIScriptPtr[1] == AI_USER)
        battlerId = gBattlerAttacker;
    else
        battlerId = gBattlerTarget;

    status = T1_READ_32(sAIScriptPtr + 2);

    if (gStatuses3[battlerId] & status)
        sAIScriptPtr = T1_READ_PTR(sAIScriptPtr + 6);
    else
        sAIScriptPtr += 10;
}

static void Cmd_if_not_status3(void)
{
    u16 battlerId;
    u32 status;

    if (sAIScriptPtr[1] == AI_USER)
        battlerId = gBattlerAttacker;
    else
        battlerId = gBattlerTarget;

    status = T1_READ_32(sAIScriptPtr + 2);

    if (!(gStatuses3[battlerId] & status))
        sAIScriptPtr = T1_READ_PTR(sAIScriptPtr + 6);
    else
        sAIScriptPtr += 10;
}

static void Cmd_if_side_affecting(void)
{
    u16 battlerId;
    u32 side, status;

    if (sAIScriptPtr[1] == AI_USER)
        battlerId = gBattlerAttacker;
    else
        battlerId = gBattlerTarget;

    side = GET_BATTLER_SIDE(battlerId);
    status = T1_READ_32(sAIScriptPtr + 2);

    if (gSideStatuses[side] & status)
        sAIScriptPtr = T1_READ_PTR(sAIScriptPtr + 6);
    else
        sAIScriptPtr += 10;
}

static void Cmd_if_not_side_affecting(void)
{
    u16 battlerId;
    u32 side, status;

    if (sAIScriptPtr[1] == AI_USER)
        battlerId = gBattlerAttacker;
    else
        battlerId = gBattlerTarget;

    side = GET_BATTLER_SIDE(battlerId);
    status = T1_READ_32(sAIScriptPtr + 2);

    if (!(gSideStatuses[side] & status))
        sAIScriptPtr = T1_READ_PTR(sAIScriptPtr + 6);
    else
        sAIScriptPtr += 10;
}

static void Cmd_if_less_than(void)
{
    if (AI_THINKING_STRUCT->funcResult < sAIScriptPtr[1])
        sAIScriptPtr = T1_READ_PTR(sAIScriptPtr + 2);
    else
        sAIScriptPtr += 6;
}

static void Cmd_if_more_than(void)
{
    if (AI_THINKING_STRUCT->funcResult > sAIScriptPtr[1])
        sAIScriptPtr = T1_READ_PTR(sAIScriptPtr + 2);
    else
        sAIScriptPtr += 6;
}

static void Cmd_if_equal(void)
{
    if (AI_THINKING_STRUCT->funcResult == sAIScriptPtr[1])
        sAIScriptPtr = T1_READ_PTR(sAIScriptPtr + 2);
    else
        sAIScriptPtr += 6;
}

static void Cmd_if_not_equal(void)
{
    if (AI_THINKING_STRUCT->funcResult != sAIScriptPtr[1])
        sAIScriptPtr = T1_READ_PTR(sAIScriptPtr + 2);
    else
        sAIScriptPtr += 6;
}

static void Cmd_if_less_than_ptr(void)
{
    const u8 *value = T1_READ_PTR(sAIScriptPtr + 1);

    if (AI_THINKING_STRUCT->funcResult < *value)
        sAIScriptPtr = T1_READ_PTR(sAIScriptPtr + 5);
    else
        sAIScriptPtr += 9;
}

static void Cmd_if_more_than_ptr(void)
{
    const u8 *value = T1_READ_PTR(sAIScriptPtr + 1);

    if (AI_THINKING_STRUCT->funcResult > *value)
        sAIScriptPtr = T1_READ_PTR(sAIScriptPtr + 5);
    else
        sAIScriptPtr += 9;
}

static void Cmd_if_equal_ptr(void)
{
    const u8 *value = T1_READ_PTR(sAIScriptPtr + 1);

    if (AI_THINKING_STRUCT->funcResult == *value)
        sAIScriptPtr = T1_READ_PTR(sAIScriptPtr + 5);
    else
        sAIScriptPtr += 9;
}

static void Cmd_if_not_equal_ptr(void)
{
    const u8 *value = T1_READ_PTR(sAIScriptPtr + 1);

    if (AI_THINKING_STRUCT->funcResult != *value)
        sAIScriptPtr = T1_READ_PTR(sAIScriptPtr + 5);
    else
        sAIScriptPtr += 9;
}

static void Cmd_if_move(void)
{
    u16 move = T1_READ_16(sAIScriptPtr + 1);

    if (AI_THINKING_STRUCT->moveConsidered == move)
        sAIScriptPtr = T1_READ_PTR(sAIScriptPtr + 3);
    else
        sAIScriptPtr += 7;
}

static void Cmd_if_not_move(void)
{
    u16 move = T1_READ_16(sAIScriptPtr + 1);

    if (AI_THINKING_STRUCT->moveConsidered != move)
        sAIScriptPtr = T1_READ_PTR(sAIScriptPtr + 3);
    else
        sAIScriptPtr += 7;
}

static void Cmd_if_in_bytes(void)
{
    const u8 *ptr = T1_READ_PTR(sAIScriptPtr + 1);

    while (*ptr != 0xFF)
    {
        if (AI_THINKING_STRUCT->funcResult == *ptr)
        {
            sAIScriptPtr = T1_READ_PTR(sAIScriptPtr + 5);
            return;
        }
        ptr++;
    }
    sAIScriptPtr += 9;
}

static void Cmd_if_not_in_bytes(void)
{
    const u8 *ptr = T1_READ_PTR(sAIScriptPtr + 1);

    while (*ptr != 0xFF)
    {
        if (AI_THINKING_STRUCT->funcResult == *ptr)
        {
            sAIScriptPtr += 9;
            return;
        }
        ptr++;
    }
    sAIScriptPtr = T1_READ_PTR(sAIScriptPtr + 5);
}

static void Cmd_if_in_hwords(void)
{
    const u16 *ptr = (const u16 *)T1_READ_PTR(sAIScriptPtr + 1);

    while (*ptr != 0xFFFF)
    {
        if (AI_THINKING_STRUCT->funcResult == *ptr)
        {
            sAIScriptPtr = T1_READ_PTR(sAIScriptPtr + 5);
            return;
        }
        ptr++;
    }
    sAIScriptPtr += 9;
}

static void Cmd_if_not_in_hwords(void)
{
    const u16 *ptr = (const u16 *)T1_READ_PTR(sAIScriptPtr + 1);

    while (*ptr != 0xFFFF)
    {
        if (AI_THINKING_STRUCT->funcResult == *ptr)
        {
            sAIScriptPtr += 9;
            return;
        }
        ptr++;
    }
    sAIScriptPtr = T1_READ_PTR(sAIScriptPtr + 5);
}

static void Cmd_if_user_has_attacking_move(void)
{
    s32 i;

    for (i = 0; i < MAX_MON_MOVES; i++)
    {
        if (gBattleMons[gBattlerAttacker].moves[i] != 0
            && gBattleMoves[gBattleMons[gBattlerAttacker].moves[i]].power != 0)
            break;
    }

    if (i == MAX_MON_MOVES)
        sAIScriptPtr += 5;
    else
        sAIScriptPtr = T1_READ_PTR(sAIScriptPtr + 1);
}

static void Cmd_if_user_has_no_attacking_moves(void)
{
    s32 i;

    for (i = 0; i < MAX_MON_MOVES; i++)
    {
        if (gBattleMons[gBattlerAttacker].moves[i] != 0
         && gBattleMoves[gBattleMons[gBattlerAttacker].moves[i]].power != 0)
            break;
    }

    if (i != MAX_MON_MOVES)
        sAIScriptPtr += 5;
    else
        sAIScriptPtr = T1_READ_PTR(sAIScriptPtr + 1);
}

static void Cmd_get_turn_count(void)
{
    AI_THINKING_STRUCT->funcResult = gBattleResults.battleTurnCounter;
    sAIScriptPtr += 1;
}

static void Cmd_get_type(void)
{
    switch (sAIScriptPtr[1])
    {
    case AI_TYPE1_USER:
        AI_THINKING_STRUCT->funcResult = gBattleMons[gBattlerAttacker].type1;
        break;
    case AI_TYPE1_TARGET:
        AI_THINKING_STRUCT->funcResult = gBattleMons[gBattlerTarget].type1;
        break;
    case AI_TYPE2_USER:
        AI_THINKING_STRUCT->funcResult = gBattleMons[gBattlerAttacker].type2;
        break;
    case AI_TYPE2_TARGET:
        AI_THINKING_STRUCT->funcResult = gBattleMons[gBattlerTarget].type2;
        break;
    case AI_TYPE_MOVE:
        AI_THINKING_STRUCT->funcResult = gBattleMoves[AI_THINKING_STRUCT->moveConsidered].type;
        break;
    }
    sAIScriptPtr += 2;
}

static void Cmd_get_considered_move_power(void)
{
    AI_THINKING_STRUCT->funcResult = gBattleMoves[AI_THINKING_STRUCT->moveConsidered].power;
    sAIScriptPtr += 1;
}

static void Cmd_get_how_powerful_move_is(void)
{
    s32 i, checkedMove;
    s32 moveDmgs[MAX_MON_MOVES];

    for (i = 0; sDiscouragedPowerfulMoveEffects[i] != 0xFFFF; i++)
    {
        if (gBattleMoves[AI_THINKING_STRUCT->moveConsidered].effect == sDiscouragedPowerfulMoveEffects[i])
            break;
    }

    if (gBattleMoves[AI_THINKING_STRUCT->moveConsidered].power > 1
        && sDiscouragedPowerfulMoveEffects[i] == 0xFFFF)
    {
        gDynamicBasePower = 0;
        gBattleStruct->dynamicMoveType = 0;
        gBattleScripting.dmgMultiplier = 1;
        gMoveResultFlags = 0;
        gCritMultiplier = 1;

        for (checkedMove = 0; checkedMove < MAX_MON_MOVES; checkedMove++)
        {
            for (i = 0; sDiscouragedPowerfulMoveEffects[i] != 0xFFFF; i++)
            {
                if (gBattleMoves[gBattleMons[gBattlerAttacker].moves[checkedMove]].effect == sDiscouragedPowerfulMoveEffects[i])
                    break;
            }

            if (gBattleMons[gBattlerAttacker].moves[checkedMove] != MOVE_NONE
                && sDiscouragedPowerfulMoveEffects[i] == 0xFFFF
                && gBattleMoves[gBattleMons[gBattlerAttacker].moves[checkedMove]].power > 1)
            {
                gCurrentMove = gBattleMons[gBattlerAttacker].moves[checkedMove];
                AI_CalcDmg(gBattlerAttacker, gBattlerTarget);
                gMoveResultFlags = TypeCalc(gCurrentMove, gBattlerAttacker, gBattlerTarget);
                moveDmgs[checkedMove] = gBattleMoveDamage * AI_THINKING_STRUCT->simulatedRNG[checkedMove] / 100;
                if (gMoveResultFlags & MOVE_RESULT_NO_EFFECT)
                    moveDmgs[checkedMove] = 0;
                else if (moveDmgs[checkedMove] == 0)
                    moveDmgs[checkedMove] = 1;
            }
            else
            {
                moveDmgs[checkedMove] = 0;
            }
        }

        for (checkedMove = 0; checkedMove < MAX_MON_MOVES; checkedMove++)
        {
            if (moveDmgs[checkedMove] > moveDmgs[AI_THINKING_STRUCT->movesetIndex])
                break;
        }

        if (checkedMove == MAX_MON_MOVES)
            AI_THINKING_STRUCT->funcResult = MOVE_MOST_POWERFUL; // Is the most powerful.
        else
            AI_THINKING_STRUCT->funcResult = MOVE_NOT_MOST_POWERFUL; // Not the most powerful.
    }
    else
    {
        AI_THINKING_STRUCT->funcResult = MOVE_POWER_DISCOURAGED; // Highly discouraged in terms of power.
    }

    sAIScriptPtr++;
}

static void Cmd_get_last_used_battler_move(void)
{
    if (sAIScriptPtr[1] == AI_USER)
        AI_THINKING_STRUCT->funcResult = gLastMoves[gBattlerAttacker];
    else
        AI_THINKING_STRUCT->funcResult = gLastMoves[gBattlerTarget];

    sAIScriptPtr += 2;
}

static void Cmd_if_equal_(void) // Same as if_equal.
{
    if (sAIScriptPtr[1] == AI_THINKING_STRUCT->funcResult)
        sAIScriptPtr = T1_READ_PTR(sAIScriptPtr + 2);
    else
        sAIScriptPtr += 6;
}

static void Cmd_if_not_equal_(void) // Same as if_not_equal.
{
    if (sAIScriptPtr[1] != AI_THINKING_STRUCT->funcResult)
        sAIScriptPtr = T1_READ_PTR(sAIScriptPtr + 2);
    else
        sAIScriptPtr += 6;
}

static void Cmd_if_would_go_first(void)
{
    if (GetWhoStrikesFirst(gBattlerAttacker, gBattlerTarget, TRUE) == sAIScriptPtr[1])
        sAIScriptPtr = T1_READ_PTR(sAIScriptPtr + 2);
    else
        sAIScriptPtr += 6;
}

static void Cmd_if_would_not_go_first(void)
{
    if (GetWhoStrikesFirst(gBattlerAttacker, gBattlerTarget, TRUE) != sAIScriptPtr[1])
        sAIScriptPtr = T1_READ_PTR(sAIScriptPtr + 2);
    else
        sAIScriptPtr += 6;
}

static void Cmd_nullsub_2A(void)
{
}

static void Cmd_nullsub_2B(void)
{
}

static void Cmd_count_alive_pokemon(void)
{
    u8 battlerId;
    u8 battlerOnField1, battlerOnField2;
    struct Pokemon *party;
    s32 i;

    AI_THINKING_STRUCT->funcResult = 0;

    if (sAIScriptPtr[1] == AI_USER)
        battlerId = gBattlerAttacker;
    else
        battlerId = gBattlerTarget;

    if (GetBattlerSide(battlerId) == B_SIDE_PLAYER)
        party = gPlayerParty;
    else
        party = gEnemyParty;

    if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
    {
        u32 position;
        battlerOnField1 = gBattlerPartyIndexes[battlerId];
        position = GetBattlerPosition(battlerId) ^ BIT_FLANK;
        battlerOnField2 = gBattlerPartyIndexes[GetBattlerAtPosition(position)];
    }
    else // In singles there's only one battlerId by side.
    {
        battlerOnField1 = gBattlerPartyIndexes[battlerId];
        battlerOnField2 = gBattlerPartyIndexes[battlerId];
    }

    for (i = 0; i < PARTY_SIZE; i++)
    {
        if (i != battlerOnField1 && i != battlerOnField2
         && GetMonData(&party[i], MON_DATA_HP) != 0
         && GetMonData(&party[i], MON_DATA_SPECIES_OR_EGG) != SPECIES_NONE
         && GetMonData(&party[i], MON_DATA_SPECIES_OR_EGG) != SPECIES_EGG)
        {
            AI_THINKING_STRUCT->funcResult++;
        }
    }

    sAIScriptPtr += 2;
}

static void Cmd_get_considered_move(void)
{
    AI_THINKING_STRUCT->funcResult = AI_THINKING_STRUCT->moveConsidered;
    sAIScriptPtr += 1;
}

static void Cmd_get_considered_move_effect(void)
{
    AI_THINKING_STRUCT->funcResult = gBattleMoves[AI_THINKING_STRUCT->moveConsidered].effect;
    sAIScriptPtr += 1;
}

static void Cmd_get_ability(void)
{
    u8 battlerId;

    if (sAIScriptPtr[1] == AI_USER)
        battlerId = gBattlerAttacker;
    else
        battlerId = gBattlerTarget;

    if (gActiveBattler != battlerId)
    {
        if (BATTLE_HISTORY->abilities[battlerId] != 0)
        {
            AI_THINKING_STRUCT->funcResult = BATTLE_HISTORY->abilities[battlerId];
            sAIScriptPtr += 2;
            return;
        }

        // abilities that prevent fleeing.
        if (gBattleMons[battlerId].ability == ABILITY_SHADOW_TAG
        || gBattleMons[battlerId].ability == ABILITY_MAGNET_PULL
        || gBattleMons[battlerId].ability == ABILITY_ARENA_TRAP)
        {
            AI_THINKING_STRUCT->funcResult = gBattleMons[battlerId].ability;
            sAIScriptPtr += 2;
            return;
        }

        if (gSpeciesInfo[gBattleMons[battlerId].species].abilities[0] != ABILITY_NONE)
        {
            if (gSpeciesInfo[gBattleMons[battlerId].species].abilities[1] != ABILITY_NONE)
            {
                // AI has no knowledge of opponent, so it guesses which ability.
                if (Random() % 2)
                    AI_THINKING_STRUCT->funcResult = gSpeciesInfo[gBattleMons[battlerId].species].abilities[0];
                else
                    AI_THINKING_STRUCT->funcResult = gSpeciesInfo[gBattleMons[battlerId].species].abilities[1];
            }
            else
            {
                AI_THINKING_STRUCT->funcResult = gSpeciesInfo[gBattleMons[battlerId].species].abilities[0];
            }
        }
        else
        {
             // AI can't actually reach this part since no pokemon has ability 2 and no ability 1.
            AI_THINKING_STRUCT->funcResult = gSpeciesInfo[gBattleMons[battlerId].species].abilities[1];
        }
    }
    else
    {
        // The AI knows its own ability.
        AI_THINKING_STRUCT->funcResult = gBattleMons[battlerId].ability;
    }

    sAIScriptPtr += 2;
}

static void Cmd_get_highest_type_effectiveness(void)
{
    s32 i;
    u8 *dynamicMoveType;

    gDynamicBasePower = 0;
    dynamicMoveType = &gBattleStruct->dynamicMoveType;
    *dynamicMoveType = 0;
    gBattleScripting.dmgMultiplier = 1;
    gMoveResultFlags = 0;
    gCritMultiplier = 1;
    AI_THINKING_STRUCT->funcResult = 0;

    for (i = 0; i < MAX_MON_MOVES; i++)
    {
        gBattleMoveDamage = 40;
        gCurrentMove = gBattleMons[gBattlerAttacker].moves[i];

        if (gCurrentMove != MOVE_NONE)
        {
            gMoveResultFlags = TypeCalc(gCurrentMove, gBattlerAttacker, gBattlerTarget);

            if (gBattleMoveDamage == 120) // Super effective STAB.
                gBattleMoveDamage = AI_EFFECTIVENESS_x2;
            if (gBattleMoveDamage == 240)
                gBattleMoveDamage = AI_EFFECTIVENESS_x4;
            if (gBattleMoveDamage == 30) // Not very effective STAB.
                gBattleMoveDamage = AI_EFFECTIVENESS_x0_5;
            if (gBattleMoveDamage == 15)
                gBattleMoveDamage = AI_EFFECTIVENESS_x0_25;

            if (gMoveResultFlags & MOVE_RESULT_DOESNT_AFFECT_FOE)
                gBattleMoveDamage = AI_EFFECTIVENESS_x0;

            if (AI_THINKING_STRUCT->funcResult < gBattleMoveDamage)
                AI_THINKING_STRUCT->funcResult = gBattleMoveDamage;
        }
    }

    sAIScriptPtr += 1;
}

static void Cmd_if_type_effectiveness(void)
{
    u8 damageVar;

    gDynamicBasePower = 0;
    gBattleStruct->dynamicMoveType = 0;
    gBattleScripting.dmgMultiplier = 1;
    gMoveResultFlags = 0;
    gCritMultiplier = 1;

    gBattleMoveDamage = AI_EFFECTIVENESS_x1;
    gCurrentMove = AI_THINKING_STRUCT->moveConsidered;

    gMoveResultFlags = TypeCalc(gCurrentMove, gBattlerAttacker, gBattlerTarget);

    if (gBattleMoveDamage == 120) // Super effective STAB.
        gBattleMoveDamage = AI_EFFECTIVENESS_x2;
    if (gBattleMoveDamage == 240)
        gBattleMoveDamage = AI_EFFECTIVENESS_x4;
    if (gBattleMoveDamage == 30) // Not very effective STAB.
        gBattleMoveDamage = AI_EFFECTIVENESS_x0_5;
    if (gBattleMoveDamage == 15)
        gBattleMoveDamage = AI_EFFECTIVENESS_x0_25;

    if (gMoveResultFlags & MOVE_RESULT_DOESNT_AFFECT_FOE)
        gBattleMoveDamage = AI_EFFECTIVENESS_x0;

    // Store gBattleMoveDamage in a u8 variable because sAIScriptPtr[1] is a u8.
    damageVar = gBattleMoveDamage;

    if (damageVar == sAIScriptPtr[1])
        sAIScriptPtr = T1_READ_PTR(sAIScriptPtr + 2);
    else
        sAIScriptPtr += 6;
}

static void Cmd_nullsub_32(void)
{
}

static void Cmd_nullsub_33(void)
{
}

static void Cmd_if_status_in_party(void)
{
    struct Pokemon *party;
    struct Pokemon *partyPtr;
    s32 i;
    u32 statusToCompareTo;
    // u8 battlerId

    // for whatever reason, game freak put the party pointer into 2 variables instead of 1
    // it's possible at some point the switch encompassed the whole function and used each respective variable creating largely duplicate code.
    switch (sAIScriptPtr[1])
    {
    case 1:
        party = partyPtr = gEnemyParty;
        break;
    default:
        party = partyPtr = gPlayerParty;
        break;
    }

    /* Emerald's fixed version below
    switch (sAIScriptPtr[1])
    {
    case AI_USER:
        battlerId = gBattlerAttacker;
        break;
    default:
        battlerId = gBattlerTarget;
        break;
    }

    party = (GetBattlerSide(battlerId) == B_SIDE_PLAYER) ? gPlayerParty : gEnemyParty;
    */

    statusToCompareTo = T1_READ_32(sAIScriptPtr + 2);

    for (i = 0; i < PARTY_SIZE; i++)
    {
        u16 species = GetMonData(&party[i], MON_DATA_SPECIES);
        u16 hp = GetMonData(&party[i], MON_DATA_HP);
        u32 status = GetMonData(&party[i], MON_DATA_STATUS);

        if (species != SPECIES_NONE && species != SPECIES_EGG && hp != 0 && status == statusToCompareTo)
        {
            sAIScriptPtr = T1_READ_PTR(sAIScriptPtr + 6);
            return;
        }
    }

    sAIScriptPtr += 10;
}

// bugged, doesnt return properly. also unused
static void Cmd_if_status_not_in_party(void)
{
    struct Pokemon *party;
    struct Pokemon *partyPtr;
    s32 i;
    u32 statusToCompareTo;
    //u8 battlerId

    switch (sAIScriptPtr[1])
    {
    case 1:
        party = partyPtr = gEnemyParty;
        break;
    default:
        party = partyPtr = gPlayerParty;
        break;
    }

    statusToCompareTo = T1_READ_32(sAIScriptPtr + 2);

    for (i = 0; i < PARTY_SIZE; i++)
    {
        u16 species = GetMonData(&party[i], MON_DATA_SPECIES);
        u16 hp = GetMonData(&party[i], MON_DATA_HP);
        u32 status = GetMonData(&party[i], MON_DATA_STATUS);

        // everytime the status is found, the AI's logic jumps further and further past its intended destination. this results in a broken AI macro and is probably why it is unused.
        if (species != SPECIES_NONE && species != SPECIES_EGG && hp != 0 && status == statusToCompareTo)
        {
            sAIScriptPtr += 10; // doesnt return?
            #ifdef UBFIX
            return;
            #endif
        }
    }
    sAIScriptPtr = T1_READ_PTR(sAIScriptPtr + 6);
}

enum
{
    WEATHER_TYPE_SUNNY,
    WEATHER_TYPE_RAIN,
    WEATHER_TYPE_SANDSTORM,
    WEATHER_TYPE_HAIL,
};

extern u16 gBattleWeather;

static void Cmd_get_weather(void)
{
    if (gBattleWeather & B_WEATHER_RAIN)
        AI_THINKING_STRUCT->funcResult = WEATHER_TYPE_RAIN;
    if (gBattleWeather & B_WEATHER_SANDSTORM)
        AI_THINKING_STRUCT->funcResult = WEATHER_TYPE_SANDSTORM;
    if (gBattleWeather & B_WEATHER_SUN)
        AI_THINKING_STRUCT->funcResult = WEATHER_TYPE_SUNNY;
    if (gBattleWeather & B_WEATHER_HAIL_TEMPORARY)
        AI_THINKING_STRUCT->funcResult = WEATHER_TYPE_HAIL;

    sAIScriptPtr += 1;
}

static void Cmd_if_effect(void)
{
    if (gBattleMoves[AI_THINKING_STRUCT->moveConsidered].effect == sAIScriptPtr[1])
        sAIScriptPtr = T1_READ_PTR(sAIScriptPtr + 2);
    else
        sAIScriptPtr += 6;
}

static void Cmd_if_not_effect(void)
{
    if (gBattleMoves[AI_THINKING_STRUCT->moveConsidered].effect != sAIScriptPtr[1])
        sAIScriptPtr = T1_READ_PTR(sAIScriptPtr + 2);
    else
        sAIScriptPtr += 6;
}

static void Cmd_if_stat_level_less_than(void)
{
    u32 battlerId;

    if (sAIScriptPtr[1] == AI_USER)
        battlerId = gBattlerAttacker;
    else
        battlerId = gBattlerTarget;

    if (gBattleMons[battlerId].statStages[sAIScriptPtr[2]] < sAIScriptPtr[3])
        sAIScriptPtr = T1_READ_PTR(sAIScriptPtr + 4);
    else
        sAIScriptPtr += 8;
}

static void Cmd_if_stat_level_more_than(void)
{
    u32 battlerId;

    if (sAIScriptPtr[1] == AI_USER)
        battlerId = gBattlerAttacker;
    else
        battlerId = gBattlerTarget;

    if (gBattleMons[battlerId].statStages[sAIScriptPtr[2]] > sAIScriptPtr[3])
        sAIScriptPtr = T1_READ_PTR(sAIScriptPtr + 4);
    else
        sAIScriptPtr += 8;
}

static void Cmd_if_stat_level_equal(void)
{
    u32 battlerId;

    if (sAIScriptPtr[1] == AI_USER)
        battlerId = gBattlerAttacker;
    else
        battlerId = gBattlerTarget;

    if (gBattleMons[battlerId].statStages[sAIScriptPtr[2]] == sAIScriptPtr[3])
        sAIScriptPtr = T1_READ_PTR(sAIScriptPtr + 4);
    else
        sAIScriptPtr += 8;
}

static void Cmd_if_stat_level_not_equal(void)
{
    u32 battlerId;

    if (sAIScriptPtr[1] == AI_USER)
        battlerId = gBattlerAttacker;
    else
        battlerId = gBattlerTarget;

    if (gBattleMons[battlerId].statStages[sAIScriptPtr[2]] != sAIScriptPtr[3])
        sAIScriptPtr = T1_READ_PTR(sAIScriptPtr + 4);
    else
        sAIScriptPtr += 8;
}

static void Cmd_if_can_faint(void)
{
    if (gBattleMoves[AI_THINKING_STRUCT->moveConsidered].power < 2)
    {
        sAIScriptPtr += 5;
        return;
    }

    gDynamicBasePower = 0;
    gBattleStruct->dynamicMoveType = 0;
    gBattleScripting.dmgMultiplier = 1;
    gMoveResultFlags = 0;
    gCritMultiplier = 1;
    gCurrentMove = AI_THINKING_STRUCT->moveConsidered;
    AI_CalcDmg(gBattlerAttacker, gBattlerTarget);
    gMoveResultFlags = TypeCalc(gCurrentMove, gBattlerAttacker, gBattlerTarget);
    if (gMoveResultFlags & MOVE_RESULT_NO_EFFECT)
    {
        sAIScriptPtr += 5;
        return;
    }

    gBattleMoveDamage = gBattleMoveDamage * AI_THINKING_STRUCT->simulatedRNG[AI_THINKING_STRUCT->movesetIndex] / 100;

    // Moves always do at least 1 damage.
    if (gBattleMoveDamage == 0)
        gBattleMoveDamage = 1;

    if (gBattleMons[gBattlerTarget].hp <= gBattleMoveDamage)
        sAIScriptPtr = T1_READ_PTR(sAIScriptPtr + 1);
    else
        sAIScriptPtr += 5;
}

static void Cmd_if_cant_faint(void)
{
    if (gBattleMoves[AI_THINKING_STRUCT->moveConsidered].power < 2)
    {
        sAIScriptPtr += 5;
        return;
    }

    gDynamicBasePower = 0;
    gBattleStruct->dynamicMoveType = 0;
    gBattleScripting.dmgMultiplier = 1;
    gMoveResultFlags = 0;
    gCritMultiplier = 1;
    gCurrentMove = AI_THINKING_STRUCT->moveConsidered;
    AI_CalcDmg(gBattlerAttacker, gBattlerTarget);
    gMoveResultFlags = TypeCalc(gCurrentMove, gBattlerAttacker, gBattlerTarget);
    if (gMoveResultFlags & MOVE_RESULT_NO_EFFECT)
    {
        sAIScriptPtr = T1_READ_PTR(sAIScriptPtr + 1);
        return;
    }

    gBattleMoveDamage = gBattleMoveDamage * AI_THINKING_STRUCT->simulatedRNG[AI_THINKING_STRUCT->movesetIndex] / 100;

    // This macro is missing the damage 0 = 1 assumption.

    if (gBattleMons[gBattlerTarget].hp > gBattleMoveDamage)
        sAIScriptPtr = T1_READ_PTR(sAIScriptPtr + 1);
    else
        sAIScriptPtr += 5;
}

// The AI knows its own side's moves and only observed moves of opponents.
static bool8 AIHasMove(u8 who, u16 value, bool8 byEffect)
{
    u8 battler = (who == AI_USER || who == AI_USER_PARTNER) ? gBattlerAttacker : gBattlerTarget;
    u8 i;
    u16 move;
    if (who == AI_USER_PARTNER || who == AI_TARGET_PARTNER)
        battler = BATTLE_PARTNER(battler);
    if ((gAbsentBattlerFlags & gBitTable[battler]) || !gBattleMons[battler].hp)
        return FALSE;
    for (i = 0; i < MAX_MON_MOVES; i++)
    {
        move = GetBattlerSide(battler) == GetBattlerSide(gBattlerAttacker)
            ? gBattleMons[battler].moves[i] : BATTLE_HISTORY->usedMoves[battler].moves[i];
        if (move != MOVE_NONE && move < MOVES_COUNT
         && (byEffect ? gBattleMoves[move].effect == value : move == value))
            return TRUE;
    }
    return FALSE;
}

static void Cmd_if_has_move(void)
{
    if (AIHasMove(sAIScriptPtr[1], T1_READ_16(sAIScriptPtr + 2), FALSE))
        sAIScriptPtr = T1_READ_PTR(sAIScriptPtr + 4);
    else
        sAIScriptPtr += 8;
}

static void Cmd_if_doesnt_have_move(void)
{
    if (!AIHasMove(sAIScriptPtr[1], T1_READ_16(sAIScriptPtr + 2), FALSE))
        sAIScriptPtr = T1_READ_PTR(sAIScriptPtr + 4);
    else
        sAIScriptPtr += 8;
}

static void Cmd_if_has_move_with_effect(void)
{
    if (AIHasMove(sAIScriptPtr[1], sAIScriptPtr[2], TRUE))
        sAIScriptPtr = T1_READ_PTR(sAIScriptPtr + 3);
    else
        sAIScriptPtr += 7;
}

static void Cmd_if_doesnt_have_move_with_effect(void)
{
    if (!AIHasMove(sAIScriptPtr[1], sAIScriptPtr[2], TRUE))
        sAIScriptPtr = T1_READ_PTR(sAIScriptPtr + 3);
    else
        sAIScriptPtr += 7;
}

static void Cmd_if_any_move_disabled_or_encored(void)
{
    u8 battlerId;

    if (sAIScriptPtr[1] == AI_USER)
        battlerId = gBattlerAttacker;
    else
        battlerId = gBattlerTarget;

    if (sAIScriptPtr[2] == 0)
    {
        if (gDisableStructs[battlerId].disabledMove == MOVE_NONE)
            sAIScriptPtr += 7;
        else
            sAIScriptPtr = T1_READ_PTR(sAIScriptPtr + 3);
    }
    else if (sAIScriptPtr[2] != 1)
    {
        sAIScriptPtr += 7;
    }
    else
    {
        if (gDisableStructs[battlerId].encoredMove != MOVE_NONE)
            sAIScriptPtr = T1_READ_PTR(sAIScriptPtr + 3);
        else
            sAIScriptPtr += 7;
    }
}

static void Cmd_if_curr_move_disabled_or_encored(void)
{
    switch (sAIScriptPtr[1])
    {
    case 0:
        if (gDisableStructs[gActiveBattler].disabledMove == AI_THINKING_STRUCT->moveConsidered)
            sAIScriptPtr = T1_READ_PTR(sAIScriptPtr + 2);
        else
            sAIScriptPtr += 6;
        break;
    case 1:
        if (gDisableStructs[gActiveBattler].encoredMove == AI_THINKING_STRUCT->moveConsidered)
            sAIScriptPtr = T1_READ_PTR(sAIScriptPtr + 2);
        else
            sAIScriptPtr += 6;
        break;
    default:
        sAIScriptPtr += 6;
        break;
    }
}

static void Cmd_flee(void)
{
    AI_THINKING_STRUCT->aiAction |= (AI_ACTION_DONE | AI_ACTION_FLEE | AI_ACTION_DO_NOT_ATTACK); // what matters is AI_ACTION_FLEE being enabled.
}

static void Cmd_if_random_safari_flee(void)
{
    u8 safariFleeRate;

    if (gBattleStruct->safariRockThrowCounter)
    {
        safariFleeRate = gBattleStruct->safariEscapeFactor * 2;
        if (safariFleeRate > 20)
            safariFleeRate = 20;
    }
    else if (gBattleStruct->safariBaitThrowCounter != 0)
    {
        safariFleeRate = gBattleStruct->safariEscapeFactor / 4;
        if (safariFleeRate == 0)
            safariFleeRate = 1;
    }
    else
        safariFleeRate = gBattleStruct->safariEscapeFactor;
    safariFleeRate *= 5;
    if ((u8)(Random() % 100) < safariFleeRate)
        sAIScriptPtr = T1_READ_PTR(sAIScriptPtr + 1);
    else
        sAIScriptPtr += 5;
}

static void Cmd_watch(void)
{
    AI_THINKING_STRUCT->aiAction |= (AI_ACTION_DONE | AI_ACTION_WATCH | AI_ACTION_DO_NOT_ATTACK); // what matters is AI_ACTION_WATCH being enabled.
}

static void Cmd_get_hold_effect(void)
{
    u8 battlerId;

    if (sAIScriptPtr[1] == AI_USER)
        battlerId = gBattlerAttacker;
    else
        battlerId = gBattlerTarget;

    if (gActiveBattler != battlerId)
        AI_THINKING_STRUCT->funcResult = BATTLE_HISTORY->itemEffects[battlerId];

    else
        AI_THINKING_STRUCT->funcResult = ItemId_GetHoldEffect(gBattleMons[battlerId].item);

    sAIScriptPtr += 2;
}

static void Cmd_get_gender(void)
{
    u8 battlerId;

    if (sAIScriptPtr[1] == AI_USER)
        battlerId = gBattlerAttacker;
    else
        battlerId = gBattlerTarget;

    AI_THINKING_STRUCT->funcResult = GetGenderFromSpeciesAndPersonality(gBattleMons[battlerId].species, gBattleMons[battlerId].personality);

    sAIScriptPtr += 2;
}

static void Cmd_is_first_turn_for(void)
{
    u8 battlerId;

    if (sAIScriptPtr[1] == AI_USER)
        battlerId = gBattlerAttacker;
    else
        battlerId = gBattlerTarget;

    AI_THINKING_STRUCT->funcResult = gDisableStructs[battlerId].isFirstTurn;

    sAIScriptPtr += 2;
}

static void Cmd_get_stockpile_count(void)
{
    u8 battlerId;

    if (sAIScriptPtr[1] == AI_USER)
        battlerId = gBattlerAttacker;
    else
        battlerId = gBattlerTarget;

    AI_THINKING_STRUCT->funcResult = gDisableStructs[battlerId].stockpileCounter;

    sAIScriptPtr += 2;
}

static void Cmd_is_double_battle(void)
{
    AI_THINKING_STRUCT->funcResult = gBattleTypeFlags & BATTLE_TYPE_DOUBLE;

    sAIScriptPtr += 1;
}

static void Cmd_get_used_held_item(void)
{
    u8 battlerId;

    if (sAIScriptPtr[1] == AI_USER)
        battlerId = gBattlerAttacker;
    else
        battlerId = gBattlerTarget;

    AI_THINKING_STRUCT->funcResult = ((u8 *)gBattleStruct->usedHeldItems)[battlerId * 2];
    sAIScriptPtr += 2;
}

static void Cmd_get_move_type_from_result(void)
{
    AI_THINKING_STRUCT->funcResult = gBattleMoves[AI_THINKING_STRUCT->funcResult].type;

    sAIScriptPtr += 1;
}

static void Cmd_get_move_power_from_result(void)
{
    AI_THINKING_STRUCT->funcResult = gBattleMoves[AI_THINKING_STRUCT->funcResult].power;

    sAIScriptPtr += 1;
}

static void Cmd_get_move_effect_from_result(void)
{
    AI_THINKING_STRUCT->funcResult = gBattleMoves[AI_THINKING_STRUCT->funcResult].effect;

    sAIScriptPtr += 1;
}

static void Cmd_get_protect_count(void)
{
    u8 battlerId;

    if (sAIScriptPtr[1] == AI_USER)
        battlerId = gBattlerAttacker;
    else
        battlerId = gBattlerTarget;

    AI_THINKING_STRUCT->funcResult = gDisableStructs[battlerId].protectUses;

    sAIScriptPtr += 2;
}

static void Cmd_nullsub_52(void)
{
}

static void Cmd_nullsub_53(void)
{
}

static void Cmd_nullsub_54(void)
{
}

static void Cmd_nullsub_55(void)
{
}

static void Cmd_nullsub_56(void)
{
}

static void Cmd_nullsub_57(void)
{
}

static void Cmd_call(void)
{
    AIStackPushVar(sAIScriptPtr + 5);
    sAIScriptPtr = T1_READ_PTR(sAIScriptPtr + 1);
}

static void Cmd_goto(void)
{
    sAIScriptPtr = T1_READ_PTR(sAIScriptPtr + 1);
}

static void Cmd_end(void)
{
    if (AIStackPop() == FALSE)
        AI_THINKING_STRUCT->aiAction |= AI_ACTION_DONE;
}

static void Cmd_if_level_compare(void)
{
    switch (sAIScriptPtr[1])
    {
    case 0: // greater than
        if (gBattleMons[gBattlerAttacker].level > gBattleMons[gBattlerTarget].level)
        {
            sAIScriptPtr = T1_READ_PTR(sAIScriptPtr + 2);
            return;
        }
        sAIScriptPtr += 6;
        return;
    case 1: // less than
        if (gBattleMons[gBattlerAttacker].level < gBattleMons[gBattlerTarget].level)
        {
            sAIScriptPtr = T1_READ_PTR(sAIScriptPtr + 2);
            return;
        }
        sAIScriptPtr += 6;
        return;
    case 2: // equal
        if (gBattleMons[gBattlerAttacker].level == gBattleMons[gBattlerTarget].level)
        {
            sAIScriptPtr = T1_READ_PTR(sAIScriptPtr + 2);
            return;
        }
        sAIScriptPtr += 6;
        return;
    }
}

static void Cmd_if_target_taunted(void)
{
    if (gDisableStructs[gBattlerTarget].tauntTimer != 0)
        sAIScriptPtr = T1_READ_PTR(sAIScriptPtr + 1);
    else
        sAIScriptPtr += 5;
}

static void Cmd_if_target_not_taunted(void)
{
    if (gDisableStructs[gBattlerTarget].tauntTimer == 0)
        sAIScriptPtr = T1_READ_PTR(sAIScriptPtr + 1);
    else
        sAIScriptPtr += 5;
}

static void AIStackPushVar(const u8 *var)
{
    gBattleResources->AI_ScriptsStack->ptr[gBattleResources->AI_ScriptsStack->size++] = var;
}

// unused
static void AIStackPushVar_cursor(void)
{
    gBattleResources->AI_ScriptsStack->ptr[gBattleResources->AI_ScriptsStack->size++] = sAIScriptPtr;
}

static bool8 AIStackPop(void)
{
    if (gBattleResources->AI_ScriptsStack->size != 0)
    {
        --gBattleResources->AI_ScriptsStack->size;
        sAIScriptPtr = gBattleResources->AI_ScriptsStack->ptr[gBattleResources->AI_ScriptsStack->size];
        return TRUE;
    }
    else
        return FALSE;
}
