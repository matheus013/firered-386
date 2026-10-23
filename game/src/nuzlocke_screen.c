#include "global.h"
#include "gflib.h"
#include "nuzlocke.h"
#include "menu.h"
#include "new_menu_helpers.h"
#include "main_menu.h"
#include "task.h"

void CB2_NuzlockeGameOver(void)
{
    static const struct BgTemplate bg = {.bg = 0, .charBaseIndex = 0, .mapBaseIndex = 31};
    static const struct WindowTemplate windows[] = {
        {.bg = 0, .tilemapLeft = 2, .tilemapTop = 2, .width = 26, .height = 16, .paletteNum = 15, .baseBlock = 1},
        DUMMY_WIN_TEMPLATE
    };
    static const u8 colors[] = {1, 2, 3};
    static const u8 text[] = _("HARDCORE NUZLOCKE\n\nCHALLENGE ENDED\nYour team was defeated.\nStart a new game.\n\nA: main menu");
    if (gMain.state == 0)
    {
        gSaveBlock2Ptr->challenge.failed = TRUE;
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
        Menu_LoadStdPalAt(BG_PLTT_ID(15));
        DeactivateAllTextPrinters();
        FillWindowPixelBuffer(0, PIXEL_FILL(1));
        AddTextPrinterParameterized4(0, FONT_NORMAL, 2, 2, 0, 0, colors, 0, text);
        PutWindowTilemap(0);
        CopyWindowToVram(0, COPYWIN_FULL);
        TransferPlttBuffer();
        ShowBg(0);
        SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_MODE_0 | DISPCNT_BG0_ON);
        gMain.state = 1;
    }
    else if (JOY_NEW(A_BUTTON))
    {
        FreeAllWindowBuffers();
        SetMainCallback2(CB2_InitMainMenu);
    }
}
