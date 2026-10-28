/* Scene 4 resources and BG layout, rendered independently of the intro state machine. */
#include <nitro.h>
#include <nitro/demo/DEMO.h>
#include <nnsys/g2d/load/g2d_NCG_load.h>
#include <nnsys/g2d/load/g2d_NCL_load.h>
#include <nnsys/g2d/load/g2d_NSC_load.h>
#include <stdio.h>
#include <stdlib.h>

void SingleScreen_ShowSubEngine(BOOL show);
static void *load(int member) {
    char path[32]; snprintf(path, sizeof(path), "%d.bin", member);
    FILE *file = fopen(path, "rb");
    if (!file || fseek(file, 0, SEEK_END)) abort();
    long size = ftell(file);
    if (size < 40 || size > 1024 * 1024 || fseek(file, 0, SEEK_SET)) abort();
    void *data = malloc(size);
    if (!data || fread(data, 1, size, file) != (size_t)size) abort();
    fclose(file);
    return data; /* Files remain alive for the diagnostic process lifetime. */
}

void NitroMain(void) {
    DEMOInitCommon();
    DEMOInitVRAM();
    GX_SetBankForBG(GX_VRAM_BG_128_A);
    GX_SetBankForSubBG(GX_VRAM_SUB_BG_128_C);
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BG0_AS_2D);
    GXS_SetGraphicsMode(GX_BGMODE_0);
    G2_BlendNone(); G2S_BlendNone();
    G2_SetBG1Control(GX_BG_SCRSIZE_TEXT_256x256, GX_BG_COLORMODE_16,
                    GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x04000, GX_BG_EXTPLTT_01);
    G2S_SetBG1Control(GX_BG_SCRSIZE_TEXT_256x256, GX_BG_COLORMODE_16,
                     GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x04000, GX_BG_EXTPLTT_01);
    G2_SetBG2ControlText(GX_BG_SCRSIZE_TEXT_256x256, GX_BG_COLORMODE_16,
                        GX_BG_SCRBASE_0x0800, GX_BG_CHARBASE_0x04000);
    G2S_SetBG2ControlText(GX_BG_SCRSIZE_TEXT_256x256, GX_BG_COLORMODE_16,
                         GX_BG_SCRBASE_0x0800, GX_BG_CHARBASE_0x04000);
    G2_SetBG3ControlText(GX_BG_SCRSIZE_TEXT_256x256, GX_BG_COLORMODE_16,
                        GX_BG_SCRBASE_0x1000, GX_BG_CHARBASE_0x04000);
    G2S_SetBG3ControlText(GX_BG_SCRSIZE_TEXT_256x256, GX_BG_COLORMODE_16,
                         GX_BG_SCRBASE_0x1000, GX_BG_CHARBASE_0x04000);
    G2_SetBG1Priority(1); G2S_SetBG1Priority(1);
    G2_SetBG2Priority(2); G2S_SetBG2Priority(2);
    G2_SetBG3Priority(3); G2S_SetBG3Priority(3);
    NNSG2dCharacterData *chr;
    NNSG2dPaletteData *palette;
    if (!NNS_G2dGetUnpackedBGCharacterData(load(54), &chr) ||
        !NNS_G2dGetUnpackedPaletteData(load(53), &palette) || palette->szByte < 0x80) abort();
    GX_LoadBG1Char(chr->pRawData, 0, chr->szByte);
    GXS_LoadBG1Char(chr->pRawData, 0, chr->szByte);
    GX_LoadBGPltt(palette->pRawData, 0, 0x80);
    GXS_LoadBGPltt(palette->pRawData, 0, 0x80);
    const int members[] = {58, 55, 56, 57, 55, 56};
    void (*const upload[])(const void *, u32, u32) = {
        GX_LoadBG1Scr, GX_LoadBG2Scr, GX_LoadBG3Scr,
        GXS_LoadBG1Scr, GXS_LoadBG2Scr, GXS_LoadBG3Scr};
    for (unsigned i = 0; i < 6; ++i) {
        NNSG2dScreenData *screen;
        if (!NNS_G2dGetUnpackedScreenData(load(members[i]), &screen)) abort();
        upload[i](screen->rawData, 0, screen->szByte);
    }
    const char *selection = getenv("HG_BG_LAYER");
    int layer = selection ? atoi(selection) : 1;
    if (layer < 1 || layer > 3) abort();
    GX_SetVisiblePlane(1 << layer); GXS_SetVisiblePlane(1 << layer);
    SingleScreen_ShowSubEngine(FALSE);
    DEMOStartDisplay();
    for (unsigned frame = 1;; ++frame) {
        if (frame == 40) SingleScreen_ShowSubEngine(TRUE);
        if (frame == 80) SingleScreen_ShowSubEngine(FALSE);
        OS_WaitVBlankIntr();
    }
}
void VBlankIntr(void) { OS_SetIrqCheckFlag(OS_IE_V_BLANK); }
