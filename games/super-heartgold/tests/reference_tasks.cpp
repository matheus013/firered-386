// Tests of the actual pinned HG C scheduler with a 64-bit host ABI.
extern "C" {
#include "sys_task.h"
}
#include <cstdlib>
#include <iostream>
#include <vector>

static_assert(sizeof(void*) == 8);
static_assert(sizeof(u32) == 4);

struct Record {
    std::vector<int>* calls;
    int id;
    bool remove;
};

void run_task(SysTask* task, void* data)
{
    auto* record = static_cast<Record*>(data);
    record->calls->push_back(record->id);
    if (record->remove) SysTask_Unlink(task);
}

int main()
{
    void* arena = std::calloc(1, SysTaskQueue_GetArenaSize(3));
    if (!arena) return 1;
    SysTaskQueue* queue = SysTaskQueue_PlacementNew(3, arena);
    std::vector<int> calls;
    Record low{&calls, 20, false}, high{&calls, 10, true}, equal{&calls, 21, false};
    SysTask* first = SysTaskQueue_InsertTask(queue, run_task, &low, 20);
    SysTaskQueue_InsertTask(queue, run_task, &high, 10);
    SysTaskQueue_InsertTask(queue, run_task, &equal, 20);
    if (SysTaskQueue_InsertTask(queue, run_task, &low, 99) != nullptr) return 2;
    if (SysTask_GetData(first) != &low || SysTask_GetPriority(first) != 20) return 3;
    SysTaskQueue_RunTasks(queue);
    if (calls != std::vector<int>({10, 20, 21}) || queue->activeCount != 2) return 4;
    calls.clear();
    SysTaskQueue_RunTasks(queue);
    if (calls != std::vector<int>({20, 21})) return 5;
    SysTask_Unlink(first);
    if (!SysTaskQueue_InsertTask(queue, run_task, &high, 1)) return 6;
    calls.clear();
    SysTaskQueue_RunTasks(queue);
    if (calls != std::vector<int>({10, 21})) return 7;
    std::free(arena);
    std::cout << "HG task C compiled natively for 64-bit host: priority, capacity, callbacks, unlink and reuse PASS\n";
    return 0;
}
