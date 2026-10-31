// Opens the real summary screen in a disposable ROM and captures the stats page of each party member.
#define TRAINING_UI_FIXTURE
#include "hardcore_smoke.c"
#undef main
#include <assert.h>
static struct mCore *core;
static void Frames(int count) { while (count--) core->runFrame(core); }
static void Key(unsigned key) { core->setKeys(core, key); Frames(4); core->setKeys(core, 0); Frames(40); }
int main(int argc, char **argv)
{
    assert(argc == 4);
    color_t *pixels;
    core = OpenTestCore(argv[1], &pixels); assert(core);
    static struct mLogger logger = {.log = TestLog};
    sLogCore = core; sLogPixels = pixels; sLogPrefix = argv[2]; mLogSetDefaultLogger(&logger);
    uint32_t addrCase = strtoul(argv[3], 0, 0);
    for (unsigned set = 0; set < 5; set++) {
        core->reset(core); Frames(400); core->busWrite32(core, addrCase, set + 1); Frames(300);
        Key(1u << 4); // info -> skills page
        for (unsigned slot = 0; slot < 5; slot++) {
            char label[32]; snprintf(label, sizeof label, "nature-%02u", set * 5 + slot);
            Shot(argv[2], label, pixels); Key(1u << 7);
        }
    }
    core->deinit(core); free(pixels); return 0;
}
