// Uses the existing PNG writer; test ROMs only, never user save files.
#define main BootSmokeMain
#include "smoke.c"
#undef main
#include <mgba/core/log.h>

static struct mCore *sLogCore;
static color_t *sLogPixels;
static const char *sLogPrefix;
static int sLogFrame;
static uint32_t sControllers, sActive, sBattlers;
static void TestLog(struct mLogger *logger, int category, enum mLogLevel level, const char *format, va_list args)
{
    (void)logger; (void)category; (void)level;
    if (strstr(format, "Illegal opcode") || strstr(format, "Jumped to invalid")) {
        vfprintf(stderr, format, args); fputc('\n', stderr);
        uint32_t pc = 0, lr = 0, sp = 0;
        sLogCore->readRegister(sLogCore, "pc", &pc);
        sLogCore->readRegister(sLogCore, "lr", &lr);
        sLogCore->readRegister(sLogCore, "sp", &sp);
        fprintf(stderr, "frame=%d pc=%08x lr=%08x sp=%08x\n", sLogFrame, pc, lr, sp);
        if (sControllers) {
            fprintf(stderr, "active=%u count=%u\n", sLogCore->busRead8(sLogCore, sActive), sLogCore->busRead8(sLogCore, sBattlers));
            for (int i = 0; i < 4; ++i) fprintf(stderr, "controller%d=%08x\n", i, sLogCore->busRead32(sLogCore, sControllers + 4 * i));
        }
        char path[4096]; snprintf(path, sizeof(path), "%s-invalid-code.png", sLogPrefix);
        WriteImage(path, sLogPixels, 240, 160);
        exit(13);
    }
}

