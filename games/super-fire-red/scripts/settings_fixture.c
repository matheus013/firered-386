// Test-only fixture: persistence, randomizer invariants and actual NPC party creation.
#include "game_settings.h"
#include "localization.h"
#include "nuzlocke.h"
#include "new_game.h"
#include "save.h"
#include "load_save.h"
#include "pokemon.h"
#include "battle.h"
#include "elite_four.h"
#include "event_data.h"
#include "string_util.h"
#include "constants/opponents.h"
#include "constants/pokemon.h"
#include "constants/moves.h"
#include "constants/battle.h"
#include "constants/maps.h"
#include "pokedex.h"
extern u8 SettingsTestCreateParty(u16 trainer);
extern bool8 SettingsTestLocale(void);
extern u8 SettingsTestTowerParty(u8 course,u8 floor,u8 opponent);
extern u16 SettingsTestOriginalField(u16 trainer,u8 slot,bool8 level);
extern bool8 SettingsTestLegend(u16 species);
extern void SettingsTestWild(u16 species,u8 level);
EWRAM_DATA u32 gHardcoreTestResult=0;
EWRAM_DATA u32 gSettingsAudit[12]={0};
#define CHECK(condition,error) if(!(condition)){gHardcoreTestResult=error;return;}
static void SettingsTestIdle(void) {}
static void SettingsTestBoot(void)
{
    static const u8 name[] = _("TEST");
    static const u8 en[] = _("TEXT SPEED");
    static const u8 pt[] = _("TEXTO");
    bool8 seen[387]={FALSE};
    u16 species,mapped,national,count=0,trainer,total,move,starter[3];
    u8 i,j,mode,course,floor,opponent,opponents;
    u32 seed;
    SetMainCallback2(SettingsTestIdle);
    SetSaveBlocksPointers();
    gHardcoreTestResult=100;
    CHECK(sizeof(struct SaveBlock2)==0xF24 && offsetof(struct SaveBlock2,encryptionKey)==0xF20,10);
    if(LoadGameSave(SAVE_NORMAL)==SAVE_STATUS_OK)
    {
        CHECK(GameSettings_GetLanguage()==GAME_LANGUAGE_PTBR && GameSettings_IsRandomizer() && Nuzlocke_IsActive(),11);
        seed=VarGet(0x40FE)|((u32)VarGet(0x40FF)<<16);
        CHECK(seed==gSaveBlock2Ptr->challenge.randomizerSeed,12);
        CHECK(Randomizer_Starter(0)==GetMonData(&gPlayerParty[0],MON_DATA_SPECIES),13);
        CHECK(gSaveBlock2Ptr->challenge.encounters[2]==0x40 && gSaveBlock2Ptr->challenge.centerHeals[3]==2,14);
        CHECK(StringCompare(L10n_GetText(en),pt)==0,15);
        gHardcoreTestResult=2;return;
    }
    StringCopy(gSaveBlock2Ptr->playerName,name);StringCopy(gSaveBlock1Ptr->rivalName,name);
    NewGameInitData();
    gSaveBlock2Ptr->challenge.settingsMagic=0;
    gSaveBlock2Ptr->challenge.randomizer=TRUE;gSaveBlock2Ptr->challenge.language=GAME_LANGUAGE_PTBR;
    CHECK(GameSettings_GetLanguage()==GAME_LANGUAGE_EN && !GameSettings_IsRandomizer(),16);
    GameSettings_SetLanguage(GAME_LANGUAGE_PTBR);Nuzlocke_ChooseMode(GAME_MODE_NORMAL);
    CHECK(GameSettings_GetLanguage()==GAME_LANGUAGE_PTBR && !GameSettings_IsRandomizer(),17);
    CHECK(StringCompare(L10n_GetText(en),pt)==0 && SettingsTestLocale(),18);
    GameSettings_SetLanguage(GAME_LANGUAGE_EN);CHECK(L10n_GetText(en)==en,19);
    CHECK(Randomizer_Species(SPECIES_BULBASAUR)==SPECIES_BULBASAUR,20);
    Nuzlocke_ChooseMode(GAME_MODE_HARDCORE);GameSettings_ChooseRandomizer(TRUE);Nuzlocke_Start();
    gBattleTypeFlags=0;gSaveBlock1Ptr->location.mapGroup=MAP_GROUP(MAP_ROUTE1);gSaveBlock1Ptr->location.mapNum=MAP_NUM(MAP_ROUTE1);
    SettingsTestWild(SPECIES_RATTATA,5);mapped=Randomizer_Species(SPECIES_RATTATA);
    CHECK(GetMonData(&gEnemyParty[0],MON_DATA_SPECIES)==mapped && GetMonData(&gEnemyParty[0],MON_DATA_LEVEL)==5,44);
    Nuzlocke_OnBattleStart();CHECK(Nuzlocke_CanCatch(),45);Nuzlocke_OnBattleStart();CHECK(!Nuzlocke_CanCatch(),46);
    GetSetPokedexFlag(SpeciesToNationalPokedexNum(mapped),FLAG_SET_SEEN);
    GetSetPokedexFlag(SpeciesToNationalPokedexNum(mapped),FLAG_SET_CAUGHT);
    gSaveBlock1Ptr->location.mapGroup=MAP_GROUP(MAP_ROUTE2);gSaveBlock1Ptr->location.mapNum=MAP_NUM(MAP_ROUTE2);
    SettingsTestWild(SPECIES_RATTATA,5);Nuzlocke_OnBattleStart();CHECK(!Nuzlocke_CanCatch(),47);
    SettingsTestWild(SPECIES_PIDGEY,5);Nuzlocke_OnBattleStart();CHECK(Nuzlocke_CanCatch(),48);
    SettingsTestWild(SPECIES_MEW,90);
    CHECK(!SettingsTestLegend(GetMonData(&gEnemyParty[0],MON_DATA_SPECIES)) && GetMonData(&gEnemyParty[0],MON_DATA_LEVEL)==12,49);
    FlagSet(FLAG_SYS_GAME_CLEAR);SettingsTestWild(SPECIES_MEW,90);
    CHECK(GetMonData(&gEnemyParty[0],MON_DATA_SPECIES)==Randomizer_Species(SPECIES_MEW) && GetMonData(&gEnemyParty[0],MON_DATA_LEVEL)==90,50);
    FlagClear(FLAG_SYS_GAME_CLEAR);gSettingsAudit[10]=7;
    Nuzlocke_ChooseMode(GAME_MODE_NORMAL);
    for(mode=0;mode<2;mode++)
    {
        GameSettings_ChooseRandomizer(mode);
        gSettingsAudit[0]=mode;
        if(mode)
        {
            seed=gSaveBlock2Ptr->challenge.randomizerSeed;
            for(species=1;species<SPECIES_EGG;species++)
            {
                national=SpeciesToNationalPokedexNum(species);
                if(national==0 || national>386)continue;
                mapped=Randomizer_Species(species);national=SpeciesToNationalPokedexNum(mapped);
                CHECK(national>0 && national<=386 && !seen[national],21);
                seen[national]=TRUE;count++;
                CHECK(mapped==Randomizer_Species(species),22);
                CHECK(SettingsTestLegend(species)==SettingsTestLegend(mapped),51);
            }
            CHECK(count==386 && seed==gSaveBlock2Ptr->challenge.randomizerSeed,23);
            gSettingsAudit[1]=count;
            for(i=0;i<3;i++)
            {
                starter[i]=Randomizer_Starter(i);
                CHECK(starter[i]==Randomizer_Starter(i),24);
                for(j=0;j<i;j++)CHECK(starter[i]!=starter[j],25);
                VarSet(VAR_TEMP_1,i);Randomizer_PrepareStarter();
                CHECK(VarGet(VAR_TEMP_2)==starter[i] && VarGet(VAR_TEMP_3)==Randomizer_Starter((i+2)%3),26);
            }
        }
        for(trainer=1;trainer<NUM_TRAINERS;trainer++)
        {
            if(!gTrainers[trainer].partySize)continue;
            gSettingsAudit[2]=trainer;
            gBattleTypeFlags=BATTLE_TYPE_TRAINER;
            CHECK(SettingsTestCreateParty(trainer)==gTrainers[trainer].partySize,27);
            for(i=0;i<gTrainers[trainer].partySize;i++)
            {
                gSettingsAudit[3]=i;
                species=GetMonData(&gEnemyParty[i],MON_DATA_SPECIES);
                CHECK(species>0 && species<SPECIES_EGG && !GetMonData(&gEnemyParty[i],MON_DATA_SANITY_IS_BAD_EGG),28);
                CHECK(GetMonData(&gEnemyParty[i],MON_DATA_LEVEL)==SettingsTestOriginalField(trainer,i,TRUE),52);
                if(!mode)CHECK(species==SettingsTestOriginalField(trainer,i,FALSE),53);
                gSettingsAudit[11]++;
                if(EliteFour_IsChallenge(trainer))
                {
                    CHECK(GetMonData(&gEnemyParty[i],MON_DATA_IVS)==0x3FFFFFFF,29);
                    total=0;
                    for(j=0;j<NUM_STATS;j++)
                    {
                        u16 ev=GetMonData(&gEnemyParty[i],MON_DATA_HP_EV+j);
                        CHECK(ev<=252,30);total+=ev;
                    }
                    CHECK(total>=508 && total<=510,31);
                    gSettingsAudit[4+mode]++;
                }
                if(mode)
                {
                    bool8 attack=FALSE;
                    for(j=0;j<MAX_MON_MOVES;j++)
                    {
                        move=GetMonData(&gEnemyParty[i],MON_DATA_MOVE1+j);
                        CHECK(move<MOVES_COUNT,32);
                        CHECK(gBattleMoves[move].target!=MOVE_TARGET_FOES_AND_ALLY,38);
                        if(gBattleMoves[move].power)attack=TRUE;
                        CHECK(GetMonData(&gEnemyParty[i],MON_DATA_PP1+j)==gBattleMoves[move].pp,33);
                    }
                    CHECK(attack,34);gSettingsAudit[6]++;
                }
            }
        }
        CreateMon(&gPlayerParty[0],SPECIES_BULBASAUR,50,15,FALSE,0,OT_ID_PRESET,0);
        CalculatePlayerPartyCount();
        for(course=0;course<4;course++)for(floor=0;floor<8;floor++)
        {
            opponent=0;
            do
            {
                opponents=SettingsTestTowerParty(course,floor,opponent++);
                for(i=0;i<2;i++)
                {
                    CHECK(GetMonData(&gEnemyParty[i],MON_DATA_SPECIES)!=SPECIES_NONE,39);
                    CHECK(GetMonData(&gEnemyParty[i],MON_DATA_LEVEL)==50,40);
                    CHECK(GetMonData(&gEnemyParty[i],MON_DATA_IVS)==0x3FFFFFFF,41);
                    total=0;
                    for(j=0;j<NUM_STATS;j++)
                    {
                        u16 ev=GetMonData(&gEnemyParty[i],MON_DATA_HP_EV+j);
                        CHECK(ev<=252,42);total+=ev;
                    }
                    CHECK(total==508,43);gSettingsAudit[8+mode]++;
                }
            }while(opponent<opponents);
        }
    }
    GameSettings_SetLanguage(GAME_LANGUAGE_PTBR);Nuzlocke_ChooseMode(GAME_MODE_HARDCORE);
    CHECK(GameSettings_GetLanguage()==GAME_LANGUAGE_PTBR && !GameSettings_IsRandomizer(),35);
    GameSettings_ChooseRandomizer(TRUE);Nuzlocke_Start();
    gSaveBlock2Ptr->challenge.encounters[2]=0x40;gSaveBlock2Ptr->challenge.centerHeals[3]=2;
    GameSettings_SetLanguage(GAME_LANGUAGE_EN);GameSettings_SetLanguage(GAME_LANGUAGE_PTBR);
    CHECK(gSaveBlock2Ptr->challenge.encounters[2]==0x40 && gSaveBlock2Ptr->challenge.centerHeals[3]==2,36);
    seed=gSaveBlock2Ptr->challenge.randomizerSeed;VarSet(0x40FE,seed);VarSet(0x40FF,seed>>16);
    CreateMon(&gPlayerParty[0],Randomizer_Starter(0),5,15,FALSE,0,OT_ID_PRESET,0);
    CalculatePlayerPartyCount();
    CHECK(TrySavingData(SAVE_NORMAL)==SAVE_STATUS_OK,37);
    gSettingsAudit[7]=1;gHardcoreTestResult=1;
}
