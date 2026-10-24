/* Minimal host allocation API, not the DS heap implementation. */
#ifndef HG_HOST_HEAP_H
#define HG_HOST_HEAP_H
#include "global.h"
#ifdef __cplusplus
extern "C" {
#endif
enum HeapID { HG_HOST_TEST_HEAP = 0 };
void *Heap_Alloc(enum HeapID heap, u32 size);
void *Heap_AllocAtEnd(enum HeapID heap, u32 size);
void Heap_Free(void *pointer);
#ifdef __cplusplus
}
#endif
#endif
