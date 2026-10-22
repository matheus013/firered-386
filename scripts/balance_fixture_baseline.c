// Disposable benchmark only: legal player records are supplied by the planner.
// Native damage, enemy party/AI, EXP, PP, status, fainting and outcomes are intact.
#include "new_game.h"
#include "pokemon.h"
#include "script_pokemon_util.h"
#include "battle.h"
#include "nuzlocke.h"
#include "game_settings.h"
#include "event_data.h"
#include "overworld.h"
#include "string_util.h"
#include "random.h"
#include "item.h"
#include "constants/maps.h"
#include "constants/items.h"
#include "constants/pokemon.h"
extern void CB2_InitBattle(void);
IWRAM_DATA volatile u32 gBenchInput[112];
IWRAM_DATA volatile u32 gBenchOutput[192];
EWRAM_DATA volatile u32 gBenchReady,gBenchCommand;
static void BenchIdle(void);
static void BenchEnd(void)
{
    u8 i,j;
    gBenchOutput[0]=gBattleOutcome;
    gBenchOutput[1]=gBattleResults.battleTurnCounter;
    gBenchOutput[2]=gBattleResults.playerFaintCounter;
    gBenchOutput[5]=gBattleResults.numHealingItemsUsed;
    for(i=0;i<PARTY_SIZE;i++)
    {
        volatile u32 *out=&gBenchOutput[136+i*8];
        out[0]=GetMonData(&gPlayerParty[i],MON_DATA_SPECIES);
        out[1]=GetMonData(&gPlayerParty[i],MON_DATA_LEVEL);
        out[2]=GetMonData(&gPlayerParty[i],MON_DATA_HP);
        out[3]=GetMonData(&gPlayerParty[i],MON_DATA_MAX_HP);
        out[4]=GetMonData(&gPlayerParty[i],MON_DATA_STATUS);
        out[5]=Nuzlocke_IsDead(&gPlayerParty[i]);
        out[6]=GetMonData(&gPlayerParty[i],MON_DATA_EXP);
        out[7]=0;
        for(j=0;j<4;j++)out[7]|=GetMonData(&gPlayerParty[i],MON_DATA_PP1+j)<<(j*8);
    }
    gBenchReady=2;SetMainCallback2(BenchIdle);
}
static void BenchStart(void)
{
    u8 i,j,ev,iv,nature;
    u16 tagged,item;
    u32 personality,packed;
    static const u8 name[] = _("TEST");
    bool8 carry=gBenchInput[7];
    if(!carry)
    {
        StringCopy(gSaveBlock2Ptr->playerName,name);StringCopy(gSaveBlock1Ptr->rivalName,name);
        NewGameInitData();Nuzlocke_ChooseMode(gBenchInput[1]);
        FlagSet(FLAG_SYS_POKEMON_GET);FlagSet(FLAG_SYS_POKEDEX_GET);EnableNationalPokedex();
        if(gBenchInput[9]){FlagSet(FLAG_EXP_SHARE_UNLOCKED);Nuzlocke_Start();}
    }
    for(i=0;i<8;i++)
        if(i<gBenchInput[2])FlagSet(FLAG_BADGE01_GET+i);else FlagClear(FLAG_BADGE01_GET+i);
    if(gBenchInput[4])FlagSet(FLAG_SYS_GAME_CLEAR);else FlagClear(FLAG_SYS_GAME_CLEAR);
    for(i=0;i<4;i++)
        if(i<gBenchInput[3])FlagSet(FLAG_DEFEATED_LORELEI+i);else FlagClear(FLAG_DEFEATED_LORELEI+i);
    gSaveBlock2Ptr->optionsTextSpeed=2;gSaveBlock2Ptr->optionsBattleSceneOff=TRUE;
    gMapHeader=*Overworld_GetMapHeaderByGroupAndId(MAP_GROUP(MAP_ROUTE1),MAP_NUM(MAP_ROUTE1));
    SeedRng(gBenchInput[6]);gMain.inBattle=FALSE;
    if(!carry)
    {
        ZeroPlayerPartyMons();
        for(i=0;i<gBenchInput[5];i++)
        {
            volatile u32 *in=&gBenchInput[16+i*16];
            iv=in[2]>31?31:in[2];nature=in[3];personality=nature;
            while((personality&1)!=(in[4]&1))personality+=NUM_NATURES;
            CreateMon(&gPlayerParty[i],in[0],in[1],iv,TRUE,personality,OT_ID_PLAYER_ID,0);
            if(in[2]>31){packed=in[2];SetMonData(&gPlayerParty[i],MON_DATA_IVS,&packed);}
            item=in[5];SetMonData(&gPlayerParty[i],MON_DATA_HELD_ITEM,&item);
            for(j=0;j<6;j++){ev=in[6+j];SetMonData(&gPlayerParty[i],MON_DATA_HP_EV+j,&ev);}
            if(Nuzlocke_IsHardcore() || gBenchInput[4])
            {tagged=0xA500|nature;SetMonData(&gPlayerParty[i],MON_DATA_TRAINING_NATURE,&tagged);}
            for(j=0;j<4;j++)SetMonMoveSlot(&gPlayerParty[i],in[12+j],j);
            CalculateMonStats(&gPlayerParty[i]);
        }
        HealPlayerParty();CalculatePlayerPartyCount();
        if(gBenchInput[10])AddBagItem(ITEM_POTION,gBenchInput[10]);
    }
    else if(gBenchInput[8])
    {
        // Between League rooms: real item effects; preserve PP and permanent loss.
        for(i=0;i<gPlayerPartyCount;i++)
        {
            if(Nuzlocke_IsDead(&gPlayerParty[i]))continue;
            if(!GetMonData(&gPlayerParty[i],MON_DATA_HP) && !Nuzlocke_IsHardcore())
                if(!ExecuteTableBasedItemEffect(&gPlayerParty[i],ITEM_REVIVE,i,0))gBenchOutput[7]++;
            for(j=0;j<4 && GetMonData(&gPlayerParty[i],MON_DATA_HP)<GetMonData(&gPlayerParty[i],MON_DATA_MAX_HP);j++)
                if(!ExecuteTableBasedItemEffect(&gPlayerParty[i],ITEM_HYPER_POTION,i,0))gBenchOutput[6]++;else break;
            if(GetMonData(&gPlayerParty[i],MON_DATA_STATUS))
                if(!ExecuteTableBasedItemEffect(&gPlayerParty[i],ITEM_FULL_HEAL,i,0))gBenchOutput[8]++;
            if(Nuzlocke_IsHardcore())
            {
                while(GetMonData(&gPlayerParty[i],MON_DATA_LEVEL)<Nuzlocke_GetLevelCap())
                    if(!ExecuteTableBasedItemEffect(&gPlayerParty[i],ITEM_RARE_CANDY,i,0))gBenchOutput[9]++;else break;
            }
        }
    }
    gBenchOutput[3]=Nuzlocke_GetLevelCap();gBenchOutput[4]=gPlayerPartyCount;
    for(i=0;i<gPlayerPartyCount;i++)
    {
        volatile u32 *out=&gBenchOutput[16+i*20];
        out[0]=GetMonData(&gPlayerParty[i],MON_DATA_SPECIES);out[1]=GetMonData(&gPlayerParty[i],MON_DATA_LEVEL);
        out[2]=GetMonData(&gPlayerParty[i],MON_DATA_HP);out[3]=GetMonData(&gPlayerParty[i],MON_DATA_MAX_HP);
        out[4]=GetMonData(&gPlayerParty[i],MON_DATA_ATK);out[5]=GetMonData(&gPlayerParty[i],MON_DATA_DEF);
        out[6]=GetMonData(&gPlayerParty[i],MON_DATA_SPEED);out[7]=GetMonData(&gPlayerParty[i],MON_DATA_SPATK);out[8]=GetMonData(&gPlayerParty[i],MON_DATA_SPDEF);
        out[9]=GetMonData(&gPlayerParty[i],MON_DATA_EXP);out[10]=0;out[11]=0;
        for(j=0;j<6;j++){out[10]+=GetMonData(&gPlayerParty[i],MON_DATA_HP_EV+j);out[11]+=GetMonData(&gPlayerParty[i],MON_DATA_HP_IV+j);}
        for(j=0;j<4;j++)out[12+j]=GetMonData(&gPlayerParty[i],MON_DATA_MOVE1+j);
    }
    gBenchReady=0;gBattleTypeFlags=BATTLE_TYPE_TRAINER;gTrainerBattleOpponent_A=gBenchInput[0];
    gMain.savedCallback=BenchEnd;SetMainCallback2(CB2_InitBattle);
}
static void BenchIdle(void)
{
    if(gBenchCommand==1){gBenchCommand=0;memset((void *)gBenchOutput,0,sizeof(gBenchOutput));BenchStart();}
}
static void BenchBoot(void){SetSaveBlocksPointers();gBenchReady=1;SetMainCallback2(BenchIdle);}
