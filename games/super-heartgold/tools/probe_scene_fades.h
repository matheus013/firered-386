/* Original fade/task scheduler integration in a diagnostic host loop. */
#include "screen_fade.h"
#include "system.h"
void *Heap_AllocAtEnd(enum HeapID id, u32 size) { return Heap_Alloc(id, size); }
static void prepare_fades(void) {
    OS_DisableIrq();
    if (OS_EnableIrq() != FALSE || reg_OS_IME != OS_IME_ENABLE) abort();
    if (OS_DisableIrq() != TRUE || reg_OS_IME != OS_IME_DISABLE) abort();
    OS_RestoreIrq(TRUE);
    SysTaskQueue **queues[] = {&gSystem.mainTaskQueue, &gSystem.vblankTaskQueue,
                              &gSystem.vwaitTaskQueue, &gSystem.printTaskQueue};
    for (unsigned i=0; i<4; ++i) {
        void *memory = Heap_Alloc(0, SysTaskQueue_GetArenaSize(32));
        *queues[i] = SysTaskQueue_PlacementNew(32, memory);
        if (!*queues[i]) abort();
    }
    SetMasterBrightnessByColor(PM_LCD_TOP, GX_RGB(0,0,0));
    SetMasterBrightnessByColor(PM_LCD_BOTTOM, GX_RGB(0,0,0));
}
static void update_fades(unsigned frame) {
    if (frame == 1 || frame == 40 || frame == 80 || frame == 120) {
        if (!IsPaletteFadeFinished()) abort();
        enum FadeType type = (frame == 1 || frame == 80) ? FADE_TYPE_UNK_9 : FADE_TYPE_UNK_8;
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, type, type, GX_RGB(0,0,0), 10, 1, 0);
        printf("FADE start frame=%u type=%d\n", frame, type); fflush(stdout);
    }
    HandleFadeUpdateFrame();
    if (frame == 4 || frame == 8) {
        printf("FADE IRQ frame=%u callback=%d ime=%u ie=%u stat=%u win=%u/%u\n", frame,
            gSystem.hBlankIntr != NULL, (unsigned)reg_OS_IME, (unsigned)reg_OS_IE,
            (unsigned)reg_GX_DISPSTAT, (unsigned)reg_G2_WININ, (unsigned)reg_G2_WINOUT);
        fflush(stdout);
    }
    SysTaskQueue_RunTasks(gSystem.mainTaskQueue);
    SysTaskQueue_RunTasks(gSystem.printTaskQueue);
    if (frame == 30 || frame == 70 || frame == 110 || frame == 150) {
        if (!IsPaletteFadeFinished()) abort();
        printf("FADE completed frame=%u\n", frame); fflush(stdout);
    }
}
