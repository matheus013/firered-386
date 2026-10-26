#include "spl_internal.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHECK(c) do { if (!(c)) abort(); } while (0)
static void *blocks[3];
static unsigned allocated;
static void *allocate(u32 bytes) {
    CHECK(allocated < 3);
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

int main(void) {
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
    return 0;
}
