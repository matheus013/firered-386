// Runs region pilot cases; the ROM hook chooses the held keys and asks for screenshots.
#define TRAINING_UI_FIXTURE
#include "hardcore_smoke.c"
#undef main
#include <assert.h>
int main(int argc, char **argv)
{
    assert(argc == 9 || argc == 11);
    color_t *pixels;
    struct mCore *core = OpenTestCore(argv[1], &pixels); assert(core);
    static struct mLogger logger = {.log = TestLog};
    sLogCore = core; sLogPixels = pixels; sLogPrefix = argv[2]; mLogSetDefaultLogger(&logger);
    unsigned cases = strtoul(argv[3], 0, 0);
    uint32_t addrCase = strtoul(argv[4], 0, 0), addrStage = strtoul(argv[5], 0, 0), addrResult = strtoul(argv[6], 0, 0);
    uint32_t addrKeys = strtoul(argv[7], 0, 0), addrShot = strtoul(argv[8], 0, 0);
    unsigned first = argc == 11 ? strtoul(argv[9], 0, 0) : 1;
    unsigned stride = argc == 11 ? strtoul(argv[10], 0, 0) : 1;
    assert(first >= 1 && first <= cases && stride >= 1);
    for (unsigned test = first; test <= cases; test += stride) {
        core->reset(core); for (int i = 0; i < 400; i++) core->runFrame(core);
        core->busWrite32(core, addrCase, test);
        unsigned stage = 0;
        for (unsigned f = 0; f < 200000 && stage < 3; f++) {
            core->setKeys(core, core->busRead16(core, addrKeys));
            core->runFrame(core);
            unsigned shot = core->busRead32(core, addrShot);
            if (shot) {
                char label[32]; snprintf(label, sizeof label, "case%u-step%u", test, shot - 1); Shot(argv[2], label, pixels);
                core->busWrite32(core, addrShot, 0);
            }
            stage = core->busRead32(core, addrStage);
        }
        if (stage < 3) { char label[32]; snprintf(label, sizeof label, "case%u-stuck", test); Shot(argv[2], label, pixels); }
        printf("case=%u stage=%u text=", test, stage);
        for (unsigned i = 0; i < 128; i++) printf("%02x", core->busRead8(core, addrResult + i));
        printf("\n");
        fflush(stdout);
    }
    core->deinit(core); free(pixels); return 0;
}
