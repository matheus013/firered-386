// Decision/damage tests in a disposable ROM; no campaign victory is simulated.
IWRAM_DATA volatile u32 gDecisionDone, gDecisionChecks, gDecisionFailures;
IWRAM_DATA volatile u32 gDecisionFailureBits;
static void BenchGymPartyChecks(void);
static void BenchExpect(bool8 ok)
{
    if (!ok) {gDecisionFailures++;gDecisionFailureBits |= 1 << (gDecisionChecks % 32);}
    gDecisionChecks++;
}
static void BenchScores(void)
{
    u8 i;
    for(i=0;i<4;i++)gBattleResources->ai->score[i]=100;
}
void RunAIDecisionChecks(void)
{
    u8 i,j,choice;u16 flags;u32 a,b;
    gBattleTypeFlags=BATTLE_TYPE_TRAINER|BATTLE_TYPE_DOUBLE;
    gAbsentBattlerFlags=0;gBattlersCount=4;gActiveBattler=gBattlerAttacker=1;gBattlerTarget=0;
    memset(gBattleMons,0,sizeof(gBattleMons));memset(gDisableStructs,0,sizeof(gDisableStructs));
    memset(gChosenMoveByBattler,0,sizeof(gChosenMoveByBattler));
    for(i=0;i<4;i++)
    {
        gChosenActionByBattler[i]=B_ACTION_NONE;
        gBattleMons[i].hp=gBattleMons[i].maxHP=100;gBattleMons[i].level=50;
        gBattleMons[i].attack=100;gBattleMons[i].defense=100;
        gBattleMons[i].spAttack=100;gBattleMons[i].spDefense=100;
        gBattleMons[i].type1=gBattleMons[i].type2=TYPE_NORMAL;
        gBattleMons[i].species=SPECIES_SNORLAX;
        for(j=0;j<NUM_BATTLE_STATS;j++)gBattleMons[i].statStages[j]=DEFAULT_STAT_STAGE;
    }
    gBattleMons[1].moves[0]=MOVE_HELPING_HAND;gBattleMons[1].pp[0]=10;
    gBattleMons[3].moves[0]=MOVE_SURF;gBattleMons[3].pp[0]=10;
    BenchScores();ApplyDoublesScores();BenchExpect(gBattleResources->ai->score[0]>100);
    gChosenMoveByBattler[3]=MOVE_PROTECT;
    BenchScores();ApplyDoublesScores();BenchExpect(gBattleResources->ai->score[0]<100);
    gChosenMoveByBattler[3]=MOVE_NONE;gBattleMons[3].pp[0]=0;
    BenchScores();ApplyDoublesScores();BenchExpect(gBattleResources->ai->score[0]<100);
    gBattleMons[3].pp[0]=10;gBattleMons[3].status1=STATUS1_FREEZE;
    BenchScores();ApplyDoublesScores();BenchExpect(gBattleResources->ai->score[0]<100);
    gBattleMons[3].status1=0;
    BenchScores();gBattleResources->ai->score[0]=0;ApplyDoublesScores();BenchExpect(gBattleResources->ai->score[0]==0);
    gBattleMons[1].moves[0]=MOVE_ROCK_TOMB;gBattleMons[0].statStages[STAT_SPEED]=MIN_STAT_STAGE;
    BenchScores();ApplyDoublesScores();BenchExpect(gBattleResources->ai->score[0]==100);
    gBattleMons[1].moves[0]=MOVE_ICY_WIND;
    BenchScores();ApplyDoublesScores();BenchExpect(gBattleResources->ai->score[0]==102);
    gBattleMons[1].moves[0]=MOVE_REFLECT;gSideStatuses[1]=SIDE_STATUS_REFLECT;
    BenchScores();ApplyDoublesScores();BenchExpect(gBattleResources->ai->score[0]<100);
    gSideStatuses[1]=0;gChosenMoveByBattler[3]=MOVE_REFLECT;
    BenchScores();ApplyDoublesScores();BenchExpect(gBattleResources->ai->score[0]<100);
    gChosenMoveByBattler[3]=MOVE_NONE;gBattleMons[1].moves[0]=MOVE_PROTECT;gDisableStructs[1].protectUses=1;
    BenchScores();ApplyDoublesScores();BenchExpect(gBattleResources->ai->score[0]<90);
    gDisableStructs[1].protectUses=0;gBattleMons[1].moves[0]=MOVE_EARTHQUAKE;
    BenchScores();ApplyDoublesScores();BenchExpect(gBattleResources->ai->score[0]<100);
    gBattleMons[3].ability=ABILITY_LEVITATE;
    BenchScores();ApplyDoublesScores();BenchExpect(gBattleResources->ai->score[0]==100);
    gChosenMoveByBattler[3]=MOVE_HELPING_HAND;
    BenchScores();ApplyDoublesScores();BenchExpect(gBattleResources->ai->score[0]>100);
    gBattleMons[1].moves[0]=MOVE_RAIN_DANCE;
    BenchScores();ApplyDoublesScores();BenchExpect(gBattleResources->ai->score[0]<100);
    gChosenMoveByBattler[3]=MOVE_NONE;
    for(i=0;i<4;i++){gBattleMons[1].moves[i]=MOVE_NONE;gBattleMons[1].pp[i]=0;}
    gBattleMons[1].moves[3]=MOVE_WATER_GUN;gBattleMons[1].pp[3]=10;
    BattleAI_SetupAIData();gBattleResources->ai->aiFlags=0;
    for(i=0;i<4;i++)gBattleResources->ai->score[i]=0;
    BenchExpect(ChooseMoveForTarget()==3);
    gBattleMons[0].type1=gBattleMons[0].type2=TYPE_ROCK;
    gBattleMons[2].type1=gBattleMons[2].type2=TYPE_WATER;
    gBattleMons[0].species=SPECIES_NOSEPASS;
    gBattleMons[2].species=SPECIES_SQUIRTLE;
    for(i=0;i<16;i++)
    {
        SeedRng(400+i);BattleAI_SetupAIData();choice=BattleAI_ChooseMoveOrAction();
        BenchExpect(choice==3 && gBattlerTarget==0);
    }
    // Native damage tests: category, screens, burn, Thick Fat and physical weather.
    gBattleTypeFlags=BATTLE_TYPE_TRAINER;gBattleWeather=0;gCritMultiplier=1;
    gBattleMons[0].type1=gBattleMons[0].type2=TYPE_NORMAL;
    gBattleMons[1].attack=30;gBattleMons[1].spAttack=180;
    gCurrentMove=MOVE_BITE;
    a=CalculateBaseDamage(&gBattleMons[1],&gBattleMons[0],MOVE_BITE,0,0,0,1,0);
    gCurrentMove=MOVE_WATER_PULSE;
    b=CalculateBaseDamage(&gBattleMons[1],&gBattleMons[0],MOVE_WATER_PULSE,0,0,0,1,0);
    BenchExpect(b>a*3);
    gBattleMons[1].attack=300;gBattleMons[1].spAttack=30;gCurrentMove=MOVE_BITE;
    a=CalculateBaseDamage(&gBattleMons[1],&gBattleMons[0],MOVE_BITE,0,0,0,1,0);
    b=CalculateBaseDamage(&gBattleMons[1],&gBattleMons[0],MOVE_BITE,SIDE_STATUS_REFLECT,0,0,1,0);
    BenchExpect(a>b);
    b=CalculateBaseDamage(&gBattleMons[1],&gBattleMons[0],MOVE_BITE,SIDE_STATUS_LIGHTSCREEN,0,0,1,0);
    BenchExpect(a==b);
    gBattleMons[1].status1=STATUS1_BURN;
    b=CalculateBaseDamage(&gBattleMons[1],&gBattleMons[0],MOVE_BITE,0,0,0,1,0);BenchExpect(a>b);
    gBattleMons[1].status1=0;gCurrentMove=MOVE_FIRE_PUNCH;
    a=CalculateBaseDamage(&gBattleMons[1],&gBattleMons[0],MOVE_FIRE_PUNCH,0,0,0,1,0);
    gBattleWeather=B_WEATHER_SUN_TEMPORARY;
    b=CalculateBaseDamage(&gBattleMons[1],&gBattleMons[0],MOVE_FIRE_PUNCH,0,0,0,1,0);BenchExpect(b>a);
    gBattleWeather=0;gBattleMons[0].ability=ABILITY_THICK_FAT;
    b=CalculateBaseDamage(&gBattleMons[1],&gBattleMons[0],MOVE_FIRE_PUNCH,0,0,0,1,0);BenchExpect(b<a);
    BenchExpect(IS_MOVE_SPECIAL(MOVE_SHADOW_BALL) && IS_MOVE_PHYSICAL(MOVE_FIRE_PUNCH) && gBattleMoves[MOVE_GROWL].category==MOVE_CATEGORY_STATUS);
    BenchGymPartyChecks();
    gDecisionDone=1;
}
