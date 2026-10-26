/* Deterministic register-boundary test, not a real-time clock accuracy test. */
#include <stdio.h>
#include <stdlib.h>
#include "global.h"
#include "timer_3.h"
#define CHECK(c) do { if (!(c)) { fprintf(stderr, "Timer check failed: %s\n", #c); abort(); } } while (0)
REGType16v s_reg_OS_TM3CNT_L;
REGType32v s_reg_OS_IF;
extern vu64 gTimer3Overflows;
static unsigned critical_depth;
OSIntrMode OS_DisableInterrupts(void) {
    CHECK(critical_depth == 0);
    critical_depth++;
    return 0;
}
OSIntrMode OS_RestoreInterrupts(OSIntrMode previous) {
    CHECK(critical_depth == 1 && previous == 0);
    critical_depth--;
    return 0;
}
void GF_AssertFail(void) { abort(); }
int main(void) {
    for (unsigned pending = 0; pending < 2; ++pending) {
        for (unsigned low = 0; low < 65536; ++low) {
            gTimer3Overflows = 0x123456789ULL;
            s_reg_OS_TM3CNT_L = low;
            s_reg_OS_IF = pending ? OS_IE_TIMER3 : 0;
            u64 expected = 0x1234567890000ULL + low;
            if (pending && low < 32768) expected += 65536;
            CHECK(GetTimer3Ticks() == expected);
            CHECK(critical_depth == 0);
        }
    }
    CHECK(Timer3TicksToSeconds(0) == 0);
    CHECK(Timer3TicksToSeconds(OS_SYSTEM_CLOCK) == 64);
    puts("HGSS Timer3 native register: 131072 samples and interrupt restoration passed");
    return 0;
}
