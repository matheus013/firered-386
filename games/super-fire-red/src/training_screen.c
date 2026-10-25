#include "global.h"
#include "game_settings.h"
#include "trainer_pokemon_sprites.h"
#include "gflib.h"
#include "training.h"
#include "pokemon.h"
#include "pokemon_summary_screen.h"
#include "menu.h"
#include "new_menu_helpers.h"
#include "overworld.h"
#include "field_fadetransition.h"
#include "task.h"
#include "malloc.h"
#include "string_util.h"
#include "event_data.h"
#include "script.h"
#include "constants/pokemon.h"

enum { PAGE_PARTY, PAGE_MENU, PAGE_NATURE, PAGE_EVS, PAGE_IVS, PAGE_CONFIRM };
struct TrainingScreen
{
    u8 page, slot, cursor, nature, editing;
    u8 values[NUM_STATS];
    u16 spriteId;
    u8 spriteSlot;
    bool8 applied;
    bool8 unavailable;
};
static EWRAM_DATA struct TrainingScreen *sTraining = NULL;
static void CB2_Training(void);
enum { UI_TITLE, UI_PARTY, UI_NATURE, UI_EVS, UI_IVS, UI_BACK, UI_SELECT, UI_EDIT, UI_ADJUST, UI_REVIEW, UI_CONFIRM, UI_APPLY, UI_NOHEAL, UI_APPLIED, UI_NEUTRAL, UI_OLDNEW, UI_FREE, UI_UNAVAILABLE, UI_NATUREHELP, UI_MAXEV, UI_MAXIV, UI_PREVIEW, UI_LEVEL, UI_HP, UI_ATTACK, UI_DEFENSE, UI_SPEED, UI_SPATK, UI_SPDEF };
static const u8 sUi0En[] = _("POKEMON TRAINING");
static const u8 sUi0Pt[] = _("CENTRO DE TREINO");
static const u8 sUi1En[] = _("SELECT A POKEMON");
static const u8 sUi1Pt[] = _("ESCOLHA O POKéMON");
static const u8 sUi2En[] = _("Nature");
static const u8 sUi2Pt[] = _("Natureza");
static const u8 sUi3En[] = _("EV training");
static const u8 sUi3Pt[] = _("Treinar EVs");
static const u8 sUi4En[] = _("IV training");
static const u8 sUi4Pt[] = _("Treinar IVs");
static const u8 sUi5En[] = _("Back");
static const u8 sUi5Pt[] = _("Voltar");
static const u8 sUi6En[] = _("A: SELECT     B: EXIT");
static const u8 sUi6Pt[] = _("A: ESCOLHER     B: SAIR");
static const u8 sUi7En[] = _("A: EDIT     B: PARTY");
static const u8 sUi7Pt[] = _("A: EDITAR     B: EQUIPE");
static const u8 sUi8En[] = _("LEFT/RIGHT: ADJUST  L:0  R:MAX");
static const u8 sUi8Pt[] = _("ESQ/DIR: AJUSTAR  L:0  R:MAX");
static const u8 sUi9En[] = _("A: REVIEW     B: CANCEL");
static const u8 sUi9Pt[] = _("A: REVISAR     B: CANCELAR");
static const u8 sUi10En[] = _("CONFIRM CHANGES");
static const u8 sUi10Pt[] = _("CONFIRMAR AJUSTES");
static const u8 sUi11En[] = _("A: APPLY     B: BACK");
static const u8 sUi11Pt[] = _("A: APLICAR     B: VOLTAR");
static const u8 sUi12En[] = _("Free. Does not heal or revive.");
static const u8 sUi12Pt[] = _("Grátis. Sem curar ou reviver.");
static const u8 sUi13En[] = _("CHANGES SAVED");
static const u8 sUi13Pt[] = _("AJUSTE APLICADO");
static const u8 sUi14En[] = _("Neutral nature");
static const u8 sUi14Pt[] = _("Natureza neutra");
static const u8 sUi15En[] = _("CURRENT     NEW");
static const u8 sUi15Pt[] = _("ATUAL       NOVO");
static const u8 sUi16En[] = _("EVs LEFT");
static const u8 sUi16Pt[] = _("EVs LIVRES");
static const u8 sUi17En[] = _("Unavailable for training.");
static const u8 sUi17Pt[] = _("Indisponível para treinar.");
static const u8 sUi18En[] = _("UP/DOWN: CHANGE NATURE");
static const u8 sUi18Pt[] = _("CIMA/BAIXO: MUDAR NATUREZA");
static const u8 sUi19En[] = _("EVs: 252/stat - 510 total");
static const u8 sUi19Pt[] = _("EVs: 252/atributo - 510 total");
static const u8 sUi20En[] = _("IVs: 0-31 per stat");
static const u8 sUi20Pt[] = _("IVs: 0-31 por atributo");
static const u8 sUi21En[] = _("STAT PREVIEW");
static const u8 sUi21Pt[] = _("ATRIBUTO");
static const u8 sUi22En[] = _("Lv.");
static const u8 sUi22Pt[] = _("Nv.");
static const u8 sUi23En[] = _("HP");
static const u8 sUi23Pt[] = _("HP");
static const u8 sUi24En[] = _("ATK");
static const u8 sUi24Pt[] = _("ATQ");
static const u8 sUi25En[] = _("DEF");
static const u8 sUi25Pt[] = _("DEF");
static const u8 sUi26En[] = _("SPEED");
static const u8 sUi26Pt[] = _("VEL");
static const u8 sUi27En[] = _("SP.ATK");
static const u8 sUi27Pt[] = _("ATQ ESP");
static const u8 sUi28En[] = _("SP.DEF");
static const u8 sUi28Pt[] = _("DEF ESP");
static const u8 sAdvancedLockedEn[] = _("Nature / IVs: after the League.");
static const u8 sAdvancedLockedPt[] = _("Natureza / IVs: após a Liga.");
static const u8 *const sUi[][2] = {
    {sUi0En, sUi0Pt},
    {sUi1En, sUi1Pt},
    {sUi2En, sUi2Pt},
    {sUi3En, sUi3Pt},
    {sUi4En, sUi4Pt},
    {sUi5En, sUi5Pt},
    {sUi6En, sUi6Pt},
    {sUi7En, sUi7Pt},
    {sUi8En, sUi8Pt},
    {sUi9En, sUi9Pt},
    {sUi10En, sUi10Pt},
    {sUi11En, sUi11Pt},
    {sUi12En, sUi12Pt},
    {sUi13En, sUi13Pt},
    {sUi14En, sUi14Pt},
    {sUi15En, sUi15Pt},
    {sUi16En, sUi16Pt},
    {sUi17En, sUi17Pt},
    {sUi18En, sUi18Pt},
    {sUi19En, sUi19Pt},
    {sUi20En, sUi20Pt},
    {sUi21En, sUi21Pt},
    {sUi22En, sUi22Pt},
    {sUi23En, sUi23Pt},
    {sUi24En, sUi24Pt},
    {sUi25En, sUi25Pt},
    {sUi26En, sUi26Pt},
    {sUi27En, sUi27Pt},
    {sUi28En, sUi28Pt},
};
static const u8 *T(u8 id) { return sUi[id][GameSettings_GetLanguage()]; }

