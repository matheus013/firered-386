/* Diagnostic native integration, using private SPA data and original SPL units. */
#include <nitro.h>
#include <nitro/demo/DEMO.h>
#include "spl_internal.h"
#include <stdio.h>
#include <stdlib.h>

/* Ordering verified against ctrl-cheeb-del/pokeheartgold lib/asm/spl_data.s,
 * revision 01eeeb4b96087cb94c84de3045e9f2d4ef00061b. */
void (*const gSplDrawPlaneFuncs[2])(fx32, fx32, fx32, fx32) = {drawXYPlane, drawXZPlane};
void (*const gSplRotateFuncs[2])(fx32, fx32, MtxFx43 *) = {rotTypeY, rotTypeXYZ};
const VecFx16 gSplAxisY = {0, FX16_ONE, 0};
u32 spl_rndm_seed;
void SingleScreen_ShowSubEngine(BOOL show);
static u32 texture_used, palette_used;
static void *allocate(u32 size) {
    void *block = calloc(1, size);
    if (!block) abort();
    return block;
}
static u32 allocate_texture(u32 size, BOOL compressed) {
    /* This fixture needs no compressed-texture auxiliary bank allocation. */
    if (compressed) abort();
    u32 address = (texture_used + 7) & ~7u;
    if (address > 128*1024 || size > 128*1024 - address) abort();
    texture_used = address + size;
    return address;
}
static u32 allocate_palette(u32 size, BOOL four_color) {
    u32 address = (palette_used + 15) & ~15u;
    if (address > 64*1024 || size > 64*1024 - address) abort();
    palette_used = address + size;
    return address;
}
void NitroMain(void) {
    DEMOInitCommon();
    DEMOInitVRAM();
    DEMOInitDisplay3D();
    DEMOStartDisplay();
    GXS_SetGraphicsMode(GX_BGMODE_0);
    GXS_SetVisiblePlane(0);
    GXS_DispOn();
    u16 blue = GX_RGB(0, 0, 31);
    GXS_LoadBGPltt(&blue, 0, sizeof(blue));
    SingleScreen_ShowSubEngine(FALSE);
    FILE *file = fopen("intro-particles.spa", "rb");
    if (!file || fseek(file, 0, SEEK_END)) abort();
    long size = ftell(file);
    if (size <= 0 || size > 1024*1024 || fseek(file, 0, SEEK_SET)) abort();
    void *data = allocate(size);
    if (fread(data, 1, size, file) != (size_t)size) abort();
    fclose(file);
    SPLManager *manager = SPL_Init(allocate, 20, 200, 5, 6, 63);
    SPL_Load(manager, data);
    const char *selection = getenv("HG_SPL_RESOURCE");
    char *end = NULL;
    long resource = selection ? strtol(selection, &end, 10) : 0;
    if (resource < 0 || resource >= manager->res_num || (selection && (end == selection || *end))) abort();
    const char *group_selection = getenv("HG_SPL_GROUP");
    long group = group_selection ? strtol(group_selection, &end, 10) : -1;
    if (group_selection && (group < 0 || group > 2 || end == group_selection || *end || selection)) abort();
    /* Original scene 4 resource ordering, pinned base 6984a3d67592157195940049be2e4bbbf933f9ff. */
    const unsigned starter_groups[3][3] = {{6,7,8}, {3,4,5}, {0,1,2}};
    if (!SPL_LoadTexByCallbackFunction(manager, allocate_texture) ||
        !SPL_LoadTexPlttByCallbackFunction(manager, allocate_palette)) abort();
    unsigned peak = 0;
    const int disabled = getenv("HG_SPL_DISABLE_DRAW") != NULL;
    const int sequence = getenv("HG_SPL_SEQUENCE") != NULL;
    const int scene_camera = sequence || getenv("HG_SPL_SCENE_CAMERA") != NULL;
    unsigned next_group = 1;
    if (sequence) group = 0;
    if (scene_camera) {
        /* Camera_ApplyPerspectiveType(ORTHOGRAPHIC), angle 8192, distance 4;
         * scene 4 overrides the near plane to 1 and retains far plane 900. */
        fx32 height = FX_Mul(FX_Div(FX_SinIdx(8192), FX_CosIdx(8192)), 4 * FX32_ONE);
        fx32 width = FX_Mul(height, FX32_CONST(1.33333333));
        G3_Ortho(height, -height, -width, width, FX32_ONE, 900 * FX32_ONE, NULL);
    }
    for (unsigned frame = 1;; ++frame) {
        if (!sequence && frame == 40) SingleScreen_ShowSubEngine(TRUE);
        if (!sequence && frame == 80) SingleScreen_ShowSubEngine(FALSE);
        /* Resource zero is a short burst. Respawn before the final capture. */
        if (sequence ? (next_group && group < 3) : (frame == 1 || frame == 100)) {
            if (sequence) {
                if (manager->ina_ptcl_list.node_num != 200 || manager->ina_emtr_list.node_num != 20) abort();
                next_group = 0;
                peak = 0;
            } else SPL_DeleteAll(manager);
            if (!sequence) spl_rndm_seed = 0x12345678;
            VecFx32 origin = {0, 0, 0};
            if (group >= 0) {
                origin.z = FX32_ONE / 64; /* Original emitter callback position. */
                for (unsigned i = 0; i < 3; ++i)
                    SPL_Create(manager, starter_groups[group][i], &origin);
            } else {
                SPL_Create(manager, resource, &origin);
            }
        }
        if (!scene_camera) SPL_Calc(manager);
        unsigned active = 200 - manager->ina_ptcl_list.node_num;
        if (active > peak) peak = active;
        G3X_Reset();
        VecFx32 eye = {0, 0, 8 * FX32_ONE}, at = {0, 0, 0}, up = {0, FX32_ONE, 0};
        if (scene_camera) eye.z = 4 * FX32_ONE;
        MtxFx43 camera;
        G3_LookAt(&eye, &up, &at, &camera);
        if (!disabled) SPL_Draw(manager, &camera);
        if (scene_camera) SPL_Calc(manager);
        if (sequence && group < 3 && !next_group && manager->act_emtr_list.node_num == 0) {
            if (manager->ina_ptcl_list.node_num != 200 || manager->ina_emtr_list.node_num != 20 || peak == 0) abort();
            printf("SPL_SEQUENCE group=%ld end_frame=%u peak=%u particles_free=200 emitters_free=20\n", group, frame, peak);
            fflush(stdout);
            ++group;
            next_group = 1;
        }
        G3_SwapBuffers(scene_camera ? GX_SORTMODE_MANUAL : GX_SORTMODE_AUTO,
                       scene_camera ? GX_BUFFERMODE_Z : GX_BUFFERMODE_W);
        if (frame == 110) {
            printf("SPL_RENDER peak=%u active=%u textures=%u palettes=%u disabled=%d\n",
                   peak, active, texture_used, palette_used, disabled);
            fflush(stdout);
        }
        OS_WaitVBlankIntr();
    }
}
void VBlankIntr(void) { OS_SetIrqCheckFlag(OS_IE_V_BLANK); }
