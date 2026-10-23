// Drives one native wild battle per case: reset, select the case, read the state after turn 1.
#define TRAINING_UI_FIXTURE
#include "hardcore_smoke.c"
#undef main
static struct mCore *core;
int main(int argc, char **argv)
{
    color_t *pixels;
    unsigned caseCount, i, j, frame;
    uint32_t addrCase, addrDone, addrOut;
    if (argc != 6) return 2;
    core = OpenTestCore(argv[1], &pixels);
    if (!core) return 3;
    static struct mLogger logger = {.log = SmokeLog};
    mLogSetDefaultLogger(&logger);
    caseCount = strtoul(argv[2], 0, 0);
    addrCase = strtoul(argv[3], 0, 0);
    addrDone = strtoul(argv[4], 0, 0);
    addrOut = strtoul(argv[5], 0, 0);
    for (i = 1; i <= caseCount; i++)
    {
        core->reset(core);
        for (frame = 0; frame < 600; frame++) core->runFrame(core);
        core->busWrite32(core, addrCase, i);
        // Battle messages wait for A; actions themselves are chosen by the patched controller.
        for (frame = 0; frame < 20000 && core->busRead32(core, addrDone) != i; frame++)
        {
            core->setKeys(core, (frame % 16) < 2 ? 1 : 0);
            core->runFrame(core);
        }
        core->setKeys(core, 0);
        printf("case=%u done=%u frames=%u out=", i, core->busRead32(core, addrDone), frame);
        for (j = 0; j < 16; j++) printf("%u%c", core->busRead32(core, addrOut + 4 * j), j == 15 ? '\n' : ',');
        fflush(stdout);
    }
    core->deinit(core);
    free(pixels);
    return 0;
}
