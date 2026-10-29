/* Host adapters and capture boundary for the original SpriteList renderer. */
#include "sprite.h"
#include "gf_g2d_renderer.h"
#include <stddef.h>
extern void WIN_CheckAndFreeAnimBank(void *, void *);
typedef union HostAllocation { max_align_t alignment; size_t size; } HostAllocation;
void GF_AssertFail(void) { abort(); }
void *Heap_Alloc(enum HeapID id, u32 size) {
    (void)id;
    HostAllocation *p = calloc(1, sizeof(*p) + size);
    if (!p) abort();
    p->size = size;
    return p + 1;
}
void Heap_Free(void *ptr) {
    if (!ptr) return;
    HostAllocation *p = (HostAllocation *)ptr - 1;
    WIN_CheckAndFreeAnimBank(ptr, (char *)ptr + p->size);
    free(p);
}
void *Sys_AllocAndReadFile(enum HeapID id, const char *path) {
    if (strcmp(path, "data/clact_default.NANR")) abort();
    FILE *f = fopen("default.NANR", "rb");
    if (!f || fseek(f, 0, SEEK_END)) abort();
    long size = ftell(f);
    if (size <= 0 || size > 1048576) abort();
    rewind(f);
    void *p = Heap_Alloc(id, size);
    if (fread(p, 1, size, f) != (size_t)size) abort();
    fclose(f);
    return p;
}
static GXOamAttr *captured_oams;
static unsigned captured_count;
static SpriteList *playback_lists[2];
static NNSG2dRendererInstance renderers[2];
static NNSG2dRenderSurface surfaces[2];
static BOOL capture_oam(const GXOamAttr *oam, u16 affine, BOOL doubled) {
    (void)affine;
    if (doubled || captured_count >= 128) abort();
    captured_oams[captured_count++] = *oam;
    return TRUE;
}
static u16 reject_affine(const MtxFx22 *matrix) { (void)matrix; abort(); }
static unsigned render_manager(GXOamAttr *objects, NNSG2dCharacterData *chr,
    NNSG2dPaletteData *pal, NNSG2dCellDataBank *cells, NNSG2dAnimBankData *anims,
    unsigned sequence, BOOL sub) {
    NNSG2dRendererInstance *renderer = &renderers[sub != 0];
    NNSG2dRenderSurface *surface = &surfaces[sub != 0];
    NNSG2dViewRect view = {{0,0}, {256 * FX32_ONE,192 * FX32_ONE}};
    GF_InitG2dRenderer(renderer, 0);
    GF_InitG2dRenderSurface(surface, &view, capture_oam, reject_affine, NULL,
        sub ? NNS_G2D_SURFACETYPE_SUB2D : NNS_G2D_SURFACETYPE_MAIN2D, renderer);
    NNSG2dImageProxy image;
    NNSG2dImagePaletteProxy palette;
    NNS_G2dInitImageProxy(&image);
    NNS_G2dInitImagePaletteProxy(&palette);
    NNS_G2dLoadImage1DMapping(chr, 0, sub ? NNS_G2D_VRAM_TYPE_2DSUB : NNS_G2D_VRAM_TYPE_2DMAIN, &image);
    NNS_G2dLoadPalette(pal, 0, sub ? NNS_G2D_VRAM_TYPE_2DSUB : NNS_G2D_VRAM_TYPE_2DMAIN, &palette);
    SpriteResourcesHeader header = {0};
    header.imageProxy = &image; header.plttProxy = &palette;
    header.cellData = cells; header.cellAnim = anims;
    SpriteListParam param = {1, renderer, 0};
    SpriteList *list = SpriteList_Create(&param);
    if (!list) abort();
    SpriteTemplate template = {0};
    template.spriteList = list; template.header = &header;
    template.position.x = 128 * FX32_ONE; template.position.y = 96 * FX32_ONE;
    template.scale.x = template.scale.y = template.scale.z = FX32_ONE;
    template.whichScreen = sub ? NNS_G2D_VRAM_TYPE_2DSUB : NNS_G2D_VRAM_TYPE_2DMAIN;
    Sprite *sprite = Sprite_CreateAffine(&template);
    if (!sprite) abort();
    Sprite_SetAnimCtrlSeq(sprite, sequence);
    Sprite_SetAnimSpeed(sprite, FX32_ONE);
    Sprite_SetAnimActiveFlag(sprite, FALSE);
    Sprite_SetDrawFlag(sprite, !getenv("HG_SPRITE_DISABLE_DRAW"));
    captured_oams = objects; captured_count = 0;
    SpriteList_RenderAndAnimateSprites(list);
    unsigned count = captured_count;
    SpriteAnimationData *state = (SpriteAnimationData *)sprite->animationData;
    unsigned expected = getenv("HG_SPRITE_DISABLE_DRAW") ? 0 :
        NNS_G2dGetCellAnimationCurrentCell(&state->animation)->numOAMAttrs;
    if (count != expected) abort();
    if (getenv("HG_SPRITE_PLAYBACK")) {
        playback_lists[sub != 0] = list;
        Sprite_SetAnimActiveFlag(sprite, TRUE);
        /* Original manager draws before ticking. Prime one tick to match the SDK reference. */
        Sprite_UpdateAnim(sprite, FX32_ONE);
    } else if (!SpriteList_Delete(list)) abort();
    return count;
}


static void tick_manager(GXOamAttr *objects, BOOL sub) {
    SpriteList *list = playback_lists[sub != 0];
    if (!list) abort();
    for (unsigned i = 0; i < 128; ++i) G2_SetOBJEffect(&objects[i], GX_OAM_EFFECT_NODISPLAY, 0);
    captured_oams = objects; captured_count = 0;
    SpriteList_RenderAndAnimateSprites(list);
}
static void destroy_managers(void) {
    for (unsigned i = 0; i < 2; ++i) {
        if (!SpriteList_Delete(playback_lists[i])) abort();
        playback_lists[i] = NULL;
    }
}
