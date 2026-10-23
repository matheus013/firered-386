#include "global.h"
#include "game_settings.h"
#include "pokemon.h"
#include "battle.h"
#include "event_data.h"
#include "pokedex.h"
#include "constants/species.h"
#include "constants/pokemon.h"
#include "constants/trainers.h"
#include "constants/moves.h"
#include "constants/battle.h"
extern const struct Evolution gEvolutionTable[][EVOS_PER_MON];

static u32 Mix(u32 value)
{
    value ^= value >> 16; value *= 0x7FEB352D;
    value ^= value >> 15; value *= 0x846CA68B;
    return value ^ (value >> 16);
}
static bool8 IsLegend(u16 species)
{
    switch(species)
    {
    case SPECIES_ARTICUNO: case SPECIES_ZAPDOS: case SPECIES_MOLTRES:
    case SPECIES_MEWTWO: case SPECIES_MEW: case SPECIES_RAIKOU:
    case SPECIES_ENTEI: case SPECIES_SUICUNE: case SPECIES_LUGIA:
    case SPECIES_HO_OH: case SPECIES_CELEBI: case SPECIES_REGIROCK:
    case SPECIES_REGICE: case SPECIES_REGISTEEL: case SPECIES_LATIAS:
    case SPECIES_LATIOS: case SPECIES_KYOGRE: case SPECIES_GROUDON:
    case SPECIES_RAYQUAZA: case SPECIES_JIRACHI: case SPECIES_DEOXYS:
        return TRUE;
    }
    return FALSE;
}
static u16 TotalStats(u16 species)
{
    const struct SpeciesInfo *info=&gSpeciesInfo[species];
    return info->baseHP+info->baseAttack+info->baseDefense+info->baseSpeed+info->baseSpAttack+info->baseSpDefense;
}
static bool8 IsSpecies(u16 species)
{
    return species>SPECIES_NONE && species<SPECIES_EGG && SpeciesToNationalPokedexNum(species)>0
        && SpeciesToNationalPokedexNum(species)<=386;
}
static u8 Bucket(u16 species)
{
    u16 bst=TotalStats(species);
    if(IsLegend(species))return 4;
    return bst<=350?0:bst<=450?1:bst<=550?2:3;
}
static u16 GreatestCommonDivisor(u16 a,u16 b)
{
    while(b){u16 next=a%b;a=b;b=next;}
    return a;
}
// A permutation inside each power band, computed without an EWRAM lookup cache.
u16 Randomizer_Species(u16 species)
{
    u16 candidate,count=0,index=0,step,target;
    u8 bucket;
    u32 seed;
    if(!GameSettings_IsRandomizer() || !IsSpecies(species))return species;
    bucket=Bucket(species);
    for(candidate=1;candidate<SPECIES_EGG;candidate++)
        if(IsSpecies(candidate) && Bucket(candidate)==bucket)
        {
            if(candidate==species)index=count;
            count++;
        }
    if(count<2)return species;
    seed=Mix(gSaveBlock2Ptr->challenge.randomizerSeed ^ (0x524E4400+bucket));
    step=(seed>>16)%count;
    while(GreatestCommonDivisor(step,count)!=1)step=(step+1)%count;
    target=(index*step+seed%count)%count;
    for(candidate=1;candidate<SPECIES_EGG;candidate++)
        if(IsSpecies(candidate) && Bucket(candidate)==bucket)
        {
            if(target==0)return candidate;
            target--;
        }
    return species;
}
static bool8 HasAttack(u16 species,u8 level)
{
    u16 i,move;u32 entry;
    for(i=0;(entry=gLevelUpLearnsets[species][i])!=LEVEL_UP_END;i++)
    {
        if((entry>>LEVEL_UP_LV_SHIFT)>level)break;
        move=entry&LEVEL_UP_MOVE_ID;
        if(gBattleMoves[move].power>0)return TRUE;
    }
    return FALSE;
}
static bool8 IsStarter(u16 species)
{
    return IsSpecies(species) && !IsLegend(species) && TotalStats(species)>=250
        && TotalStats(species)<=350 && HasAttack(species,5);
}
u16 Randomizer_Starter(u8 slot)
{
    static const u16 original[]={SPECIES_BULBASAUR,SPECIES_SQUIRTLE,SPECIES_CHARMANDER};
    u16 species,count=0,step,target;
    u32 seed;
    if(slot>2)return SPECIES_NONE;
    if(!GameSettings_IsRandomizer())return original[slot];
    for(species=1;species<SPECIES_EGG;species++)if(IsStarter(species))count++;
    seed=Mix(gSaveBlock2Ptr->challenge.randomizerSeed ^ 0x53544152);
    step=(seed>>16)%count;
    while(GreatestCommonDivisor(step,count)!=1)step=(step+1)%count;
    target=(slot*step+seed%count)%count;
    for(species=1;species<SPECIES_EGG;species++)if(IsStarter(species))
    {
        if(target==0)return species;
        target--;
    }
    return SPECIES_BULBASAUR;
}
void Randomizer_PrepareStarter(void)
{
    u8 slot=VarGet(VAR_TEMP_1);
    gSpecialVar_Result=GameSettings_IsRandomizer();
    if(!GameSettings_IsRandomizer() || slot>2)return;
    VarSet(VAR_TEMP_2,Randomizer_Starter(slot));
    VarSet(VAR_TEMP_3,Randomizer_Starter((slot+2)%3));
}
static u16 StarterEvolution(u16 species,u8 level)
{
    u8 depth,i;
    for(depth=0;depth<3;depth++)
    {
        u16 next=species;
        for(i=0;i<EVOS_PER_MON;i++)
            if(gEvolutionTable[species][i].method==EVO_LEVEL && gEvolutionTable[species][i].param<=level)
            {next=gEvolutionTable[species][i].targetSpecies;break;}
        if(next==species)break;
        species=next;
    }
    return species;
}
static u16 RivalStarter(u16 species,u8 level)
{
    u8 slot;
    switch(species)
    {
    case SPECIES_BULBASAUR:case SPECIES_IVYSAUR:case SPECIES_VENUSAUR:slot=0;break;
    case SPECIES_SQUIRTLE:case SPECIES_WARTORTLE:case SPECIES_BLASTOISE:slot=1;break;
    case SPECIES_CHARMANDER:case SPECIES_CHARMELEON:case SPECIES_CHARIZARD:slot=2;break;
    default:return SPECIES_NONE;
    }
    return StarterEvolution(Randomizer_Starter(slot),level);
}
static void SelectMoves(struct Pokemon *mon,u16 species,u8 level)
{
    u16 selected[MAX_MON_MOVES]={MOVE_NONE,MOVE_NONE,MOVE_NONE,MOVE_NONE};
    u16 i,move,best,score,bestScore;u32 entry;
    u8 slot,j;
    for(slot=0;slot<MAX_MON_MOVES;slot++)
    {
        best=MOVE_NONE;bestScore=0;
        for(i=0;(entry=gLevelUpLearnsets[species][i])!=LEVEL_UP_END;i++)
        {
            if((entry>>LEVEL_UP_LV_SHIFT)>level)break;
            move=entry&LEVEL_UP_MOVE_ID;
            if(gBattleMoves[move].target==MOVE_TARGET_FOES_AND_ALLY)continue;
            for(j=0;j<slot;j++)if(selected[j]==move)break;
            if(j!=slot)continue;
            score=gBattleMoves[move].power;
            if(score && (gBattleMoves[move].type==gSpeciesInfo[species].types[0] || gBattleMoves[move].type==gSpeciesInfo[species].types[1]))score=score*3/2;
            if(score && slot<2)score+=200;
            if(!score && slot<2)score=1;
            if(slot>=2 && !gBattleMoves[move].power)score=30;
            if(score>bestScore){best=move;bestScore=score;}
        }
        selected[slot]=best;
        SetMonMoveSlot(mon,best,slot);
    }
}
void Randomizer_TrainerMon(struct Pokemon *mon,u16 trainer,u8 slot)
{
    u16 original,species,item;
    u8 level,iv,trainerClass;
    u32 personality;
    if(!GameSettings_IsRandomizer())return;
    original=GetMonData(mon,MON_DATA_SPECIES);
    level=GetMonData(mon,MON_DATA_LEVEL);
    species=Randomizer_Species(original);
    trainerClass=gTrainers[trainer].trainerClass;
    if(trainerClass==TRAINER_CLASS_RIVAL_EARLY || trainerClass==TRAINER_CLASS_RIVAL_LATE || trainerClass==TRAINER_CLASS_CHAMPION)
    {
        u16 starter=RivalStarter(original,level);
        if(starter!=SPECIES_NONE)species=starter;
    }
    // Avoid a trainer with only non-damaging early-level moves.
    if(!HasAttack(species,level))
    {
        u16 candidate,start=Mix(gSaveBlock2Ptr->challenge.randomizerSeed ^ trainer ^ (slot<<16))%(SPECIES_EGG-1)+1;
        for(candidate=0;candidate<SPECIES_EGG-1;candidate++)
        {
            u16 next=(start+candidate-1)%(SPECIES_EGG-1)+1;
            if(IsSpecies(next) && Bucket(next)==Bucket(original) && HasAttack(next,level)){species=next;break;}
        }
    }
    personality=GetMonData(mon,MON_DATA_PERSONALITY);
    iv=GetMonData(mon,MON_DATA_HP_IV);
    item=GetMonData(mon,MON_DATA_HELD_ITEM);
    CreateMon(mon,species,level,iv,TRUE,personality,OT_ID_RANDOM_NO_SHINY,0);
    SetMonData(mon,MON_DATA_HELD_ITEM,&item);
    SelectMoves(mon,species,level);
}
