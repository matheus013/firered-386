/* Original HGSS heap + real NNS allocator, with deterministic OS boundaries.
 * No graphics assets are registered, so their release hooks are empty.
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "heap.h"
#include "pm_string.h"
#define CHECK(c) do { if (!(c)) { fprintf(stderr, "Heap check failed: %s\n", #c); abort(); } } while (0)
static unsigned char *arena;
static size_t used;
static unsigned critical;
void GF_AssertFail(void) { fputs("HGSS heap assertion\n", stderr); abort(); }
BOOL CommState_Exists(void) { return FALSE; }
void PrintErrorMessageAndReset(void) { abort(); }
OSProcMode OS_GetProcMode(void) { return OS_PROCMODE_USER; }
OSIntrMode OS_DisableInterrupts(void) { CHECK(!critical); critical = 1; return 0; }
OSIntrMode OS_RestoreInterrupts(OSIntrMode previous) { CHECK(critical && !previous); critical = 0; return 0; }
void *OS_AllocFromArenaLo(OSArenaId id, u32 size, u32 align) {
    (void)id;
    used = (used + align - 1) & ~(size_t)(align - 1);
    CHECK(used + size <= 2 * 1024 * 1024);
    void *result = arena + used;
    used += size;
    return result;
}
void *OS_AllocFromArenaHi(OSArenaId id, u32 size, u32 align) {
    return OS_AllocFromArenaLo(id, size, align);
}
void WIN_CheckAndFreeAnimBank(void *start, void *end) { (void)start; (void)end; }
void WIN_CheckAndFreeCellBank(void *start, void *end) { (void)start; (void)end; }
void WIN_CheckAndFreeCharData(void *start, void *end) { (void)start; (void)end; }
void MIi_CpuClear32(u32 data, void *destination, u32 size) {
    for (u32 offset = 0; offset < size; offset += 4)
        memcpy((unsigned char *)destination + offset, &data, 4);
}
int main(void) {
    arena = calloc(1, 2 * 1024 * 1024);
    CHECK(arena);
    HeapParam templates[] = {{1024 * 1024, OS_ARENA_MAIN}};
    Heap_InitSystem(templates, 1, 64, 4);
    u32 root_free = HeapExp_FndGetTotalFreeSize(0);
    CHECK(Heap_Create(0, 1, 128 * 1024));
    CHECK(Heap_CreateAtEnd(0, 2, 128 * 1024));
    u32 child_free = HeapExp_FndGetTotalFreeSize(1);
    for (unsigned iteration = 0; iteration < 1000; ++iteration) {
        void *blocks[32];
        for (unsigned i = 0; i < 32; ++i) {
            unsigned size = 17 + i * 3;
            blocks[i] = i & 1 ? Heap_AllocAtEnd(1, size) : Heap_Alloc(1, size);
            CHECK(blocks[i]);
            CHECK((uintptr_t)blocks[i] % _Alignof(void *) == 0);
            memset(blocks[i], (int)i, size);
        }
        for (unsigned i = 0; i < 32; ++i) {
            unsigned size = 17 + i * 3;
            for (unsigned j = 0; j < size; ++j) CHECK(((unsigned char *)blocks[i])[j] == i);
            Heap_Free(blocks[i]);
        }
    }
    CHECK(HeapExp_FndGetTotalFreeSize(1) == child_free);
    CHECK(!Heap_Alloc(1, 4 * 1024 * 1024));
    for (unsigned i = 0; i < 1000; ++i) {
        const u16 sample[] = {0x121, 0x122, 0x123, EOS};
        String *text = String_New(16, 2);
        CHECK(text);
        CopyU16ArrayToStringN(text, sample, 4);
        String *copy = String_Dup(text, 1);
        CHECK(copy && !String_Compare(text, copy));
        String_Cat(text, copy);
        String_AddChar(text, 0x124);
        CHECK(String_GetLength(text) == 7 && String_cstr(text)[7] == EOS);
        CHECK(String_cstr(text)[3] == 0x121 && String_cstr(text)[6] == 0x124);
        String_SetEmpty(text);
        CHECK(String_GetLength(text) == 0 && String_cstr(text)[0] == EOS);
        String_Delete(copy);
        String_Delete(text);
    }
    CHECK(HeapExp_FndGetTotalFreeSize(1) == child_free);
    Heap_Destroy(1);
    Heap_Destroy(2);
    CHECK(HeapExp_FndGetTotalFreeSize(0) == root_free);
    /* SDK callers outside HGSS heap.c still request +/-4. Exercise both
     * directions and a deliberately four-byte-offset initial arena. */
    unsigned char *raw = malloc(128 * 1024 + 4);
    CHECK(raw);
    NNSFndHeapHandle direct = NNS_FndCreateExpHeap(raw + 4, 128 * 1024);
    CHECK(direct);
    u32 direct_free = NNS_FndGetTotalFreeSizeForExpHeap(direct);
    const int alignments[] = {4, -4, 8, -8, 16, -16, 32, -32};
    for (unsigned a = 0; a < 8; ++a) {
        for (unsigned size = 0; size < 128; ++size) {
            void *block = NNS_FndAllocFromExpHeapEx(direct, size, alignments[a]);
            unsigned alignment = abs(alignments[a]);
            if (alignment < sizeof(void *)) alignment = sizeof(void *);
            CHECK(block && (uintptr_t)block % alignment == 0);
            memset(block, 0x5a, size ? size : 1);
            NNS_FndFreeToExpHeap(direct, block);
            CHECK(NNS_FndGetTotalFreeSizeForExpHeap(direct) == direct_free);
        }
    }
    void *resize = NNS_FndAllocFromExpHeapEx(direct, 97, 4);
    CHECK(resize);
    CHECK(NNS_FndResizeForMBlockExpHeap(direct, resize, 25) == 32);
    NNS_FndFreeToExpHeap(direct, resize);
    CHECK(NNS_FndGetTotalFreeSizeForExpHeap(direct) == direct_free);
    NNS_FndDestroyExpHeap(direct);
    free(raw);
    CHECK(!critical);
    free(arena);
    puts("HGSS heap: 32000 allocation cycles, 1000 string cycles and 1024 direct SDK alignment cases passed");
    return 0;
}
