/* Static cell rendering through the SDK animation, OAM conversion and GX upload. */
#include <nitro.h>
#include <nitro/demo/DEMO.h>
#include <nnsys/g2d/g2d_CellAnimation.h>
#include <nnsys/g2d/fmt/g2d_Oam_data.h>
#include <nnsys/g2d/load/g2d_NCG_load.h>
#include <nnsys/g2d/load/g2d_NCL_load.h>
#include <nnsys/g2d/load/g2d_NCE_load.h>
#include <nnsys/g2d/load/g2d_NAN_load.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void SingleScreen_ShowSubEngine(BOOL show);
/* The SDK exposes these tables as definition macros, without a compiled owner. */
NNS_G2D_DEFINE_NNSI_OBJSIZEWTBL;
NNS_G2D_DEFINE_NNSI_OBJSIZEHTBL;
static void *load(int member) {
    char path[32]; snprintf(path, sizeof(path), "%d.bin", member);
    FILE *file = fopen(path, "rb");
    if (!file || fseek(file, 0, SEEK_END)) abort();
    long size = ftell(file);
    if (size < 40 || size > 1024 * 1024 || fseek(file, 0, SEEK_SET)) abort();
    void *data = malloc(size);
    if (!data || fread(data, 1, size, file) != (size_t)size) abort();
    fclose(file);
    return data;
}

void NitroMain(void) {
    const char *choice = getenv("HG_SPRITE_CASE");
    if (!choice || strlen(choice) != 1 || *choice < '0' || *choice > '5') abort();
    unsigned selected = *choice - '0';
    const int palettes[] = {81,81,81,85,93,89};
    const int chars[] = {82,82,82,86,94,90};
    const int cells[] = {84,84,84,88,96,92};
    const int anims[] = {83,83,83,87,95,91};
    NNSG2dCharacterData *chr;
    NNSG2dPaletteData *palette;
    NNSG2dCellDataBank *bank;
    NNSG2dAnimBankData *animations;
    if (!NNS_G2dGetUnpackedCharacterData(load(chars[selected]), &chr) ||
        !NNS_G2dGetUnpackedPaletteData(load(palettes[selected]), &palette) ||
        !NNS_G2dGetUnpackedCellBank(load(cells[selected]), &bank) ||
        !NNS_G2dGetUnpackedAnimBank(load(anims[selected]), &animations)) abort();
    unsigned sequence = selected < 3 ? selected : 0;
    const NNSG2dAnimSequenceData *seq = NNS_G2dGetAnimSequenceByIdx(animations, sequence);
    if (!seq || !seq->numFrames) abort();
    NNSG2dCellAnimation animation;
    NNS_G2dInitCellAnimation(&animation, seq, bank);
    const NNSG2dCellData *cell = NNS_G2dGetCellAnimationCurrentCell(&animation);
    if (!cell || !cell->numOAMAttrs || cell->numOAMAttrs > 128) abort();
    GXOamAttr objects[128];
    memset(objects, 0, sizeof(objects));
    for (unsigned i = 0; i < 128; ++i) G2_SetOBJEffect(&objects[i], GX_OAM_EFFECT_NODISPLAY, 0);
    NNSG2dFVec2 origin = {128 * FX32_ONE, 96 * FX32_ONE};
    unsigned count = 0;
    if (!getenv("HG_SPRITE_DISABLE_DRAW"))
        count = NNS_G2dMakeCellToOams(objects, 128, cell, NULL, &origin, 0, FALSE);
    if (count && count != cell->numOAMAttrs) abort();

    DEMOInitCommon(); DEMOInitVRAM();
    GX_SetBankForOBJ(GX_VRAM_OBJ_128_A);
    GX_SetBankForSubOBJ(GX_VRAM_SUB_OBJ_128_D);
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BG0_AS_2D);
    GXS_SetGraphicsMode(GX_BGMODE_0);
    GX_SetOBJVRamModeChar(chr->mappingType); GXS_SetOBJVRamModeChar(chr->mappingType);
    GX_SetVisiblePlane(GX_PLANEMASK_OBJ); GXS_SetVisiblePlane(GX_PLANEMASK_OBJ);
    G2_BlendNone(); G2S_BlendNone();
    u16 backdrop = GX_RGB(0,0,0);
    GX_LoadBGPltt(&backdrop, 0, 2); GXS_LoadBGPltt(&backdrop, 0, 2);
    GX_LoadOBJ(chr->pRawData, 0, chr->szByte); GXS_LoadOBJ(chr->pRawData, 0, chr->szByte);
    GX_LoadOBJPltt(palette->pRawData, 0, palette->szByte);
    GXS_LoadOBJPltt(palette->pRawData, 0, palette->szByte);
    GX_LoadOAM(objects, 0, sizeof(objects)); GXS_LoadOAM(objects, 0, sizeof(objects));
    printf("SPRITE case=%u sequence=%u cell_oams=%u emitted=%u\n", selected, sequence, cell->numOAMAttrs, count);
    fflush(stdout);
    SingleScreen_ShowSubEngine(FALSE); DEMOStartDisplay();
    for (unsigned frame = 1;; ++frame) {
        if (frame == 40) SingleScreen_ShowSubEngine(TRUE);
        if (frame == 80) SingleScreen_ShowSubEngine(FALSE);
        OS_WaitVBlankIntr();
    }
}
void VBlankIntr(void) { OS_SetIrqCheckFlag(OS_IE_V_BLANK); }
