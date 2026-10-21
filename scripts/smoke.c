// Headless boot check using the installed mGBA library. Does not validate gameplay.
#include <mgba/flags.h>
#include <mgba/core/core.h>
#include <png.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int WriteImage(const char *path, const color_t *pixels, unsigned width, unsigned height)
{
    FILE *file = fopen(path, "wb");
    if (!file) return 0;
    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop info = png ? png_create_info_struct(png) : NULL;
    if (!png || !info) { fclose(file); return 0; }
    if (setjmp(png_jmpbuf(png))) {
        png_destroy_write_struct(&png, &info);
        fclose(file);
        return 0;
    }
    png_init_io(png, file);
    png_set_IHDR(png, info, width, height, 8, PNG_COLOR_TYPE_RGB,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    png_write_info(png, info);
    unsigned char *row = malloc(width * 3);
    if (!row) { png_destroy_write_struct(&png, &info); fclose(file); return 0; }
    for (unsigned y = 0; y < height; ++y) {
        for (unsigned x = 0; x < width; ++x) {
            uint32_t color = pixels[y * width + x];
            row[x * 3] = color & 255;
            row[x * 3 + 1] = (color >> 8) & 255;
            row[x * 3 + 2] = (color >> 16) & 255;
        }
        png_write_row(png, row);
    }
    free(row);
    png_write_end(png, info);
    png_destroy_write_struct(&png, &info);
    fclose(file);
    return 1;
}

int main(int argc, char **argv)
{
    if (argc < 3) { fprintf(stderr, "Usage: smoke ROM screenshot.png [frames [battle mainFuncAddr readyFuncAddr flagsAddr absentAddr countAddr]]\n"); return 2; }
    unsigned limit = argc > 3 ? strtoul(argv[3], NULL, 0) : 900;
    int battle = argc >= 10 && strcmp(argv[4], "battle") == 0;
    int ready = 0;
    struct mCore *core = mCoreFind(argv[1]);
    if (!core || !core->init(core)) return 1;
    mCoreInitConfig(core, "firered-smoke");
    if (!mCoreLoadFile(core, argv[1])) { core->deinit(core); return 1; }
    unsigned width, height;
    core->desiredVideoDimensions(core, &width, &height);
    color_t *pixels = calloc(width * height, sizeof(*pixels));
    if (!pixels) { core->deinit(core); return 1; }
    core->setVideoBuffer(core, pixels, width);
    core->setAudioBufferSize(core, 1024);
    core->reset(core);
    for (unsigned frame = 0; frame < limit; ++frame) {
        // Repeated START presses dismiss the intro and reach the title/new-game flow.
        if (battle) {
            uint32_t callback = core->busRead32(core, strtoul(argv[5], NULL, 0));
            if ((callback & ~1u) == (strtoul(argv[6], NULL, 0) & ~1u)) ready = 1;
            core->setKeys(core, !ready && frame % 30 == 0 ? 1 : 0);
        } else {
            core->setKeys(core, frame > 300 && frame % 90 == 0 ? (1u << 3) : 0);
        }
        core->runFrame(core);
    }
    uint32_t frames = core->frameCounter(core);
    int varied = 0;
    for (unsigned i = 1; i < width * height; ++i)
        if (pixels[i] != pixels[0]) { varied = 1; break; }
    int success = frames >= limit && varied && (!battle || ready) && WriteImage(argv[2], pixels, width, height);
    printf("frames=%u dimensions=%ux%u nonuniform_image=%d result=%s\n",
           frames, width, height, varied, success ? "PASS" : "FAIL");
    if (battle) {
        printf("battle_ready=%d battle_flags=%u absent=%u battlers=%u\n", ready,
               core->busRead32(core, strtoul(argv[7], NULL, 0)),
               core->busRead8(core, strtoul(argv[8], NULL, 0)),
               core->busRead8(core, strtoul(argv[9], NULL, 0)));
    }
    core->deinit(core);
    free(pixels);
    return success ? 0 : 1;
}
