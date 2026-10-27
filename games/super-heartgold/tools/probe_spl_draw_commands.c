/* Include the complete private unit so its three static helpers can be tested. */
#include "spl_draw_ptcl.c"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

static draw_msg_t commands[32];
static unsigned count;
#define CHECK(x) do { if (!(x)) { fprintf(stderr, "failed line %d: %s\n", __LINE__, #x); exit(42); } } while (0)
void SIM_HandleG3Command(draw_msg_t *message) {
    CHECK(count < 32);
    commands[count++] = *message;
}
void SIM_handleAssertionFailureMsg(const char *file, unsigned line, const char *fmt, ...) {
    fprintf(stderr, "%s:%u\n", file, line);
    abort();
}
int main(void) {
    for (unsigned plane = 0; plane < 2; ++plane) {
        count = 0;
        if (plane) drawXZPlane(8192, 12288, 0, 0);
        else drawXYPlane(8192, 12288, 0, 0);
        /* Native SDK completes quads after four vertices; End has no message. */
        CHECK(count == 9);
        CHECK(commands[0].type == DRAW_CMD_G3_BEGIN);
        CHECK(commands[0].data.numU16 == GX_BEGIN_QUADS);
        const u32 tex[] = {0, 32, 32 | (48u << 16), 48u << 16};
        const int x[] = {-4096, 4096, 4096, -4096};
        const int yz[] = {4096, 4096, -4096, -4096};
        for (unsigned v = 0; v < 4; ++v) {
            CHECK(commands[1 + v * 2].type == DRAW_CMD_G3_TEXCOORD);
            CHECK(commands[1 + v * 2].data.numU32 == tex[v]);
            draw_msg_t *vertex = &commands[2 + v * 2];
            CHECK(vertex->type == DRAW_CMD_G3_VTX16);
            CHECK(vertex->data.xyz.x == x[v]);
            CHECK(vertex->data.xyz.y == (plane ? 0 : yz[v]));
            CHECK(vertex->data.xyz.z == (plane ? yz[v] : 0));
        }
    }
    /* Exhaustive signed-16 range: match DS VTX10 rounding, not VTX16 precision. */
    for (int coordinate = -32768; coordinate <= 32767; ++coordinate) {
        count = 0;
        Vtx10(coordinate, coordinate, coordinate);
        int rounded = coordinate / 64;
        if (coordinate < 0 && coordinate % 64) --rounded;
        CHECK(count == 1);
        CHECK(commands[0].data.xyz.x == rounded * 64);
        CHECK(commands[0].data.xyz.y == rounded * 64);
        CHECK(commands[0].data.xyz.z == rounded * 64);
    }
    count = 0;
    MtxScale(-4096, 8192, 16384);
    CHECK(count == 1 && commands[0].type == DRAW_CMD_G3_SCALE);
    CHECK(commands[0].data.scale[0] == -4096);
    CHECK(commands[0].data.scale[1] == 8192);
    CHECK(commands[0].data.scale[2] == 16384);
#ifdef TEST_ADAPTED_SPL
    count = 0;
    SplNativePolygonAttr(0x1f00c0);
    CHECK(count == 1 && commands[0].type == DRAW_CMD_G3_POLYGONATTR);
    CHECK(commands[0].data.numU32 == 0x1f00c0);
#endif
    puts("SPL drawing commands: XY/XZ planes, 65536 signed coordinates, scale and polygon attributes passed");
    return 0;
}