static struct mCore *OpenTestCore(const char *rom, color_t **pixels)
{
    struct mCore *core = mCoreFind(rom);
    if (!core || !core->init(core)) return NULL;
    mCoreInitConfig(core, "hardcore-regression");
    if (!mCoreLoadFile(core, rom)) return NULL;
    *pixels = calloc(240 * 160, sizeof(**pixels));
    core->setVideoBuffer(core, *pixels, 240);
    core->setAudioBufferSize(core, 1024);
    return core;
}
static int TaskExists(struct mCore *core, uint32_t tasks, uint32_t function)
{
    for (int i = 0; i < 16; ++i)
        if (core->busRead8(core, tasks + i * 40 + 4)
            && (core->busRead32(core, tasks + i * 40) & ~1u) == (function & ~1u)) return 1;
    return 0;
}
static void Shot(const char *prefix, const char *stage, color_t *pixels)
{
    char path[4096];
    snprintf(path, sizeof(path), "%s-%s.png", prefix, stage);
    if (!WriteImage(path, pixels, 240, 160)) exit(20);
}
int main(int argc, char **argv)
{
    if (argc < 5) return 2;
    color_t *pixels;
    struct mCore *core = OpenTestCore(argv[1], &pixels);
    if (!core) return 3;
    static struct mLogger logger = {.log = TestLog};
    sLogCore = core; sLogPixels = pixels; sLogPrefix = argv[2];
    mLogSetDefaultLogger(&logger);
    uint32_t address = strtoul(argv[4], NULL, 0);
    core->reset(core);
    if (!strcmp(argv[3], "rules")) {
        for (int phase = 1; phase <= 2; ++phase) {
            uint32_t result = 0;
            for (int frame = 0; frame < 1800 && (!result || result >= 100); ++frame) {
                core->runFrame(core);
                result = core->busRead32(core, address);
            }
            printf("rules phase=%d result=%u expected=%d\n", phase, result, phase);
            if (result != (uint32_t)phase) return 4;
            if (phase == 1) {
                void *data = NULL;
                size_t size = core->savedataClone(core, &data);
                printf("save_bytes=%zu data_present=%d\n", size, data != NULL);
                if (size != 131072 || !data) return 5;
                char path[4096]; snprintf(path, sizeof(path), "%s-test-only.sav", argv[2]);
                FILE *file = fopen(path, "wb");
                if (!file || fwrite(data, 1, size, file) != size) return 6;
                fclose(file);
                core->deinit(core); free(pixels);
                core = OpenTestCore(argv[1], &pixels);
                sLogCore = core; sLogPixels = pixels;
                if (!core || !core->savedataRestore(core, data, size, false)) return 7;
                free(data); core->reset(core);
            }
        }
    } else if (!strcmp(argv[3], "gameover") || !strcmp(argv[3], "battle-loss")) {
        int battle = !strcmp(argv[3], "battle-loss");
        if (battle) {
            sControllers = strtoul(argv[6], NULL, 0); sActive = strtoul(argv[7], NULL, 0); sBattlers = strtoul(argv[8], NULL, 0);
        }
        for (int frame = 0; frame < (battle ? 3600 : 180); ++frame) {
            sLogFrame = frame;
            uint32_t save = core->busRead32(core, address);
            if (frame > 180 && core->busRead8(core, save + 0xB26)) break;
            core->setKeys(core, battle && frame % 20 == 0 ? 1 : 0);
            core->runFrame(core);
        }
        core->setKeys(core, 0);
        for (int frame = 0; frame < 30; ++frame) core->runFrame(core);
        uint32_t save = core->busRead32(core, address);
        if (core->busRead8(core, save + 0xB26) != 1) return 8;
        if (battle) {
            uint32_t party = strtoul(argv[5], NULL, 0);
            if (!(core->busRead8(core, party + 19) & 0x10) || core->busRead16(core, party + 86)) return 12;
            puts("PASS: actual double battle faint permanently marks the player mon and reaches the game-over screen.");
        }
        Shot(argv[2], "screen", pixels);
        core->setKeys(core, 1); core->runFrame(core); core->setKeys(core, 0);
        for (int frame = 0; frame < 120; ++frame) core->runFrame(core);
        Shot(argv[2], "menu", pixels);
    } else {
        if (argc != 10) return 2;
        uint32_t savePtr = strtoul(argv[5], NULL, 0);
        uint32_t choose = strtoul(argv[6], NULL, 0);
        uint32_t review = strtoul(argv[7], NULL, 0);
        uint32_t controls = strtoul(argv[8], NULL, 0);
        uint32_t mainAddress = strtoul(argv[9], NULL, 0);
        int hardcore = !strcmp(argv[3], "hardcore");
        int stage = 0, nextInput = 180, completed = 0;
        for (int frame = 0; frame < 1800; ++frame) {
            uint32_t save = core->busRead32(core, savePtr);
            core->setKeys(core, 0);
            if (frame > nextInput) {
                if (stage == 0 && TaskExists(core, address, choose)) {
                    Shot(argv[2], "choices", pixels);
                    if (hardcore) core->setKeys(core, 1u << 7);
                    stage = 1; nextInput = frame + 15;
                } else if ((stage == 1 || stage == 5) && TaskExists(core, address, choose)) {
                    core->setKeys(core, 1); ++stage; nextInput = frame + 20;
                } else if (stage == 2 && TaskExists(core, address, review)) {
                    Shot(argv[2], "review", pixels);
                    // Selection must not be committed before confirmation.
                    if (core->busRead32(core, save + 0xB20) == 0x31435A4E) return 9;
                    core->setKeys(core, 2); stage = 3; nextInput = frame + 20;
                } else if (stage == 3 && TaskExists(core, address, choose)) {
                    if (hardcore) core->setKeys(core, 1u << 7);
                    stage = 5; nextInput = frame + 15;
                } else if (stage == 6 && TaskExists(core, address, review)) {
                    core->setKeys(core, 1); stage = 7; nextInput = frame + 20;
                } else if (stage == 7 && TaskExists(core, address, controls)) {
                    if (core->busRead32(core, save + 0xB20) != 0x31435A4E
                        || core->busRead8(core, save + 0xB24) != (uint32_t)hardcore) return 10;
                    Shot(argv[2], "confirmed", pixels); completed = 1; break;
                }
            }
            core->runFrame(core);
            if (frame < 240 && frame % 60 == 0) printf("frame=%d callback=%08x state=%u task0=%08x active=%u\n", frame,
                core->busRead32(core, mainAddress + 4), core->busRead8(core, mainAddress + 0x438),
                core->busRead32(core, address), core->busRead8(core, address + 4));
        }
        printf("menu hardcore=%d stage=%d completed=%d\n", hardcore, stage, completed);
        if (!completed) {
            Shot(argv[2], "failure", pixels);
            uint32_t pc = 0; core->readRegister(core, "pc", &pc); printf("pc=%08x\n", pc);
            for (int i = 0; i < 16; ++i)
                if (core->busRead8(core, address + 40 * i + 4)) printf("task=%d func=%08x\n", i, core->busRead32(core, address + 40 * i));
            return 11;
        }
    }
    core->deinit(core); free(pixels);
    puts("PASS");
    return 0;
}
