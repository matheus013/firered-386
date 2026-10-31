// Decision/damage tests in a disposable ROM; no campaign victory is simulated.
#include "berry.h"
IWRAM_DATA volatile u32 gDecisionDone, gDecisionChecks, gDecisionFailures;
IWRAM_DATA volatile u32 gDecisionFailureBits;
IWRAM_DATA volatile u32 gDecisionFirstFailure;
static void BenchGymPartyChecks(void);
static void BenchNatureChecks(void);
static void BenchExpect(bool8 ok)
{
    if (!ok) {if (!gDecisionFailures) gDecisionFirstFailure=gDecisionChecks;gDecisionFailures++;gDecisionFailureBits |= 1 << (gDecisionChecks % 32);}
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
    u8 script[8] = {0};
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
    // Equal script scores: choose damage across move slots, not just targets.
    gBattleMons[0].type1=gBattleMons[0].type2=TYPE_NORMAL;
    gBattleMons[2].type1=gBattleMons[2].type2=TYPE_NORMAL;
    gBattleMons[1].moves[0]=MOVE_TACKLE;gBattleMons[1].pp[0]=10;
    gBattleMons[1].moves[1]=MOVE_RETURN;gBattleMons[1].pp[1]=10;
    gBattleMons[1].moves[3]=MOVE_STRENGTH;gBattleMons[1].friendship=255;
    for(i=0;i<16;i++)
    {
        SeedRng(800+i);BattleAI_SetupAIData();gBattleResources->ai->aiFlags=0;
        choice=ChooseMoveForTarget();BenchExpect(choice==1);
    }
    // Spread value must include both foes and exclude an absent flank.
    gBattlerTarget=0;
    a=AIMoveDamageValue(MOVE_SURF);b=AIDamageValue(MOVE_SURF,0);
    BenchExpect(a==b+AIDamageValue(MOVE_SURF,2) && a>b);
    gAbsentBattlerFlags=4;BenchExpect(AIMoveDamageValue(MOVE_SURF)==AIDamageValue(MOVE_SURF,0));gAbsentBattlerFlags=0;
    gBattleMons[0].ability=ABILITY_LEVITATE;BenchExpect(AIDamageValue(MOVE_EARTHQUAKE,0)==0);
    gBattleMons[0].ability=ABILITY_WATER_ABSORB;BenchExpect(AIDamageValue(MOVE_SURF,0)==0);
    gBattleMons[0].ability=ABILITY_FLASH_FIRE;BenchExpect(AIDamageValue(MOVE_FLAMETHROWER,0)==0);
    gBattleMons[0].ability=ABILITY_VOLT_ABSORB;BenchExpect(AIDamageValue(MOVE_THUNDER,0)==0);
    gBattleMons[0].ability=0;gBattleMons[0].hp=gBattleMons[0].maxHP=200;
    gBattleMons[0].type1=gBattleMons[0].type2=TYPE_WATER;
    BenchExpect(AIDamageValue(MOVE_NIGHT_SHADE,0)==250);
    gBattleMons[0].type1=gBattleMons[0].type2=TYPE_NORMAL;
    BenchExpect(AIDamageValue(MOVE_NIGHT_SHADE,0)==0);
    gBattleMons[0].type1=gBattleMons[0].type2=TYPE_GHOST;
    BenchExpect(AIDamageValue(MOVE_SEISMIC_TOSS,0)==0);
    BenchExpect(AIDamageValue(MOVE_NIGHT_SHADE,0)==250);
    gBattleMons[0].type1=gBattleMons[0].type2=TYPE_NORMAL;
    gBattleMons[0].hp=gBattleMons[0].maxHP=100;
    gBattleWeather=B_WEATHER_RAIN_TEMPORARY;a=AIDamageValue(MOVE_THUNDER,0);
    gBattleWeather=B_WEATHER_SUN_TEMPORARY;b=AIDamageValue(MOVE_THUNDER,0);
    BenchExpect(a==b*2 || a==b*2+1);
    gBattleMons[2].ability=ABILITY_AIR_LOCK;BenchExpect(!AIWeatherHasEffect());
    BenchExpect(AIDamageValue(MOVE_THUNDER,0)>b && AIDamageValue(MOVE_THUNDER,0)<a);
    gBattleMons[1].moves[0]=MOVE_RAIN_DANCE;gBattleMons[1].type1=TYPE_WATER;
    BenchScores();ApplyDoublesScores();BenchExpect(gBattleResources->ai->score[0]<100);
    gBattleMons[2].hp=0;BenchExpect(AIWeatherHasEffect());gBattleMons[2].hp=100;
    gAbsentBattlerFlags=4;BenchExpect(AIWeatherHasEffect());gAbsentBattlerFlags=0;
    gBattleMons[2].ability=0;gBattleWeather=0;gBattleMons[1].type1=TYPE_NORMAL;
    gBattleMons[3].ability=0;gChosenActionByBattler[3]=B_ACTION_USE_MOVE;
    gChosenMoveByBattler[3]=MOVE_PROTECT;gBattleMons[1].moves[0]=MOVE_EARTHQUAKE;
    BenchScores();ApplyDoublesScores();BenchExpect(gBattleResources->ai->score[0]==100);
    gDisableStructs[3].protectUses=1;
    BenchScores();ApplyDoublesScores();BenchExpect(gBattleResources->ai->score[0]<100);
    gDisableStructs[3].protectUses=0;gChosenMoveByBattler[3]=0;gChosenActionByBattler[3]=B_ACTION_NONE;
    gBattleMons[1].moves[0]=MOVE_THUNDER_WAVE;gChosenMoveByBattler[3]=MOVE_WILL_O_WISP;
    gChosenActionByBattler[3]=B_ACTION_USE_MOVE;gBattleStruct->moveTarget[3]=0;
    BenchScores();ApplyDoublesScores();BenchExpect(gBattleResources->ai->score[0]==88);
    gBattleStruct->moveTarget[3]=2;
    BenchScores();ApplyDoublesScores();BenchExpect(gBattleResources->ai->score[0]==100);
    gChosenMoveByBattler[3]=0;gChosenActionByBattler[3]=B_ACTION_NONE;
    // Per-battler history: do not mix flanks or infer unobserved opposing moves.
    memset(BATTLE_HISTORY,0,sizeof(*BATTLE_HISTORY));
    for(i=0;i<4;i++)gBattlerPartyIndexes[i]=i/2;
    gBattlerTarget=0;gLastMoves[0]=MOVE_SURF;RecordLastUsedMoveByTarget();
    RecordLastUsedMoveByTarget();
    BenchExpect(BATTLE_HISTORY->usedMoves[0].moves[0]==MOVE_SURF && BATTLE_HISTORY->usedMoves[0].moves[1]==0);
    gBattlerTarget=2;gLastMoves[2]=MOVE_THUNDER;RecordLastUsedMoveByTarget();gBattlerTarget=0;
    BenchExpect(AIHasMove(AI_TARGET,MOVE_SURF,FALSE));
    BenchExpect(!AIHasMove(AI_TARGET,MOVE_THUNDER,FALSE));
    BenchExpect(AIHasMove(AI_TARGET_PARTNER,MOVE_THUNDER,FALSE));
    gBattleMons[2].moves[0]=MOVE_ICE_BEAM;
    BenchExpect(!AIHasMove(AI_TARGET_PARTNER,MOVE_ICE_BEAM,FALSE));
    gBattleMons[1].moves[0]=MOVE_GROWL;gBattleMons[3].moves[0]=MOVE_HELPING_HAND;
    BenchExpect(AIHasMove(AI_USER_PARTNER,MOVE_HELPING_HAND,FALSE));
    BenchExpect(!AIHasMove(AI_USER,MOVE_HELPING_HAND,FALSE));
    BenchExpect(AIHasMove(AI_TARGET_PARTNER,EFFECT_THUNDER,TRUE));
    RecordAbilityBattle(0,ABILITY_WATER_ABSORB);RecordAbilityBattle(2,ABILITY_STATIC);
    BenchExpect(BATTLE_HISTORY->abilities[0]==ABILITY_WATER_ABSORB && BATTLE_HISTORY->abilities[2]==ABILITY_STATIC);
    gBattlerPartyIndexes[0]=4;gLastMoves[0]=0;RecordLastUsedMoveByTarget();
    BenchExpect(BATTLE_HISTORY->usedMoves[0].moves[0]==0 && BATTLE_HISTORY->abilities[0]==0);
    BenchExpect(BATTLE_HISTORY->usedMoves[2].moves[0]==MOVE_THUNDER);
    // Native script opcodes must consume TypeCalc's returned immunity flags.
    gBattleMons[0].ability=ABILITY_LEVITATE;gBattleMons[0].hp=1;
    gBattleMons[1].moves[0]=MOVE_EARTHQUAKE;
    for(i=1;i<4;i++)gBattleMons[1].moves[i]=0;
    gBattleResources->ai->moveConsidered=MOVE_EARTHQUAKE;
    gBattleResources->ai->movesetIndex=0;gBattleResources->ai->simulatedRNG[0]=100;
    sAIScriptPtr=script;Cmd_if_can_faint();BenchExpect(sAIScriptPtr==script+5);
    sAIScriptPtr=script;Cmd_if_cant_faint();BenchExpect(sAIScriptPtr==NULL);
    sAIScriptPtr=script;Cmd_get_highest_type_effectiveness();BenchExpect(gBattleResources->ai->funcResult==0);
    gBattleMons[0].hp=100;
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
    // A nature set by the training editor must drive battle checks (confusion berries)
    // and the editor's stat colors, not the personality's nature.
    {
        struct Pokemon *mon=&gPlayerParty[gBattlerPartyIndexes[0]];
        u8 own=GetMonData(mon,MON_DATA_PERSONALITY)%NUM_NATURES,edited=(own+1)%NUM_NATURES;
        u16 tag=0xA500|edited,clear=0;
        SetMonData(mon,MON_DATA_TRAINING_NATURE,&tag);
        BenchExpect(GetBattlerNature(0)==edited);
        BenchExpect(GetFlavorRelationByNature(GetBattlerNature(0),FLAVOR_SPICY)==GetFlavorRelationByNature(edited,FLAVOR_SPICY));
        SetMonData(mon,MON_DATA_TRAINING_NATURE,&clear);
        BenchExpect(GetBattlerNature(0)==own);
        BenchExpect(GetFlavorRelationByNature(NATURE_MODEST,FLAVOR_SPICY)<0 && GetFlavorRelationByNature(NATURE_ADAMANT,FLAVOR_SPICY)>0);
        BenchExpect(GetNatureStatEffect(NATURE_CALM,STAT_SPDEF)>0 && GetNatureStatEffect(NATURE_CALM,STAT_ATK)<0);
        BenchExpect(GetNatureStatEffect(NATURE_CALM,STAT_DEF)==0 && GetNatureStatEffect(NATURE_CALM,STAT_SPEED)==0 && GetNatureStatEffect(NATURE_CALM,STAT_HP)==0);
        BenchExpect(GetNatureStatEffect(NATURE_HARDY,STAT_ATK)==0);
    }
    // Wild mons always hold a species item (70% common / 30% rare); no items stays empty.
    {
        static const u16 sSpecies[]={SPECIES_RATICATE,SPECIES_BUTTERFREE,SPECIES_VULPIX,SPECIES_SNORLAX,SPECIES_BULBASAUR};
        u32 flags=gBattleTypeFlags;
        u16 counts[5][2],item;
        u8 s,n;
        struct Pokemon saved=gEnemyParty[0];
        for(s=0;s<5;s++)
        {
            counts[s][0]=counts[s][1]=0;
            for(n=0;n<100;n++)
            {
                CreateMon(&gEnemyParty[0],sSpecies[s],10,0,FALSE,0,OT_ID_PLAYER_ID,0);
                gBattleTypeFlags=0;SetWildMonHeldItem();
                item=GetMonData(&gEnemyParty[0],MON_DATA_HELD_ITEM);
                if(item==gSpeciesInfo[sSpecies[s]].itemCommon && item!=ITEM_NONE)counts[s][0]++;
                else if(item==gSpeciesInfo[sSpecies[s]].itemRare && item!=ITEM_NONE)counts[s][1]++;
            }
        }
        BenchExpect(counts[0][0]+counts[0][1]==100 && counts[0][0]>counts[0][1] && counts[0][1]>10);
        BenchExpect(counts[1][1]==100 && counts[2][0]==100 && counts[3][0]==100);
        BenchExpect(counts[4][0]+counts[4][1]==0);
        CreateMon(&gEnemyParty[0],SPECIES_RATICATE,10,0,FALSE,0,OT_ID_PLAYER_ID,0);
        gBattleTypeFlags=BATTLE_TYPE_TRAINER;SetWildMonHeldItem();
        BenchExpect(GetMonData(&gEnemyParty[0],MON_DATA_HELD_ITEM)==ITEM_NONE);
        gEnemyParty[0]=saved;gBattleTypeFlags=flags;
    }
    BenchNatureChecks();
    BenchGymPartyChecks();
    gDecisionDone=1;
}