static const u16 sTrainingPalette[] = {
    RGB(2,4,8), RGB(3,6,12), RGB(30,31,31), RGB(1,3,7),
    RGB(8,13,20), RGB(7,26,26), RGB(5,14,22), RGB(31,23,9),
    RGB(13,27,17), RGB(31,12,14), RGB(17,21,26), RGB(4,8,15),
    RGB(10,19,26), RGB(25,29,31), RGB(8,10,16), RGB(0,0,0)
};
static const u8 sStatIds[] = {UI_HP,UI_ATTACK,UI_DEFENSE,UI_SPEED,UI_SPATK,UI_SPDEF};

static void PrintAt(u8 x,u8 y,u8 background,u8 foreground,const u8 *text)
{
    u8 colors[3];
    colors[0]=background;colors[1]=foreground;colors[2]=background;
    AddTextPrinterParameterized4(0,FONT_SMALL,x,y,0,0,colors,0,text);
}
static void Panel(u8 x,u8 y,u8 width,u8 height,u8 fill,u8 edge)
{
    FillWindowPixelRect(0,edge,x,y,width,height);
    FillWindowPixelRect(0,fill,x+1,y+1,width-2,height-2);
}
static void NumberAt(u8 x,u8 y,u16 value,u8 background,u8 foreground)
{
    u8 buffer[12];
    ConvertIntToDecimalStringN(buffer,value,STR_CONV_MODE_RIGHT_ALIGN,3);
    PrintAt(x,y,background,foreground,buffer);
}
static void UpdatePortrait(void)
{
    struct Pokemon *mon=&gPlayerParty[sTraining->slot];
    if(sTraining->spriteId<MAX_SPRITES && sTraining->spriteSlot==sTraining->slot)return;
    if(sTraining->spriteId<MAX_SPRITES)FreeAndDestroyMonPicSprite(sTraining->spriteId);
    sTraining->spriteId=CreateMonPicSprite_HandleDeoxys(GetMonData(mon,MON_DATA_SPECIES_OR_EGG),GetMonData(mon,MON_DATA_OT_ID),GetMonData(mon,MON_DATA_PERSONALITY),TRUE,48,79,12,0xFFFF);
    sTraining->spriteSlot=sTraining->slot;
}
static u16 PreviewStat(u8 stat)
{
    struct Pokemon preview=gPlayerParty[sTraining->slot];
    u8 i;
    u16 nature=0xA500|sTraining->nature;
    if(sTraining->page==PAGE_NATURE || (sTraining->page==PAGE_CONFIRM && sTraining->editing==PAGE_NATURE))
        SetMonData(&preview,MON_DATA_TRAINING_NATURE,&nature);
    else for(i=0;i<NUM_STATS;i++)SetMonData(&preview,((sTraining->page==PAGE_EVS || (sTraining->page==PAGE_CONFIRM && sTraining->editing==PAGE_EVS))?MON_DATA_HP_EV:MON_DATA_HP_IV)+i,&sTraining->values[i]);
    CalculateMonStats(&preview);
    return GetMonData(&preview,MON_DATA_MAX_HP+stat);
}
static void DrawTraining(void)
{
    u8 i,background,buffer[80],*end,kind;
    u16 total=0,old,maximum;
    struct Pokemon *mon=&gPlayerParty[sTraining->slot];
    FillWindowPixelBuffer(0,PIXEL_FILL(0));
    Panel(0,0,224,19,1,4);
    FillWindowPixelRect(0,5,0,18,224,2);
    PrintAt(7,2,1,2,T(UI_TITLE));
    Panel(0,24,79,103,11,4);
    Panel(85,24,139,103,11,4);
    GetMonData(mon,MON_DATA_NICKNAME,buffer);PrintAt(5,27,11,2,buffer);
    UpdatePortrait();
    PrintAt(6,108,11,10,T(UI_LEVEL));NumberAt(28,108,GetMonData(mon,MON_DATA_LEVEL),11,2);
    for(i=0;i<NUM_STATS;i++)total+=GetMonData(mon,MON_DATA_HP_EV+i);
    FillWindowPixelRect(0,4,6,124,66,2);
    FillWindowPixelRect(0,5,6,124,66*total/510,2);
    if(sTraining->page==PAGE_PARTY)
    {
        for(i=0;i<gPlayerPartyCount;i++)
        {
            background=i==sTraining->slot?6:11;
            Panel(89,28+15*i,131,14,background,i==sTraining->slot?5:11);
            GetMonData(&gPlayerParty[i],MON_DATA_NICKNAME,buffer);PrintAt(94,29+15*i,background,2,buffer);
            NumberAt(195,29+15*i,GetMonData(&gPlayerParty[i],MON_DATA_LEVEL),background,10);
        }
        PrintAt(4,133,0,sTraining->unavailable?9:10,sTraining->unavailable?T(UI_UNAVAILABLE):T(UI_SELECT));
    }
    else if(sTraining->page==PAGE_MENU)
    {
        for(i=0;i<4;i++)
        {
            background=i==sTraining->cursor?6:11;
            Panel(91,31+22*i,127,18,background,i==sTraining->cursor?5:4);
            PrintAt(98,33+22*i,background,(i==0 || i==2) && !Training_AreAdvancedEditsUnlocked()?10:i==3?10:2,T(UI_NATURE+i));
        }
        PrintAt(4,128,0,sTraining->applied?8:10,sTraining->applied?T(UI_APPLIED):!Training_AreAdvancedEditsUnlocked()?(GameSettings_GetLanguage()==GAME_LANGUAGE_PTBR?sAdvancedLockedPt:sAdvancedLockedEn):T(UI_MAXEV));
        PrintAt(4,140,0,2,T(UI_EDIT));
    }
    else if(sTraining->page==PAGE_NATURE || (sTraining->page==PAGE_CONFIRM && sTraining->editing==PAGE_NATURE))
    {
        PrintAt(93,29,11,5,gNatureNamePointers[sTraining->nature]);
        for(i=0;i<NUM_STATS;i++)
        {
            PrintAt(92,45+i*12,11,10,T(sStatIds[i]));
            old=GetMonData(mon,MON_DATA_MAX_HP+i);
            NumberAt(147,45+i*12,old,11,10);
            // Color by the selected nature's own effect, not by the change from the current
            // nature, so undoing the current nature does not look like part of the new one.
            NumberAt(192,45+i*12,PreviewStat(i),11,GetNatureStatEffect(sTraining->nature,i)>0?8:GetNatureStatEffect(sTraining->nature,i)<0?9:2);
        }
        PrintAt(4,128,0,10,sTraining->page==PAGE_CONFIRM?T(UI_NOHEAL):T(UI_NATUREHELP));
        PrintAt(4,140,0,2,sTraining->page==PAGE_CONFIRM?T(UI_APPLY):T(UI_REVIEW));
    }
    else
    {
        kind=sTraining->page==PAGE_CONFIRM?sTraining->editing:sTraining->page;
        maximum=kind==PAGE_EVS?252:31;
        PrintAt(93,26,11,sTraining->page==PAGE_CONFIRM?7:10,sTraining->page==PAGE_CONFIRM?T(UI_CONFIRM):T(UI_OLDNEW));
        total=0;
        for(i=0;i<NUM_STATS;i++)
        {
            background=sTraining->page!=PAGE_CONFIRM && i==sTraining->cursor?6:11;
            FillWindowPixelRect(0,background,90,40+i*14,130,14);
            PrintAt(93,39+i*14,background,2,T(sStatIds[i]));
            old=GetMonData(mon,(kind==PAGE_EVS?MON_DATA_HP_EV:MON_DATA_HP_IV)+i);
            NumberAt(148,39+i*14,old,background,10);
            NumberAt(195,39+i*14,sTraining->values[i],background,old!=sTraining->values[i]?5:2);
            FillWindowPixelRect(0,4,93,51+i*14,122,2);
            FillWindowPixelRect(0,i==sTraining->cursor?5:12,93,51+i*14,122*sTraining->values[i]/maximum,2);
            total+=sTraining->values[i];
        }
        FillWindowPixelRect(0,11,4,107,70,18);
        PrintAt(6,107,11,10,kind==PAGE_EVS?T(UI_FREE):T(UI_PREVIEW));
        NumberAt(25,117,kind==PAGE_EVS?510-total:PreviewStat(sTraining->cursor),11,5);
        PrintAt(4,128,0,10,sTraining->page==PAGE_CONFIRM?T(UI_NOHEAL):T(UI_ADJUST));
        PrintAt(4,140,0,2,sTraining->page==PAGE_CONFIRM?T(UI_APPLY):T(UI_REVIEW));
    }
    PutWindowTilemap(0);CopyWindowToVram(0,COPYWIN_FULL);
}
static void VBlankTraining(void)
{
    LoadOam();ProcessSpriteCopyRequests();TransferPlttBuffer();
}

