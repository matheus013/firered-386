// Native API stress tests, injected only into a disposable ROM.
#include "new_game.h"
#include "load_save.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "nuzlocke.h"
#include "training.h"
#include "event_data.h"
#include "roamer.h"
#include "daycare.h"
#include "string_util.h"
#include "constants/items.h"
#include "constants/pokemon.h"
#include "constants/maps.h"
extern void CreateRoamerMonInstance(void);
extern u32 CommunityCountShinies(void);
extern u16 TakePokemonFromRoute5Daycare(void);
extern u8 GetNumLevelsGainedForRoute5DaycareMon(void);
extern void GetCostToWithdrawRoute5DaycareMon(void);
EWRAM_DATA u32 gCommunityDone=0,gCommunityPassed=0,gCommunityFailed=0;
EWRAM_DATA u32 gCommunityObserved[6]={0};
#define RECORD(bit, condition) do {if(condition)gCommunityPassed|=1u<<(bit);else gCommunityFailed|=1u<<(bit);}while(0)
static void CommunityIdle(void) {}
static void CommunityBoot(void)
{
    static const u8 name[] = _("TEST");
    static const u16 from[4]={SPECIES_KADABRA,SPECIES_MACHOKE,SPECIES_GRAVELER,SPECIES_HAUNTER};
    static const u16 to[4]={SPECIES_ALAKAZAM,SPECIES_MACHAMP,SPECIES_GOLEM,SPECIES_GENGAR};
    static const u32 variedIvs[]={0,1,0x12345678,0x15555555,0x2AAAAAAA,0x3FFFFFFF};
    static const u8 statuses[]={0,1,7,8,16,32,64,128};
    struct Pokemon mon, restored;
    struct BoxPokemon box;
    u32 packed=0x3FFFFFFF, personality;
    u16 item=ITEM_EVERSTONE;
    u8 i,b,p,ret,level;
    bool8 good;
    SetMainCallback2(CommunityIdle);
    SetSaveBlocksPointers();
    StringCopy(gSaveBlock2Ptr->playerName,name);StringCopy(gSaveBlock1Ptr->rivalName,name);
    NewGameInitData();Nuzlocke_ChooseMode(GAME_MODE_NORMAL);EnableNationalPokedex();
    RECORD(0,IsNationalPokedexEnabled());
    for(i=0;i<4;i++){
        CreateMon(&mon,from[i],36,31,TRUE,100,OT_ID_PLAYER_ID,0);
        good=GetEvolutionTargetSpecies(&mon,EVO_MODE_NORMAL,0)==0;
        CreateMon(&mon,from[i],37,31,TRUE,100,OT_ID_PLAYER_ID,0);
        RECORD(i+1,good&&GetEvolutionTargetSpecies(&mon,EVO_MODE_NORMAL,0)==to[i]);
    }
    SetMonData(&mon,MON_DATA_HELD_ITEM,&item);
    RECORD(5,GetEvolutionTargetSpecies(&mon,EVO_MODE_NORMAL,0)==0);
    CreateMon(&mon,SPECIES_SENTRET,15,31,TRUE,100,OT_ID_PLAYER_ID,0);
    RECORD(6,GetEvolutionTargetSpecies(&mon,EVO_MODE_NORMAL,0)==SPECIES_FURRET);
    CreateMon(&mon,SPECIES_PIKACHU,20,31,TRUE,100,OT_ID_PLAYER_ID,0);
    RECORD(7,GetEvolutionTargetSpecies(&mon,EVO_MODE_ITEM_CHECK,ITEM_THUNDER_STONE)==SPECIES_RAICHU);
    {
        static const u16 tradeFrom[6]={SPECIES_ONIX,SPECIES_SEADRA,SPECIES_SCYTHER,SPECIES_PORYGON,SPECIES_POLIWHIRL,SPECIES_CLAMPERL};
        static const u16 tradeTo[6]={SPECIES_STEELIX,SPECIES_KINGDRA,SPECIES_SCIZOR,SPECIES_PORYGON2,SPECIES_POLITOED,SPECIES_HUNTAIL};
        good=TRUE;
        for(i=0;i<6;i++){
            CreateMon(&mon,tradeFrom[i],36,31,TRUE,100,OT_ID_PLAYER_ID,0);
            good=good&&GetEvolutionTargetSpecies(&mon,EVO_MODE_NORMAL,0)==0;
            CreateMon(&mon,tradeFrom[i],37,31,TRUE,100,OT_ID_PLAYER_ID,0);
            good=good&&GetEvolutionTargetSpecies(&mon,EVO_MODE_NORMAL,0)==tradeTo[i];
        }
        RECORD(28,good);
        CreateMon(&mon,SPECIES_SLOWPOKE,20,31,TRUE,100,OT_ID_PLAYER_ID,0);
        good=GetEvolutionTargetSpecies(&mon,EVO_MODE_ITEM_USE,ITEM_KINGS_ROCK)==SPECIES_SLOWKING;
        CreateMon(&mon,SPECIES_POLIWHIRL,20,31,TRUE,100,OT_ID_PLAYER_ID,0);
        good=good&&GetEvolutionTargetSpecies(&mon,EVO_MODE_ITEM_USE,ITEM_WATER_STONE)==SPECIES_POLIWRATH;
        CreateMon(&mon,SPECIES_CLAMPERL,20,31,TRUE,100,OT_ID_PLAYER_ID,0);
        good=good&&GetEvolutionTargetSpecies(&mon,EVO_MODE_ITEM_USE,ITEM_DEEP_SEA_SCALE)==SPECIES_GOREBYSS;
        RECORD(29,good&&GetEvolutionTargetSpecies(&mon,EVO_MODE_TRADE,ITEM_NONE)==0);
    }
    CreateMon(&mon,SPECIES_BULBASAUR,5,31,TRUE,0,OT_ID_PRESET,0);
    good=IsMonShiny(&mon);
    CreateMon(&mon,SPECIES_BULBASAUR,5,31,TRUE,255,OT_ID_PRESET,0);
    good=good&&IsMonShiny(&mon);
    CreateMon(&mon,SPECIES_BULBASAUR,5,31,TRUE,256,OT_ID_PRESET,0);
    RECORD(8,good&&!IsMonShiny(&mon));
    gCommunityObserved[0]=CommunityCountShinies();
    RECORD(9,gCommunityObserved[0]==256);
    CreateMonWithIVsPersonality(&mon,SPECIES_RAIKOU,50,packed,1234);
    gCommunityObserved[1]=GetMonData(&mon,MON_DATA_IVS);
    RECORD(10,gCommunityObserved[1]==packed);
    gSaveBlock1Ptr->roamer.species=SPECIES_RAIKOU;gSaveBlock1Ptr->roamer.level=50;
    gSaveBlock1Ptr->roamer.ivs=packed;gSaveBlock1Ptr->roamer.personality=1234;
    gSaveBlock1Ptr->roamer.hp=20;gSaveBlock1Ptr->roamer.status=0;
    gSaveBlock1Ptr->roamer.cool=1;gSaveBlock1Ptr->roamer.beauty=2;gSaveBlock1Ptr->roamer.cute=4;
    CreateRoamerMonInstance();
    gCommunityObserved[2]=GetMonData(&gEnemyParty[0],MON_DATA_IVS);
    RECORD(11,gCommunityObserved[2]==packed);
    gCommunityObserved[3]=GetMonData(&gEnemyParty[0],MON_DATA_STATUS);
    RECORD(12,gCommunityObserved[3]==0);
    good=TRUE;
    for(i=0;i<ARRAY_COUNT(variedIvs);i++){
        CreateMonWithIVsPersonality(&mon,SPECIES_RAIKOU,50,variedIvs[i],1234);
        good=good&&GetMonData(&mon,MON_DATA_IVS)==variedIvs[i]&&!GetMonData(&mon,MON_DATA_SANITY_IS_BAD_EGG);
    }
    RECORD(18,good);
    good=TRUE;
    for(i=0;i<ARRAY_COUNT(statuses);i++){
        gSaveBlock1Ptr->roamer.status=statuses[i];
        CreateRoamerMonInstance();
        good=good&&GetMonData(&gEnemyParty[0],MON_DATA_STATUS)==statuses[i]
            &&GetMonData(&gEnemyParty[0],MON_DATA_HP)==20
            &&GetMonData(&gEnemyParty[0],MON_DATA_COOL)==1
            &&GetMonData(&gEnemyParty[0],MON_DATA_BEAUTY)==2
            &&GetMonData(&gEnemyParty[0],MON_DATA_CUTE)==4;
    }
    RECORD(19,good);
    // An EXP cap should also be checked in temporary mons used on daycare withdrawal.
    for(b=0;b<2;b++){
        Nuzlocke_ChooseMode(b?GAME_MODE_HARDCORE:GAME_MODE_NORMAL);
        ZeroPlayerPartyMons();CreateMon(&gPlayerParty[0],SPECIES_BULBASAUR,5,15,FALSE,0,OT_ID_PLAYER_ID,0);CalculatePlayerPartyCount();
        CreateMon(&mon,SPECIES_PIDGEY,5,15,TRUE,5678,OT_ID_PLAYER_ID,0);
        gSaveBlock1Ptr->daycare.mons[0].mon=mon.box;
        gSaveBlock1Ptr->daycare.mons[0].steps=50000;gSpecialVar_0x8004=0;
        good=GetNumLevelsGainedFromDaycare()==13;
        GetDaycareCost();
        RECORD(20+b,good&&gSpecialVar_0x8005==1400);
        TakePokemonFromDaycare();level=0;
        for(i=0;i<PARTY_SIZE;i++) if(GetMonData(&gPlayerParty[i],MON_DATA_SPECIES)==SPECIES_PIDGEY) level=GetMonData(&gPlayerParty[i],MON_DATA_LEVEL);
        gCommunityObserved[4+b]=level;
        RECORD(13+b,level&&level<=Nuzlocke_GetLevelCap());
        ZeroPlayerPartyMons();
        CreateMon(&gPlayerParty[0],SPECIES_BULBASAUR,5,15,FALSE,0,OT_ID_PLAYER_ID,0);CalculatePlayerPartyCount();
        gSaveBlock1Ptr->route5DayCareMon.mon=mon.box;
        gSaveBlock1Ptr->route5DayCareMon.steps=50000;
        good=GetNumLevelsGainedForRoute5DaycareMon()==13;
        GetCostToWithdrawRoute5DaycareMon();
        good=good&&gSpecialVar_0x8005==1400;
        TakePokemonFromRoute5Daycare();
        RECORD(22+b,good&&GetMonData(&gPlayerParty[1],MON_DATA_LEVEL)==18);
    }
    // Steps are valued against the cap at withdrawal; existing over-level mons keep their level.
    Nuzlocke_ChooseMode(GAME_MODE_NORMAL);
    CreateMon(&mon,SPECIES_PIDGEY,5,15,TRUE,5678,OT_ID_PLAYER_ID,0);
    gSaveBlock1Ptr->daycare.mons[0].mon=mon.box;
    gSaveBlock1Ptr->daycare.mons[0].steps=50000;
    FlagSet(FLAG_BADGE01_GET);
    RECORD(24,GetNumLevelsGainedFromDaycare()==23);
    FlagClear(FLAG_BADGE01_GET);
    CreateMon(&mon,SPECIES_PIDGEY,25,15,TRUE,5678,OT_ID_PLAYER_ID,0);
    packed=GetMonData(&mon,MON_DATA_EXP);
    RECORD(25,Nuzlocke_ClampTrainingExperience(&mon,packed+50000)==packed);
    CreateMon(&mon,SPECIES_PIDGEY,5,15,TRUE,5678,OT_ID_PLAYER_ID,0);
    packed=GetMonData(&mon,MON_DATA_EXP);
    RECORD(26,Nuzlocke_ClampTrainingExperience(&mon,packed+1)==packed+1);
    FlagSet(FLAG_SYS_GAME_CLEAR);
    RECORD(27,Nuzlocke_ClampTrainingExperience(&mon,packed+50000)==packed+50000);
    FlagClear(FLAG_SYS_GAME_CLEAR);
    Nuzlocke_ChooseMode(GAME_MODE_HARDCORE);
    CreateMon(&mon,SPECIES_BULBASAUR,5,31,TRUE,0,OT_ID_PRESET,0);
    personality=GetMonData(&mon,MON_DATA_PERSONALITY);
    good=Training_SetNature(&mon,NATURE_MODEST);
    box=mon.box;BoxMonToMon(&box,&restored);
    RECORD(15,good&&GetNature(&restored)==NATURE_MODEST&&IsMonShiny(&restored)&&GetMonData(&restored,MON_DATA_PERSONALITY)==personality&&!GetMonData(&restored,MON_DATA_SANITY_IS_BAD_EGG));
    Nuzlocke_ChooseMode(GAME_MODE_NORMAL);
    for(i=0;i<PARTY_SIZE;i++)CreateMon(&gPlayerParty[i],SPECIES_PIDGEY,5,15,FALSE,0,OT_ID_PLAYER_ID,0);
    CalculatePlayerPartyCount();
    ResetPokemonStorageSystem();
    for(b=0;b<TOTAL_BOXES_COUNT;b++)for(p=0;p<IN_BOX_COUNT;p++)CreateBoxMon(GetBoxedMonPtr(b,p),SPECIES_RATTATA,5,15,TRUE,100,OT_ID_PLAYER_ID,0);
    CreateMon(&mon,SPECIES_DUNSPARCE,5,31,TRUE,100,OT_ID_PLAYER_ID,0);
    ret=GiveMonToPlayer(&mon);
    RECORD(16,ret==MON_CANT_GIVE&&GetBoxMonData(GetBoxedMonPtr(13,29),MON_DATA_SPECIES)==SPECIES_RATTATA);
    ZeroBoxMonData(GetBoxedMonPtr(13,29));ret=GiveMonToPlayer(&mon);
    RECORD(17,ret==MON_GIVEN_TO_PC&&GetBoxMonData(GetBoxedMonPtr(13,29),MON_DATA_SPECIES)==SPECIES_DUNSPARCE&&!GetBoxMonData(GetBoxedMonPtr(13,29),MON_DATA_SANITY_IS_BAD_EGG));
    gCommunityDone=1;
}
