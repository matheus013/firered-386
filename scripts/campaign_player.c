// Test-only battle choices; damage, opponent AI, rewards and outcomes remain native.
static u16 CampaignTypeMultiplier(u8 type,u8 target)
{
    const u8 *entry=gTypeEffectiveness;
    u16 result=10;
    for (;entry[0]!=TYPE_ENDTABLE;entry+=3)
    {
        if (entry[0]==TYPE_FORESIGHT) continue;
        if (entry[0]==type && (entry[1]==gBattleMons[target].type1 || entry[1]==gBattleMons[target].type2))
            result=result*entry[2]/10;
    }
    if ((type==TYPE_GROUND && gBattleMons[target].ability==ABILITY_LEVITATE)
        || (type==TYPE_ELECTRIC && gBattleMons[target].ability==ABILITY_VOLT_ABSORB)
        || (type==TYPE_WATER && gBattleMons[target].ability==ABILITY_WATER_ABSORB)
        || (type==TYPE_FIRE && gBattleMons[target].ability==ABILITY_FLASH_FIRE)) return 0;
    return result;
}
static void CampaignChooseAction(void)
{
    BtlController_EmitTwoReturnValues(1,(gBattleTypeFlags&(BATTLE_TYPE_TRAINER|BATTLE_TYPE_GHOST))?B_ACTION_USE_MOVE:B_ACTION_USE_ITEM,0);
    PlayerBufferExecCompleted();
}
static void CampaignChooseItem(void)
{
    gSpecialVar_ItemId=ITEM_MASTER_BALL;
    BtlController_EmitOneReturnValue(1,ITEM_MASTER_BALL);
    PlayerBufferExecCompleted();
}
static void CampaignChooseMove(void)
{
    struct ChooseMoveStruct *info=(struct ChooseMoveStruct *)(&gBattleBufferA[gActiveBattler][4]);
    u8 i,target,bestMove=0,bestTarget=gActiveBattler^BIT_SIDE;
    u32 best=0,score;
    u16 move;
    for(i=0;i<MAX_MON_MOVES;i++)
    {
        move=info->moves[i];
        if(!move || !info->currentPp[i])continue;
        for(target=0;target<gBattlersCount;target++)
        {
            if(GetBattlerSide(target)==GetBattlerSide(gActiveBattler) || !gBattleMons[target].hp || (gAbsentBattlerFlags & gBitTable[target]))continue;
            score=gBattleMoves[move].power*CampaignTypeMultiplier(gBattleMoves[move].type,target);
            if(gBattleMoves[move].type<TYPE_MYSTERY)
                score=score*gBattleMons[gActiveBattler].attack/(gBattleMons[target].defense?gBattleMons[target].defense:1);
            else
                score=score*gBattleMons[gActiveBattler].spAttack/(gBattleMons[target].spDefense?gBattleMons[target].spDefense:1);
            if(move==MOVE_WATER_SPOUT)score=score*gBattleMons[gActiveBattler].hp/gBattleMons[gActiveBattler].maxHP;
            if(gBattleMons[target].ability==ABILITY_WONDER_GUARD && CampaignTypeMultiplier(gBattleMoves[move].type,target)<=10)score=0;
            if(gBattleMoves[move].type==gBattleMons[gActiveBattler].type1 || gBattleMoves[move].type==gBattleMons[gActiveBattler].type2)score=score*3/2;
            if(score>best){best=score;bestMove=i;bestTarget=target;}
        }
    }
    BtlController_EmitTwoReturnValues(1,10,bestMove|(bestTarget<<8));
    PlayerBufferExecCompleted();
}
static void CampaignChoosePokemon(void)
{
    u8 i,partner=GetBattlerAtPosition(GetBattlerPosition(gActiveBattler)^BIT_FLANK);
    for(i=0;i<PARTY_SIZE;i++)
        if(GetMonData(&gPlayerParty[i],MON_DATA_HP) && i!=gBattlerPartyIndexes[gActiveBattler]
            && (!(gBattleTypeFlags & BATTLE_TYPE_DOUBLE) || (gAbsentBattlerFlags & gBitTable[partner]) || i!=gBattlerPartyIndexes[partner]))break;
    BtlController_EmitChosenMonReturnValue(1,i,NULL);
    PlayerBufferExecCompleted();
}
