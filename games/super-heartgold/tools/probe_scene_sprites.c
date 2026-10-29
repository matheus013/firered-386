/* Cell rendering through SDK animation, OAM conversion and GX upload. */
#include <nitro.h>
#include <nitro/demo/DEMO.h>
#include <nnsys/g2d/g2d_CellAnimation.h>
#include <nnsys/g2d/fmt/g2d_Oam_data.h>
#include <nnsys/g2d/load/g2d_NCG_load.h>
#include <nnsys/g2d/load/g2d_NCL_load.h>
#include <nnsys/g2d/load/g2d_NCE_load.h>
#include <nnsys/g2d/load/g2d_NAN_load.h>
#include <nnsys/g2d/load/g2d_NSC_load.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef HG_SPRITE_MANAGER
#include "probe_scene_sprite_manager.h"
#endif

void SingleScreen_ShowSubEngine(BOOL show);
/* Supply size tables only when the SDK OAM module is not linked. */
#ifndef HG_ORIGINAL_OAM
NNS_G2D_DEFINE_NNSI_OBJSIZEWTBL;
NNS_G2D_DEFINE_NNSI_OBJSIZEHTBL;
#endif
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

static void load_backgrounds(void) {
    /* Separate BG and OBJ banks; same BG1 layout/resources as scene 4. */
    GX_SetBankForBG(GX_VRAM_BG_128_B);
    GX_SetBankForSubBG(GX_VRAM_SUB_BG_128_C);
    G2_SetBG1Control(GX_BG_SCRSIZE_TEXT_256x256, GX_BG_COLORMODE_16,
                    GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x04000, GX_BG_EXTPLTT_01);
    G2S_SetBG1Control(GX_BG_SCRSIZE_TEXT_256x256, GX_BG_COLORMODE_16,
                     GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x04000, GX_BG_EXTPLTT_01);
    G2_SetBG1Priority(1); G2S_SetBG1Priority(1);
    NNSG2dCharacterData *chr;
    NNSG2dPaletteData *palette;
    NNSG2dScreenData *main_map, *sub_map;
    if (!NNS_G2dGetUnpackedBGCharacterData(load(54), &chr) ||
        !NNS_G2dGetUnpackedPaletteData(load(53), &palette) ||
        !NNS_G2dGetUnpackedScreenData(load(58), &main_map) ||
        !NNS_G2dGetUnpackedScreenData(load(57), &sub_map)) abort();
    GX_LoadBG1Char(chr->pRawData, 0, chr->szByte);
    GXS_LoadBG1Char(chr->pRawData, 0, chr->szByte);
    GX_LoadBGPltt(palette->pRawData, 0, 0x80);
    GXS_LoadBGPltt(palette->pRawData, 0, 0x80);
    GX_LoadBG1Scr(main_map->rawData, 0, main_map->szByte);
    GXS_LoadBG1Scr(sub_map->rawData, 0, sub_map->szByte);
    GX_SetVisiblePlane(GX_PLANEMASK_OBJ | GX_PLANEMASK_BG1);
    GXS_SetVisiblePlane(GX_PLANEMASK_OBJ | GX_PLANEMASK_BG1);
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
    NNSG2dFVec2 initial_origin = origin;
    if (getenv("HG_SPRITE_INITIAL_TRANSFORM")) {
        unsigned element = seq->animType & 255;
        if (element == NNS_G2D_ANIMELEMENT_INDEX_T) {
            const NNSG2dAnimDataT *value = NNS_G2dGetAnimCtrlCurrentElement(&animation.animCtrl);
            initial_origin.x += value->px * FX32_ONE;
            initial_origin.y += value->py * FX32_ONE;
        } else if (element != NNS_G2D_ANIMELEMENT_INDEX) abort();
    }
    unsigned count = 0;
    if (!getenv("HG_SPRITE_DISABLE_DRAW"))
        count = NNS_G2dMakeCellToOams(objects, 128, cell, NULL, &initial_origin, 0, FALSE);
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
#ifndef HG_ORIGINAL_TRANSFERS
    GX_LoadOBJ(chr->pRawData, 0, chr->szByte); GXS_LoadOBJ(chr->pRawData, 0, chr->szByte);
    GX_LoadOBJPltt(palette->pRawData, 0, palette->szByte);
    GXS_LoadOBJPltt(palette->pRawData, 0, palette->szByte);
#else
    ObjCharTransferTemplate transfers = {2, 0x10000, 0x10000, 0};
    ObjCharTransfer_InitEx(&transfers, chr->mappingType, chr->mappingType);
    ObjCharTransfer_ClearBuffers();
    ObjPlttTransfer_Init(2, 0);
    ObjPlttTransfer_Reset();
#endif
#ifdef HG_ORIGINAL_OAM
    NNS_G2dInitOamManagerModule();
    ClearMainOAM(0); ClearSubOAM(0);
    OamManager_Create(4, 124, 1, 31, 0, 128, 0, 32, 0);
#endif
#ifdef HG_SPRITE_MANAGER
    for (unsigned i = 0; i < 128; ++i) G2_SetOBJEffect(&objects[i], GX_OAM_EFFECT_NODISPLAY, 0);
    count = render_manager(objects, chr, palette, bank, animations, sequence, FALSE);
    upload_manager(objects, FALSE);
    for (unsigned i = 0; i < 128; ++i) G2_SetOBJEffect(&objects[i], GX_OAM_EFFECT_NODISPLAY, 0);
    render_manager(objects, chr, palette, bank, animations, sequence, TRUE);
    upload_manager(objects, TRUE);
#else
    GX_LoadOAM(objects, 0, sizeof(objects)); GXS_LoadOAM(objects, 0, sizeof(objects));
#endif
    if (getenv("HG_SPRITE_COMPOSE")) load_backgrounds();
    printf("SPRITE case=%u sequence=%u cell_oams=%u emitted=%u\n", selected, sequence, cell->numOAMAttrs, count);
    fflush(stdout);
    SingleScreen_ShowSubEngine(FALSE); DEMOStartDisplay();
    for (unsigned frame = 1;; ++frame) {
        if (getenv("HG_SPRITE_PLAYBACK")) {
#ifdef HG_SPRITE_MANAGER
            tick_manager(objects, FALSE);
            upload_manager(objects, FALSE);
            tick_manager(objects, TRUE);
            upload_manager(objects, TRUE);
            if (frame == 160) destroy_managers();
#else
            NNS_G2dTickCellAnimation(&animation, FX32_ONE);
            cell = NNS_G2dGetCellAnimationCurrentCell(&animation);
            if (!cell || cell->numOAMAttrs > 128) abort();
            NNSG2dFVec2 position = origin;
            unsigned element = seq->animType & 255;
            if (element == NNS_G2D_ANIMELEMENT_INDEX_T) {
                const NNSG2dAnimDataT *value = NNS_G2dGetAnimCtrlCurrentElement(&animation.animCtrl);
                position.x += value->px * FX32_ONE;
                position.y += value->py * FX32_ONE;
            } else if (element != NNS_G2D_ANIMELEMENT_INDEX) abort();
            for (unsigned i = 0; i < 128; ++i)
                G2_SetOBJEffect(&objects[i], GX_OAM_EFFECT_NODISPLAY, 0);
            if (!getenv("HG_SPRITE_DISABLE_DRAW"))
                NNS_G2dMakeCellToOams(objects, 128, cell, NULL, &position, 0, FALSE);
            GX_LoadOAM(objects, 0, sizeof(objects));
            GXS_LoadOAM(objects, 0, sizeof(objects));
#endif
        }
        if (frame == 40) SingleScreen_ShowSubEngine(TRUE);
        if (frame == 80) SingleScreen_ShowSubEngine(FALSE);
        OS_WaitVBlankIntr();
    }
}
void VBlankIntr(void) { OS_SetIrqCheckFlag(OS_IE_V_BLANK); }