static void ExitTraining(void)
{
    SetVBlankCallback(NULL);
    if(sTraining->spriteId<MAX_SPRITES)FreeAndDestroyMonPicSprite(sTraining->spriteId);
    FreeAllSpritePalettes();
    Free(sTraining);
    sTraining = NULL;
    FreeAllWindowBuffers();
    SetMainCallback2(CB2_ReturnToFieldContinueScriptPlayMapMusic);
}

static void CB2_Training(void)
{
    static const struct BgTemplate bg = {.bg = 0, .charBaseIndex = 0, .mapBaseIndex = 31};
    static const struct WindowTemplate windows[] = {
        {.bg = 0, .tilemapLeft = 1, .tilemapTop = 1, .width = 28, .height = 19, .paletteNum = 15, .baseBlock = 1},
        DUMMY_WIN_TEMPLATE
    };
    u8 i;
    s16 value, maximum;
    u16 total;
    if (gMain.state == 0)
    {
        sTraining = AllocZeroed(sizeof(*sTraining));
        if (sTraining == NULL)
        {
            SetMainCallback2(CB2_ReturnToFieldContinueScriptPlayMapMusic);
            return;
        }
        sTraining->spriteId=0xFFFF;
        SetVBlankCallback(NULL);
        SetHBlankCallback(NULL);
        SetGpuReg(REG_OFFSET_DISPCNT, 0);
        SetGpuReg(REG_OFFSET_BLDCNT, 0);
        SetGpuReg(REG_OFFSET_BLDALPHA, 0);
        SetGpuReg(REG_OFFSET_BLDY, 0);
        ResetTasks();
        ResetSpriteData();
        ResetPaletteFade();
        DmaFill16(3, 0, VRAM, VRAM_SIZE);
        ResetBgsAndClearDma3BusyFlags(0);
        InitBgsFromTemplates(0, &bg, 1);
        InitWindows(windows);
        LoadPalette(sTrainingPalette,BG_PLTT_ID(15),sizeof(sTrainingPalette));
        LoadPalette(sTrainingPalette,0,2);
        FreeAllSpritePalettes();
        DeactivateAllTextPrinters();
        DrawTraining();
        TransferPlttBuffer();
        ShowBg(0);
        SetVBlankCallback(VBlankTraining);
        SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_MODE_0 | DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP | DISPCNT_OBJ_ON);
        gMain.state = 1;
        return;
    }
    AnimateSprites();
    BuildOamBuffer();
    if (gMain.newKeys == 0)
        return;
    if (sTraining->page == PAGE_PARTY)
    {
        if (JOY_NEW(B_BUTTON)) { ExitTraining(); return; }
        if (JOY_NEW(DPAD_UP) && sTraining->slot) sTraining->slot--;
        if (JOY_NEW(DPAD_DOWN) && sTraining->slot + 1 < gPlayerPartyCount) sTraining->slot++;
        if (JOY_NEW(A_BUTTON))
        {
            sTraining->unavailable = !Training_CanEdit(&gPlayerParty[sTraining->slot]);
            if (!sTraining->unavailable)
            {
                sTraining->page = PAGE_MENU;
                sTraining->cursor = 0;
            }
        }
    }
    else if (sTraining->page == PAGE_MENU)
    {
        if (JOY_NEW(B_BUTTON)) sTraining->page = PAGE_PARTY;
        if (JOY_NEW(DPAD_UP)) sTraining->cursor = (sTraining->cursor + 3) % 4;
        if (JOY_NEW(DPAD_DOWN)) sTraining->cursor = (sTraining->cursor + 1) % 4;
        if (JOY_NEW(A_BUTTON))
        {
            if (sTraining->cursor == 3) sTraining->page = PAGE_PARTY;
            else if (sTraining->cursor == 1 || Training_AreAdvancedEditsUnlocked())
            {
                sTraining->page = PAGE_NATURE + sTraining->cursor;
                sTraining->nature = GetNature(&gPlayerParty[sTraining->slot]);
                for (i = 0; i < NUM_STATS; i++)
                    sTraining->values[i] = GetMonData(&gPlayerParty[sTraining->slot], (sTraining->page == PAGE_EVS ? MON_DATA_HP_EV : MON_DATA_HP_IV) + i);
                sTraining->cursor = 0;
            }
        }
    }
    else if (sTraining->page == PAGE_CONFIRM)
    {
        if (JOY_NEW(B_BUTTON)) sTraining->page = sTraining->editing;
        if (JOY_NEW(A_BUTTON))
        {
            if (sTraining->editing == PAGE_NATURE) sTraining->applied = Training_SetNature(&gPlayerParty[sTraining->slot], sTraining->nature);
            else if (sTraining->editing == PAGE_EVS) sTraining->applied = Training_SetEVs(&gPlayerParty[sTraining->slot], sTraining->values);
            else sTraining->applied = Training_SetIVs(&gPlayerParty[sTraining->slot], sTraining->values);
            sTraining->page = PAGE_MENU;
            sTraining->cursor = 0;
        }
    }
    else
    {
        if (JOY_NEW(B_BUTTON)) { sTraining->page = PAGE_MENU; sTraining->cursor = 0; }
        else if (JOY_NEW(A_BUTTON)) { sTraining->editing = sTraining->page; sTraining->page = PAGE_CONFIRM; }
        else if (sTraining->page == PAGE_NATURE)
        {
            if (JOY_NEW(DPAD_UP)) sTraining->nature = (sTraining->nature + 24) % 25;
            if (JOY_NEW(DPAD_DOWN)) sTraining->nature = (sTraining->nature + 1) % 25;
        }
        else
        {
            if (JOY_NEW(DPAD_UP)) sTraining->cursor = (sTraining->cursor + 5) % 6;
            if (JOY_NEW(DPAD_DOWN)) sTraining->cursor = (sTraining->cursor + 1) % 6;
            maximum = sTraining->page == PAGE_EVS ? 252 : 31;
            total = 0;
            for (i = 0; i < NUM_STATS; i++) if (i != sTraining->cursor) total += sTraining->values[i];
            if (sTraining->page == PAGE_EVS && maximum > 510 - total) maximum = 510 - total;
            if (maximum < 0) maximum = 0;
            value = sTraining->values[sTraining->cursor];
            if (JOY_NEW(DPAD_LEFT)) value -= sTraining->page == PAGE_EVS ? 4 : 1;
            if (JOY_NEW(DPAD_RIGHT)) value += sTraining->page == PAGE_EVS ? 4 : 1;
            if (JOY_NEW(L_BUTTON)) value = 0;
            if (JOY_NEW(R_BUTTON)) value = maximum;
            if (value < 0) value = 0;
            if (value > maximum) value = maximum;
            sTraining->values[sTraining->cursor] = value;
        }
    }
    DrawTraining();
}

static void Task_StartTraining(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        DestroyTask(taskId);
        SetMainCallback2(CB2_Training);
    }
}

void OpenTrainingMenu(void)
{
    if (!Training_IsUnlocked())
    {
        ScriptContext_Enable();
        return;
    }
    CreateTask(Task_StartTraining, 10);
    BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
}
