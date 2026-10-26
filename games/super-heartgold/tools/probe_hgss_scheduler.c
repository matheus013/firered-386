/* Execute the original HGSS scheduler with native pointers and sanitizers. */

#include <stdio.h>
#include <stdlib.h>
#include "sys_task.h"
#define CHECK(condition) do { if (!(condition)) { fprintf(stderr, "Check failed at line %d: %s\n", __LINE__, #condition); abort(); } } while (0)

static unsigned events[32], count;
/* Test boundary: a game assertion must fail the process, never disappear. */
void GF_AssertFail(void) { fputs("HGSS assertion failed\n", stderr); abort(); }
static void record(SysTask *task, void *data) {
    CHECK(count < 32);
    events[count++] = *(unsigned *)data;
    CHECK(SysTask_GetData(task) == data);
}
static void remove_self(SysTask *task, void *data) {
    record(task, data);
    CHECK(SysTask_Unlink(task));
}
static SysTask *victim;
static void remove_next(SysTask *task, void *data) {
    record(task, data);
    CHECK(SysTask_Unlink(victim));
    CHECK(SysTask_Unlink(task));
}
static unsigned child_id = 9;
static void create_child(SysTask *task, void *data) {
    record(task, data);
    CHECK(SysTaskQueue_InsertTask(task->queue, record, &child_id, 20));
    CHECK(SysTask_Unlink(task));
}
static SysTaskQueue *new_queue(unsigned capacity) {
    void *memory = calloc(1, SysTaskQueue_GetArenaSize(capacity));
    CHECK(memory);
    return SysTaskQueue_PlacementNew(capacity, memory);
}
int main(void) {
    unsigned assert_effect = 0;
    GF_ASSERT(++assert_effect == 1);
    CHECK(assert_effect == 1);
    unsigned ids[] = {1, 2, 3, 4};
    SysTaskQueue *queue = new_queue(3);
    SysTask *third = SysTaskQueue_InsertTask(queue, record, &ids[2], 20);
    SysTask *first = SysTaskQueue_InsertTask(queue, record, &ids[0], 10);
    SysTask *second = SysTaskQueue_InsertTask(queue, record, &ids[1], 10);
    CHECK(third && first && second && SysTask_GetPriority(third) == 20);
    CHECK(!SysTaskQueue_InsertTask(queue, record, &ids[3], 30));
    SysTaskQueue_RunTasks(queue);
    CHECK(count == 3 && events[0] == 1 && events[1] == 2 && events[2] == 3);
    CHECK(SysTask_Unlink(first) && SysTask_Unlink(second) && SysTask_Unlink(third));
    CHECK(queue->activeCount == 0);
    count = 0;
    for (unsigned i = 0; i < 10000; ++i) {
        CHECK(SysTaskQueue_InsertTask(queue, remove_self, &ids[0], 0));
        SysTaskQueue_RunTasks(queue);
        CHECK(queue->activeCount == 0 && count == 1 && events[0] == 1);
        count = 0;
    }
    CHECK(SysTaskQueue_InsertTask(queue, remove_next, &ids[0], 0));
    victim = SysTaskQueue_InsertTask(queue, record, &ids[1], 1);
    CHECK(victim);
    SysTaskQueue_RunTasks(queue);
    CHECK(count == 1 && events[0] == 1 && queue->activeCount == 0);
    count = 0;
    CHECK(SysTaskQueue_InsertTask(queue, create_child, &ids[0], 0));
    SysTaskQueue_RunTasks(queue);
    CHECK(count == 1 && events[0] == 1);
    SysTaskQueue_RunTasks(queue);
    CHECK(count == 2 && events[1] == 9);
    free(queue);
    puts("HGSS native scheduler: priority, FIFO, capacity, recycling, removal and deferred insertion passed");
    return 0;
}
