// Appended only to a disposable battle_script_commands.c, exposing private commands.
bool8 CaptureTestThrowWithBall(bool8 blocked, u16 ball)
{
    gBattleControllerExecFlags = 0;
    gBattlerAttacker = 0;
    gBattlerPartyIndexes[1] = 0;
    gBattleMons[1].species = SPECIES_CATERPIE;
    gBattleMons[1].maxHP = 20;
    gBattleMons[1].hp = 1;
    gBattleMons[1].status1 = STATUS1_PARALYSIS;
    gLastUsedItem = ball;
    Cmd_handleballthrow();
    return gBattlescriptCurrInstr == (blocked ? BattleScript_NuzlockeCaptureBlocked : BattleScript_SuccessBallThrow);
}
bool8 CaptureTestThrow(bool8 blocked)
{
    return CaptureTestThrowWithBall(blocked, ITEM_POKE_BALL);
}
void CaptureTestGiveCaughtMon(void)
{
    Cmd_givecaughtmon();
    Cmd_trysetcaughtmondexflags();
}
