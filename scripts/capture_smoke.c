#define TRAINING_UI_FIXTURE
#include "hardcore_smoke.c"
#undef main
#include <assert.h>
int main(int argc, char **argv)
{
    assert(argc == 7);
    color_t *pixels;
    struct mCore *core = OpenTestCore(argv[1], &pixels);
    assert(core);
    uint32_t done = strtoul(argv[3], 0, 0), passed = strtoul(argv[4], 0, 0), failed = strtoul(argv[5], 0, 0);
    uint32_t passMask = 0, failMask = 0;
    uint32_t observed[7], observedAddress = strtoul(argv[6], 0, 0);
    static struct mLogger logger = {.log = TestLog};
    sLogPrefix = argv[2];
    mLogSetDefaultLogger(&logger);
    core->reset(core);
    for (int phase = 1; phase <= 2; phase++) {
        sLogCore = core; sLogPixels = pixels;
        for (int f = 0; f < 10000 && !core->busRead32(core, done); f++) {
            sLogFrame = f;
            core->runFrame(core);
        }
        if (core->busRead32(core, done) != (uint32_t)phase) return 2;
        passMask |= core->busRead32(core, passed);
        failMask |= core->busRead32(core, failed);
        if (phase == 1) {
            for (int i = 0; i < 7; i++) observed[i] = core->busRead32(core, observedAddress + 4*i);
            void *save = NULL;
            size_t size = core->savedataClone(core, &save);
            if (!save || size != 131072) return 3;
            core->deinit(core); free(pixels);
            core = OpenTestCore(argv[1], &pixels);
            if (!core || !core->savedataRestore(core, save, size, false)) return 4;
            free(save); core->reset(core);
        }
    }
    printf("{\"passed\":%u,\"failed\":%u,\"save_bytes\":131072,\"fresh_instances\":2,\"observed\":[", passMask, failMask);
    for (int i = 0; i < 7; i++) printf("%s%u", i ? "," : "", observed[i]);
    puts("]}");
    core->deinit(core); free(pixels);
    return 0;
}
