/* Original SpriteList lifecycle with real SDK controllers and explicit host IO/heap adapters. */
#include "sprite.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>

#define CHECK(x) do { if (!(x)) { fprintf(stderr, "failed line %d: %s\n", __LINE__, #x); abort(); } } while (0)
extern void WIN_CheckAndFreeAnimBank(void *, void *);
extern void WIN_CheckAndFreeCellBank(void *, void *);
static const char *default_animation;
static unsigned allocations;
typedef union AllocationHeader { max_align_t alignment; size_t size; } AllocationHeader;
void GF_AssertFail(void) { abort(); }
void SIM_handleAssertionFailureMsg(const char *file, unsigned line, const char *fmt, ...) {
    (void)fmt; fprintf(stderr, "SDK assertion %s:%u\n", file, line); abort();
}
void *Heap_Alloc(enum HeapID heapID, u32 size) {
    (void)heapID;
    AllocationHeader *block = calloc(1, sizeof(*block) + size); CHECK(block);
    block->size = size; ++allocations;
    return block + 1;
}
void Heap_Free(void *ptr) {
    if (!ptr) return;
    AllocationHeader *block = (AllocationHeader *)ptr - 1;
    WIN_CheckAndFreeAnimBank(ptr, (unsigned char *)ptr + block->size);
    CHECK(allocations); --allocations; free(block);
}
static void *read_file(const char *path) {
    FILE *file = fopen(path, "rb"); CHECK(file);
    CHECK(fseek(file, 0, SEEK_END) == 0);
    long size = ftell(file); CHECK(size > 0 && size < 1048576);
    rewind(file);
    void *data = Heap_Alloc(0, size);
    CHECK(fread(data, 1, size, file) == (size_t)size); fclose(file);
    return data;
}
void *Sys_AllocAndReadFile(enum HeapID heapID, const char *path) {
    (void)heapID;
    CHECK(strcmp(path, "data/clact_default.NANR") == 0);
    return read_file(default_animation);
}
static void check_list(SpriteList *list, unsigned count) {
    unsigned seen = 0, previous_priority = 0;
    Sprite *previous = &list->dummy;
    for (Sprite *p = list->dummy.next; p != &list->dummy; p = p->next) {
        CHECK(++seen <= count && p->prev == previous);
        CHECK(seen == 1 || p->drawPriority >= previous_priority);
        CHECK(p->spriteList == list);
        previous = p; previous_priority = p->drawPriority;
    }
    CHECK(seen == count && list->dummy.prev == previous && list->stackPointer == count);
}
int main(int argc, char **argv) {
    CHECK(argc == 4); default_animation = argv[1];
    struct { u64 guard; NNSG2dCellDataBank *cells; NNSG2dAnimBankData *anims; u64 end; } resources = {0};
    void *cells = read_file(argv[2]), *anims = read_file(argv[3]);
    CHECK(NNS_G2dGetUnpackedCellBank(cells, &resources.cells));
    CHECK(NNS_G2dGetUnpackedAnimBank(anims, &resources.anims));
    NNSG2dImageProxy image;
    NNSG2dImagePaletteProxy palette;
    NNS_G2dInitImageProxy(&image); NNS_G2dInitImagePaletteProxy(&palette);
    NNS_G2dSetImageLocation(&image, NNS_G2D_VRAM_TYPE_2DMAIN, 0);
    NNS_G2dSetImagePaletteLocation(&palette, NNS_G2D_VRAM_TYPE_2DMAIN, 0);
    palette.fmt = GX_TEXFMT_PLTT16;
    SpriteResourcesHeader header = {0};
    header.imageProxy = &image; header.plttProxy = &palette;
    header.cellData = resources.cells; header.cellAnim = resources.anims;
    unsigned baseline = allocations;
    for (unsigned cycle = 0; cycle < 64; ++cycle) {
        /* No draw calls: only the core's overwrite/flip fields are accessed. */
        NNSG2dRendererInstance renderer = {0};
        SpriteListParam param = {6, &renderer, 0};
        SpriteList *list = SpriteList_Create(&param); CHECK(list);
        SpriteTemplate template = {0};
        template.spriteList = list; template.header = &header;
        template.scale.x = template.scale.y = template.scale.z = FX32_ONE;
        template.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
        Sprite *sprites[6];
        for (unsigned i = 0; i < 6; ++i) {
            template.drawPriority = (i * 5) % 6;
            sprites[i] = Sprite_CreateAffine(&template); CHECK(sprites[i]);
            check_list(list, i + 1);
        }
        CHECK(Sprite_CreateAffine(&template) == NULL);
        for (unsigned t = 0; t < 160; ++t) for (unsigned i = 0; i < 6; ++i) {
            Sprite_UpdateAnim(sprites[i], FX32_ONE);
            CHECK(memcmp(&sprites[i]->imageProxy, &image, sizeof(image)) == 0);
            CHECK(memcmp(&sprites[i]->paletteProxy, &palette, sizeof(palette)) == 0);
        }
        Sprite_Delete(sprites[2]); check_list(list, 5);
        Sprite *reused = Sprite_CreateAffine(&template); CHECK(reused == sprites[2]);
        check_list(list, 6);
        CHECK(SpriteList_DeleteAllSprites(list)); check_list(list, 0);
        CHECK(SpriteList_Delete(list)); CHECK(allocations == baseline);
    }
    WIN_CheckAndFreeCellBank(&resources, &resources + 1);
    WIN_CheckAndFreeAnimBank(&resources, &resources + 1);
    Heap_Free(cells); Heap_Free(anims); CHECK(allocations == 0);
    puts("64 SpriteList cycles, 448 creates, capacity/reuse/order and 61440 ticks passed");
}

