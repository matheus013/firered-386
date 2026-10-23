// Boots a disposable ROM whose main callback runs native checks, then reports the counters.
#define TRAINING_UI_FIXTURE
#include "hardcore_smoke.c"
#undef main
int main(int argc, char **argv)
{
    color_t *pixels;
    unsigned frame, done, passed, count, failed, first;
    if (argc != 7) return 2;
    struct mCore *core = OpenTestCore(argv[1], &pixels);
    if (!core) return 3;
    static struct mLogger logger = {.log = SmokeLog};
    mLogSetDefaultLogger(&logger);
    core->reset(core);
    for (frame = 0; frame < 3000 && !core->busRead32(core, strtoul(argv[2], 0, 0)); frame++) core->runFrame(core);
    done = core->busRead32(core, strtoul(argv[2], 0, 0));
    passed = core->busRead32(core, strtoul(argv[3], 0, 0));
    count = core->busRead32(core, strtoul(argv[4], 0, 0));
    failed = core->busRead32(core, strtoul(argv[5], 0, 0));
    first = core->busRead32(core, strtoul(argv[6], 0, 0));
    printf("done=%u passed=%u checks=%u failed=%u first_failure=%u frames=%u\n", done, passed, count, failed, first, frame);
    core->deinit(core);
    free(pixels);
    return done == 1 && failed == 0 && passed == count ? 0 : 1;
}
