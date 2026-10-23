#ifndef GUARD_MODERN_BATTLE_H
#define GUARD_MODERN_BATTLE_H

// Battle state for generation-nine field effects. Allocated with gBattleStruct, so it lives
// on the heap and starts zeroed in every battle.

#define TERRAIN_NONE     0
#define TERRAIN_ELECTRIC 1
#define TERRAIN_GRASSY   2
#define TERRAIN_MISTY    3
#define TERRAIN_PSYCHIC  4

struct ModernBattleState
{
    u8 terrain;
    u8 terrainTimer;
    u8 trickRoomTimer;
    u8 gravityTimer;
    u8 magicRoomTimer;
    u8 wonderRoomTimer;
    u8 snow;                // the current hail was started by Snowscape: no damage, Ice-type Defense boost
    u8 tailwindTimer[2];    // per side
    u8 pendingEnds;         // field effects that ran out this turn, printed one by one
    u8 stealthRock[2];      // per side
    u8 toxicSpikes[2];      // layers per side
    u8 stickyWeb[2];
    u8 hazardDone[4];       // hazards already applied to the battler switching in
    // Batch B4c-e. "Turn" markers store the last battle turn (plus one) during which a state lasts.
    u8 roosted[4];
    u8 roostTypes[4][2];
    u8 aquaRing[4];
    u8 magnetRiseTimer[4];
    u8 laserFocusTurn[4];
    u8 throatChopTurn[4];
    u8 healBlockTurn[4];
    u8 smackedDown[4];
    u8 spikyShieldTurn[4];
    u8 lastMoveFailed[4];
    u8 movesUsed[4];
    u8 weightLoss[4];
    u8 critBoost[4];
    u8 statsRaisedTurn[4];
    u8 statsLoweredTurn[4];
    u8 sideGuardTurn[2][2]; // [side][0 = Quick Guard, 1 = Wide Guard]
    u8 healingWish[2];
    u8 allyFaintedTurn[2];
    u8 roundTurn[2];
    u8 echoedVoiceTurn;
    u8 echoedVoiceCount;
    u8 rageFistHits[2][6];
    u8 ateBerry[2];         // party bitmask per side
    u16 lastMoveUsed;
    u8 scriptIter;          // Magnetic Flux walks the user's side
};

extern struct ModernBattleState *gModernBattle;

bool32 IsBattlerGroundedModern(u8 battler);
bool32 IsModernTerrainActive(u8 terrain);
s8 GetModernMovePriority(u8 battler, u16 move);
bool32 IsModernMoveBannedByGravity(u16 move);
void DecrementModernFieldTimers(void);
bool32 TryPrintModernFieldEnd(void);
void Cmd_trysetfieldeffect(void);
u32 GetStealthRockDamage(u8 battler);
void ClearModernBattlerState(u8 battler, bool32 batonPass);
bool32 IsModernLevitating(u8 battler);
extern const u16 gModernMiscStringIds[];

#define MODERN_MSG_SIDE_GUARD   0
#define MODERN_MSG_AFTER_YOU    1
#define MODERN_MSG_QUASH        2
#define MODERN_MSG_AQUA_RING    3
#define MODERN_MSG_MAGNET_RISE  4
#define MODERN_MSG_LASER_FOCUS  5
#define MODERN_MSG_SMACK_DOWN   6
#define MODERN_MSG_HEAL_BLOCK   7
#define MODERN_MSG_INCINERATE   8
#define MODERN_MSG_BUG_BITE     9
#define MODERN_MSG_DRAGON_CHEER 10
#define MODERN_MSG_AUTOTOMIZE   11
void UpdateModernBattlerStatesEndTurn(void);
bool32 IsModernStateActiveThisTurn(u8 marker);
bool32 IsBattlerMagnetRisen(u8 battler);
bool32 IsBattlerForcedGrounded(u8 battler);
bool32 IsModernSoundMove(u16 move);
bool32 IsModernHealingMove(u16 move);
bool32 IsModernMoveBlocked(u8 battler, u16 move);
void RecordModernStatChange(u8 battler, bool32 raised);
void RecordModernFaint(u8 battler);
void RecordModernHitTaken(u8 battler);
void RecordModernBerryEaten(u8 battler);
void RecordModernMoveUse(u8 battler, u16 chosenMove, u16 move, u8 resultFlags);
u16 GetModernMemoryMovePower(u16 move, u8 battlerAtk);
bool32 IsModernLaserFocused(u8 battler);
u8 GetModernCritBoost(u8 battler);
u32 GetModernWeightLoss(u8 battler);
bool32 DoModernStatusMode(u8 mode);
const u8 *GetModernDispatchScript(u16 move);

#endif // GUARD_MODERN_BATTLE_H
