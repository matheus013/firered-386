// Runs the real caught-Pokemon battle script in a disposable ROM and dumps the held-item message.
#define TRAINING_UI_FIXTURE
#include "hardcore_smoke.c"
#undef main
#include <assert.h>
int main(int argc, char **argv)
{
    assert(argc == 6);
    color_t *pixels;
    struct mCore *core = OpenTestCore(argv[1], &pixels); assert(core);
    static struct mLogger logger = {.log = TestLog};
    sLogCore = core; sLogPixels = pixels; sLogPrefix = argv[2]; mLogSetDefaultLogger(&logger);
    uint32_t addrCase = strtoul(argv[3], 0, 0), addrStage = strtoul(argv[4], 0, 0), addrText = strtoul(argv[5], 0, 0);
    for (unsigned test = 1; test <= 3; test++) {
        core->reset(core); for (int i = 0; i < 400; i++) core->runFrame(core);
        core->busWrite32(core, addrCase, test);
        unsigned stage = 0, shot = 0;
        for (unsigned f = 0; f < 4000 && stage < 3; f++) {
            core->setKeys(core, f % 30 < 4 ? 1 : 0); // A advances each battle message
            core->runFrame(core);
            stage = core->busRead32(core, addrStage);
            if (stage == 2 && !shot && ++shot) {
                for (int i = 0; i < 40; i++) core->runFrame(core);
                char label[32]; snprintf(label, sizeof label, "case%u-held", test); Shot(argv[2], label, pixels);
            }
        }
        if (stage < 3) { char label[32]; snprintf(label, sizeof label, "case%u-stuck", test); Shot(argv[2], label, pixels); }
        printf("case=%u stage=%u text=", test, stage);
        for (unsigned i = 0; i < 64; i++) printf("%02x", core->busRead8(core, addrText + i));
        printf("\n");
    }
    core->deinit(core); free(pixels); return 0;
}
