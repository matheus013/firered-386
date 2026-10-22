// Declared deterministic player policy, no edits to damage or enemy decisions.
#include "battle_util.h"
static u16 BenchEffect(u8 type,u8 target)
{
    const u8 *entry=gTypeEffectiveness;u16 result=10;
    for(;entry[0]!=TYPE_ENDTABLE;entry+=3)
    {
        if(entry[0]==TYPE_FORESIGHT)continue;
        if(entry[0]==type && (entry[1]==gBattleMons[target].type1 || entry[1]==gBattleMons[target].type2))result=result*entry[2]/10;
    }
    if((type==TYPE_GROUND&&gBattleMons[target].ability==ABILITY_LEVITATE)
        ||(type==TYPE_ELECTRIC&&gBattleMons[target].ability==ABILITY_VOLT_ABSORB)
        ||(type==TYPE_WATER&&gBattleMons[target].ability==ABILITY_WATER_ABSORB)
        ||(type==TYPE_FIRE&&gBattleMons[target].ability==ABILITY_FLASH_FIRE))return 0;
    if(gBattleMons[target].ability==ABILITY_WONDER_GUARD && result<=10)return 0;
    return result;
}
static u32 BenchMoveScore(u16 move,u8 target)
{
    u32 score;u16 power=gBattleMoves[move].power,effect=BenchEffect(gBattleMoves[move].type,target);
    u8 partner=GetBattlerAtPosition(GetBattlerPosition(gActiveBattler)^BIT_FLANK);
    if(!power)
    {
        if(move==MOVE_THUNDER_WAVE && !gBattleMons[target].status1 && BenchEffect(TYPE_ELECTRIC,target)
            && gBattlersCount==4 && gBattleMons[target].speed>gBattleMons[partner].speed)return gBattleMons[target].hp/2;
        if((move==MOVE_SPORE||move==MOVE_SLEEP_POWDER||move==MOVE_HYPNOSIS) && !gBattleMons[target].status1
            && gBattleMons[target].ability!=ABILITY_INSOMNIA && gBattleMons[target].ability!=ABILITY_VITAL_SPIRIT)
            return gBattleMons[target].hp*gBattleMoves[move].accuracy/250;
        if((move==MOVE_RECOVER||move==MOVE_MILK_DRINK||move==MOVE_SOFT_BOILED||move==MOVE_SYNTHESIS)
            && gBattleMons[gActiveBattler].hp*5<gBattleMons[gActiveBattler].maxHP*2)
            return gBattleMons[gActiveBattler].maxHP/2;
        if(move==MOVE_GROWL && gBattlersCount==2 && gBattleMons[target].statStages[STAT_ATK]>4 && gBattleResults.battleTurnCounter<2)return 7;
        return 0;
    }
    if(!effect)return 0;
    if(gBattleMoves[move].target==MOVE_TARGET_FOES_AND_ALLY && gBattlersCount==4
        && !(gAbsentBattlerFlags&gBitTable[partner]) && gBattleMons[partner].hp && BenchEffect(gBattleMoves[move].type,partner))return 0;
    if(move==MOVE_WATER_SPOUT||move==MOVE_ERUPTION)power=power*gBattleMons[gActiveBattler].hp/gBattleMons[gActiveBattler].maxHP;
    if(power==1)
    {
        if(move==MOVE_DRAGON_RAGE)return 40;
        if(move==MOVE_SONIC_BOOM)return 20;
        if(move==MOVE_SEISMIC_TOSS||move==MOVE_NIGHT_SHADE)return gBattleMons[gActiveBattler].level;
        return 0;
    }
    score=(2*gBattleMons[gActiveBattler].level/5+2)*power;
    if(gBattleMoves[move].type<TYPE_MYSTERY)score=score*gBattleMons[gActiveBattler].attack/(gBattleMons[target].defense?gBattleMons[target].defense:1);
    else score=score*gBattleMons[gActiveBattler].spAttack/(gBattleMons[target].spDefense?gBattleMons[target].spDefense:1);
    score=score/50+2;score=score*effect/10;
    if(gBattleMoves[move].type==gBattleMons[gActiveBattler].type1||gBattleMoves[move].type==gBattleMons[gActiveBattler].type2)score=score*3/2;
    score=score*(gBattleMoves[move].accuracy?gBattleMoves[move].accuracy:100)/100;
    if(move==MOVE_FAKE_OUT && gDisableStructs[gActiveBattler].isFirstTurn)return gBattleMons[target].hp/2+score;
    {
        u8 j;
        for(j=0;j<4;j++)if(gBattleMons[target].moves[j]==MOVE_WATER_SPOUT||gBattleMons[target].moves[j]==MOVE_ERUPTION)
            {score=score*3/2;break;}
    }
    if(gBattleMoves[move].priority>0 && score>=gBattleMons[target].hp)score+=40;
    if(score>gBattleMons[target].hp+10)score=gBattleMons[target].hp+10;
    return score;
}
extern volatile u32 gBenchInput[112];
static void BenchChooseAction(void)
{
    if(gBenchInput[10] && !gBenchInput[1] && gBattleMons[gActiveBattler].hp*2<=gBattleMons[gActiveBattler].maxHP && CheckBagHasItem(ITEM_POTION,1))
        BtlController_EmitTwoReturnValues(1,B_ACTION_USE_ITEM,0);
    else BtlController_EmitTwoReturnValues(1,B_ACTION_USE_MOVE,0);
    PlayerBufferExecCompleted();
}
static void BenchChooseMove(void)
{
    struct ChooseMoveStruct *info=(struct ChooseMoveStruct *)(&gBattleBufferA[gActiveBattler][4]);
    u8 i,target,bestMove=0xFF,bestTarget=gActiveBattler^BIT_SIDE,blocked=CheckMoveLimitations(gActiveBattler,0,MOVE_LIMITATIONS_ALL);u32 best=0,score;
    for(i=0;i<4;i++)if(info->moves[i]&&info->currentPp[i]&&!(blocked&gBitTable[i]))
    {
        if(bestMove==0xFF)bestMove=i;
        for(target=0;target<gBattlersCount;target++)
            if(GetBattlerSide(target)!=GetBattlerSide(gActiveBattler)&&gBattleMons[target].hp&&!(gAbsentBattlerFlags&gBitTable[target]))
            {score=BenchMoveScore(info->moves[i],target);if(score>best){best=score;bestMove=i;bestTarget=target;}}
    }
    if(bestMove==0xFF)bestMove=0;
    BtlController_EmitTwoReturnValues(1,10,bestMove|(bestTarget<<8));PlayerBufferExecCompleted();
}
static void BenchChoosePokemon(void)
{
    u8 i,j,best=PARTY_SIZE,partner=GetBattlerAtPosition(GetBattlerPosition(gActiveBattler)^BIT_FLANK);u32 bestScore=0,score;
    for(i=0;i<gPlayerPartyCount;i++)
        if(GetMonData(&gPlayerParty[i],MON_DATA_HP)&&i!=gBattlerPartyIndexes[gActiveBattler]
            &&(!(gBattleTypeFlags&BATTLE_TYPE_DOUBLE)||(gAbsentBattlerFlags&gBitTable[partner])||i!=gBattlerPartyIndexes[partner]))
        {
            score=GetMonData(&gPlayerParty[i],MON_DATA_SPEED)+GetMonData(&gPlayerParty[i],MON_DATA_HP);
            for(j=0;j<4;j++)score+=gBattleMoves[GetMonData(&gPlayerParty[i],MON_DATA_MOVE1+j)].power;
            if(best==PARTY_SIZE||score>bestScore){best=i;bestScore=score;}
        }
    BtlController_EmitChosenMonReturnValue(1,best,NULL);PlayerBufferExecCompleted();
}
