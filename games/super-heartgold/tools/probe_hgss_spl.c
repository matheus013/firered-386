#include "spl_internal.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHECK(c) do { if (!(c)) abort(); } while (0)
static void *blocks[1024];
static unsigned allocated;
#ifdef HG_SPL_RESOURCE_TEST
/* Loader stores field callbacks but does not execute their RNG in this test. */
u32 spl_rndm_seed;
void SIM_handleAssertionFailureMsg(const char *file, unsigned line, const char *format, ...) {
    (void)format;
    fprintf(stderr, "SDK assertion: %s:%u\n", file, line);
    abort();
}
void MI_Copy36B(const void *source, void *destination) { memcpy(destination, source, 36); }
#endif
static void *allocate(u32 bytes) {
    CHECK(allocated < 1024);
    void *result = malloc(bytes);
    CHECK(result);
    memset(result, 0xA5, bytes);
    blocks[allocated++] = result;
    return result;
}
/* Controlled byte-fill boundary. Graphics and resource loading are not called. */
void MI_CpuFill8(void *destination, u8 value, u32 size) { memset(destination, value, size); }

static void check_list(SplList *list, unsigned expected) {
    SplNode *previous = NULL;
    unsigned count = 0;
    for (SplNode *node = list->p_begin; node; node = node->p_next) {
        CHECK(++count <= expected);
        CHECK(node->p_prev == previous);
        CHECK((uintptr_t)node % sizeof(void *) == 0);
        previous = node;
    }
    CHECK(count == expected && list->node_num == (int)expected);
    CHECK(list->p_end == previous);
}

int main(int argc, char **argv) {
    for (unsigned cycle = 0; cycle < 100; ++cycle) {
        allocated = 0;
        SPLManager *manager = SPL_Init(allocate, 20, 200, 5, 6, 63);
        CHECK(allocated == 3 && manager->max_emtr_num == 20 && manager->max_ptcl_num == 200);
        check_list((SplList *)&manager->ina_emtr_list, 20);
        check_list((SplList *)&manager->ina_ptcl_list, 200);
        check_list((SplList *)&manager->act_emtr_list, 0);
        CHECK(manager->res_ary == NULL && manager->tex_ary == NULL);
        CHECK(manager->polygonID.fix == 5 && manager->polygonID.current == 6);
        SplList *list = (SplList *)&manager->ina_ptcl_list;
        SplNode *middle = list->p_begin->p_next;
        CHECK(spl_del(list, middle) == middle);
        check_list(list, 199);
        spl_push_front(list, middle);
        check_list(list, 200);
        SplNode *tail = list->p_end;
        CHECK(spl_del(list, tail) == tail);
        check_list(list, 199);
        spl_push_front(list, tail);
        for (unsigned remaining = 200; remaining; --remaining) {
            CHECK(spl_pop_front(list));
            check_list(list, remaining - 1);
        }
        CHECK(spl_pop_front(list) == NULL);
        for (unsigned i = 0; i < allocated; ++i) free(blocks[i]);
    }
    puts("SPL: 100 pool initialization and list lifecycle cycles passed");
#ifdef HG_SPL_RESOURCE_TEST
    CHECK(argc == 2);
    FILE *file = fopen(argv[1], "rb");
    CHECK(file && fseek(file, 0, SEEK_END) == 0);
    long length = ftell(file);
    CHECK(length >= 32 && length < 1024 * 1024);
    rewind(file);
    unsigned char *data = malloc((size_t)length);
    CHECK(data && fread(data, 1, (size_t)length, file) == (size_t)length);
    fclose(file);
    allocated = 0;
    SPLManager *manager = SPL_Init(allocate, 20, 200, 5, 6, 63);
    SPL_Load(manager, data);
    CHECK(manager->res_num == (data[8] | data[9] << 8));
    CHECK(manager->tex_num == (data[10] | data[11] << 8));
    for (unsigned i = 0; i < manager->res_num; ++i) {
        uintptr_t base = (uintptr_t)manager->res_ary[i].p_base;
        CHECK(base >= (uintptr_t)data && base + sizeof(SPLResBase) <= (uintptr_t)data + length);
    }
    printf("SPL resource: %u emitters and %u textures loaded\n", manager->res_num, manager->tex_num);
    for (unsigned i = 0; i < allocated; ++i) free(blocks[i]);
    free(data);
#else
    (void)argc; (void)argv;
#endif
    return 0;
}
