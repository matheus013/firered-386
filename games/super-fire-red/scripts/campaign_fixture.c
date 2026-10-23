// Assisted integration fixture: field scripts and battles are real, travel/team are supplied.
#include "new_game.h"
#include "load_save.h"
#include "save.h"
#include "pokemon.h"
#include "script_pokemon_util.h"
#include "nuzlocke.h"
#include "event_data.h"
#include "overworld.h"
#include "script.h"
#include "string_util.h"
#include "event_object_movement.h"
#include "field_fadetransition.h"
#include "battle.h"
#include "battle_setup.h"
#include "item.h"
#include "constants/maps.h"
#include "constants/moves.h"
#include "constants/game_stat.h"
#include "constants/trainer_tower.h"
#include "campaign_cases.h"
extern bool8 CampaignIsField(void);
extern void CallTrainerTowerFunc(void);
EWRAM_DATA u32 gCampaignCommand, gCampaignCase, gCampaignReady, gCampaignResult;
EWRAM_DATA u32 gCampaignStage, gCampaignWins, gCampaignCredits, gCampaignLoaded;
EWRAM_DATA u32 gCampaignCaseType, gCampaignAudit[11];
static u16 sCampaignStable;
static void CampaignIdle(void) {}
static void CampaignBalancedTeam(void)
{
    static const u16 species[]={SPECIES_MEWTWO,SPECIES_RAYQUAZA,SPECIES_METAGROSS,SPECIES_KYOGRE,SPECIES_TYRANITAR,SPECIES_SNORLAX};
    static const u16 moves[][4]={
        {MOVE_PSYCHIC,MOVE_ICE_BEAM,MOVE_THUNDERBOLT,MOVE_SHADOW_BALL},
        {MOVE_DRAGON_CLAW,MOVE_AERIAL_ACE,MOVE_FLAMETHROWER,MOVE_ROCK_SLIDE},
        {MOVE_METEOR_MASH,MOVE_SHADOW_BALL,MOVE_BRICK_BREAK,MOVE_AERIAL_ACE},
        {MOVE_SURF,MOVE_ICE_BEAM,MOVE_THUNDERBOLT,MOVE_WATER_SPOUT},
        {MOVE_ROCK_SLIDE,MOVE_CRUNCH,MOVE_BRICK_BREAK,MOVE_AERIAL_ACE},
        {MOVE_BODY_SLAM,MOVE_SHADOW_BALL,MOVE_BRICK_BREAK,MOVE_STRENGTH}
    };
    u8 i,j,ev=252;
    u16 item=ITEM_LEFTOVERS;
    for(i=0;i<PARTY_SIZE;i++)
    {
        CreateMon(&gPlayerParty[i],species[i],100,31,TRUE,100+i,OT_ID_PLAYER_ID,0);
        for(j=0;j<4;j++)SetMonMoveSlot(&gPlayerParty[i],moves[i][j],j);
        SetMonData(&gPlayerParty[i],MON_DATA_HELD_ITEM,&item);
        SetMonData(&gPlayerParty[i],MON_DATA_HP_EV,&ev);
        SetMonData(&gPlayerParty[i],i==0||i==3?MON_DATA_SPATK_EV:MON_DATA_ATK_EV,&ev);
        CalculateMonStats(&gPlayerParty[i]);
    }
    HealPlayerParty();CalculatePlayerPartyCount();
}
void CampaignCreditsFinished(void)
{
    gCampaignCredits++;
    SetMainCallback1(NULL);
    SetMainCallback2(CampaignIdle);
}
void CampaignInit(void)
{
    u8 i;
    Nuzlocke_ChooseMode(GAME_MODE_NORMAL);
#ifdef CAMPAIGN_STORY_TEST
    VarSet(VAR_MAP_SCENE_PALLET_TOWN_PROFESSOR_OAKS_LAB,4);
#else
    EnableNationalPokedex();
    FlagSet(FLAG_SYS_POKEDEX_GET);
#endif
    FlagSet(FLAG_SYS_POKEMON_GET);
    gSaveBlock2Ptr->optionsTextSpeed = 2;
    gSaveBlock2Ptr->optionsBattleSceneOff = TRUE;
    for (i=0;i<PARTY_SIZE;i++)
    {
        CreateMon(&gPlayerParty[i],SPECIES_MEWTWO,100,31,TRUE,100+i,OT_ID_PLAYER_ID,0);
        SetMonMoveSlot(&gPlayerParty[i],MOVE_PSYCHIC,0);
        SetMonMoveSlot(&gPlayerParty[i],MOVE_ICE_BEAM,1);
        SetMonMoveSlot(&gPlayerParty[i],MOVE_THUNDERBOLT,2);
        SetMonMoveSlot(&gPlayerParty[i],MOVE_FLAMETHROWER,3);
    }
    CalculatePlayerPartyCount();
    SetWarpDestination(MAP_GROUP(MAP_PEWTER_CITY_GYM),MAP_NUM(MAP_PEWTER_CITY_GYM),-1,6,6);
    WarpIntoMap();
}
static void CampaignBoot(void)
{
    static const u8 name[] = _("TEST");
    SetSaveBlocksPointers();
    if (LoadGameSave(SAVE_NORMAL)==SAVE_STATUS_OK)
    {
        gCampaignLoaded=1;
#ifdef CAMPAIGN_RESEED_TEAM
        CampaignBalancedTeam();
#endif
        SetMainCallback2(CB2_ContinueSavedGame);
    }
    else
    {
        StringCopy(gSaveBlock2Ptr->playerName,name);
        StringCopy(gSaveBlock1Ptr->rivalName,name);
        SetMainCallback2(CB2_NewGame);
    }
}
static void CampaignTick(void)
{
    const struct CampaignCase *step=&sCampaignCases[gCampaignCase];
    bool8 field = CampaignIsField() && !gPaletteFade.active
        && !ScriptContext_IsEnabled() && !ArePlayerFieldControlsLocked();
    u8 i;
    gCampaignCaseType=step->credits;
    if (gSaveBlock1Ptr!=NULL && gSaveBlock2Ptr!=NULL)
    {
        gCampaignAudit[0]=0;
        for(i=0;i<8;i++)if(FlagGet(FLAG_BADGE01_GET+i))gCampaignAudit[0]|=1<<i;
        gCampaignAudit[1]=FlagGet(FLAG_SYS_GAME_CLEAR);
        gCampaignAudit[2]=IsNationalPokedexEnabled();
        gCampaignAudit[3]=FlagGet(FLAG_SYS_CAN_LINK_WITH_RS);
        gCampaignAudit[4]=FlagGet(FLAG_HIDE_CERULEAN_CAVE_GUARD);
        gCampaignAudit[5]=FlagGet(FLAG_SYS_UNLOCKED_TANOBY_RUINS);
        gCampaignAudit[6]=VarGet(VAR_MAP_SCENE_ONE_ISLAND_POKEMON_CENTER_1F);
        gCampaignAudit[7]=GetGameStat(GAME_STAT_ENTERED_HOF);
        gCampaignAudit[8]=gSaveBlock1Ptr->towerChallengeId;
        if(gSaveBlock1Ptr->towerChallengeId<NUM_TOWER_CHALLENGE_TYPES)
        {
            gCampaignAudit[9]=gSaveBlock1Ptr->trainerTower[gSaveBlock1Ptr->towerChallengeId].floorsCleared;
            gCampaignAudit[10]=gSaveBlock1Ptr->trainerTower[gSaveBlock1Ptr->towerChallengeId].receivedPrize;
        }
    }
    gCampaignReady=field;
    if (gCampaignCommand==1 && field)
    {
        gCampaignCommand=0;gCampaignResult=0;gCampaignStage=1;sCampaignStable=0;
        HealPlayerParty();
        if(!step->reuseMap || gSaveBlock1Ptr->location.mapGroup!=MAP_GROUP(step->map) || gSaveBlock1Ptr->location.mapNum!=MAP_NUM(step->map))
        {
            SetWarpDestination(MAP_GROUP(step->map),MAP_NUM(step->map),-1,step->x,step->y);
            WarpIntoMap();
            gFieldCallback=FieldCB_WarpExitFadeFromBlack;
            SetMainCallback2(CB2_LoadMap);
        }
        return;
    }
    if (gCampaignStage==1)
    {
        if (field && ++sCampaignStable>30) {gCampaignResult=1;gCampaignStage=0;}
        if (!field) sCampaignStable=0;
    }
    if (gCampaignCommand==2 && field)
    {
        gCampaignCommand=0;gCampaignResult=0;gCampaignStage=2;sCampaignStable=0;
        gSpecialVar_LastTalked=step->localId;
        gSelectedObjectEvent=GetObjectEventIdByLocalIdAndMap(step->localId,MAP_NUM(step->map),MAP_GROUP(step->map));
        if(step->supply)AddBagItem(step->supply,1);
        if(step->towerStart)
        {
            CampaignBalancedTeam();
            // The real receptionist copies VAR_RESULT to 8005 before starting.
            gSpecialVar_Result=step->towerStart-1;
        }
        if (step->script!=NULL) ScriptContext_SetupScript(step->script);
    }
    if (gCampaignStage==2)
    {
        if (field && ++sCampaignStable>30)
        {
            gCampaignResult=(!step->flag||FlagGet(step->flag))
                &&(!step->var||VarGet(step->var)==step->value)
                &&(!step->item||CheckBagHasItem(step->item,1))
                &&(!step->trainer||HasTrainerBeenFought(step->trainer))
                &&(!step->towerExpected||gSaveBlock1Ptr->towerChallengeId==step->towerExpected-1)
                &&(!step->towerFloor||gSaveBlock1Ptr->trainerTower[gSaveBlock1Ptr->towerChallengeId].floorsCleared==step->towerFloor)
                &&(!step->towerPrize||(gSaveBlock1Ptr->trainerTower[gSaveBlock1Ptr->towerChallengeId].receivedPrize
                    &&gSaveBlock1Ptr->trainerTower[gSaveBlock1Ptr->towerChallengeId].checkedFinalTime))?1:2;
            gCampaignStage=0;
        }
        if (!field) sCampaignStable=0;
    }
    if (gCampaignCommand==3 && field)
    {
        gCampaignCommand=0;
        gCampaignResult=TrySavingData(SAVE_NORMAL)==SAVE_STATUS_OK?1:3;
    }
}
