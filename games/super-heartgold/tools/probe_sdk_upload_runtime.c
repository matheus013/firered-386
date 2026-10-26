#include <nitro.h>
#include <nitro/hw/X86/mmap_global.h>
#include <nitro/hw/X86/mmap_vram.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

/* Headless platform globals; GX and MI implementations are compiled unchanged. */
u32 GXi_DmaId = GX_DMA_NOT_USE;
GXVRamTex s_SIM_GXVRamTex;
GXVRamTexPltt s_SIM_GXVRamTexPltt;
extern void WIN_Init_sTexStartAddrTable(void);
extern void WIN_Init_sTexPlttStartAddrTable(void);
extern void GX_InitGXState(void);
#define CHECK(x) do { if (!(x)) { fprintf(stderr, "check failed: %s:%d: %s\n", __FILE__, __LINE__, #x); exit(1); } } while (0)

void SIM_handleAssertionFailureMsg(const char *file, unsigned line, const char *fmt, ...)
{
    va_list args;
    fprintf(stderr, "%s:%u: ", file, line);
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    abort();
}

void OS_Terminate(void) { abort(); }
/* CPU-only test: entering asynchronous DMA must fail, never silently succeed. */
void OSi_EnterDmaCallback(u32 channel, void (*callback)(void *), void *arg) { abort(); }

static void check_texture_banks(void)
{
    static u32 payload[512 * 1024 / sizeof(u32)];
    for (unsigned i = 0; i < sizeof(payload) / sizeof(payload[0]); ++i)
        payload[i] = i ^ 0x71ac92e5;
    for (unsigned mask = 1; mask < 16; ++mask) {
        unsigned size = 0;
        for (unsigned bank = 0; bank < 4; ++bank)
            if (mask & (1u << bank)) size += 128 * 1024;
        for (unsigned direct = 0; direct < 2; ++direct) {
            memset((void *)HW_LCDC_VRAM_A, 0x5a, 512 * 1024);
            GX_SetBankForTex((GXVRamTex)mask);
            GX_BeginLoadTex();
            if (direct) GX_LoadTexEx((GXVRamTex)mask, payload, 0, size);
            else GX_LoadTex(payload, 0, size);
            GX_EndLoadTex();
            CHECK(GX_GetBankForTex() == mask);
            unsigned offset = 0;
            for (unsigned bank = 0; bank < 4; ++bank) {
                u8 *base = (u8 *)(HW_LCDC_VRAM_A + bank * 128 * 1024);
                if (mask & (1u << bank)) {
                    CHECK(memcmp(base, (u8 *)payload + offset, 128 * 1024) == 0);
                    offset += 128 * 1024;
                } else {
                    for (unsigned i = 0; i < 128 * 1024; ++i) CHECK(base[i] == 0x5a);
                }
            }
        }
    }
}

int main(void)
{
    u32 data[32];
    for (unsigned i = 0; i < 32; ++i) data[i] = 0x12340000 + i;
    GX_InitGXState();
    WIN_Init_sTexStartAddrTable();
    WIN_Init_sTexPlttStartAddrTable();
    check_texture_banks();
    GX_SetBankForTex(GX_VRAM_TEX_01_AC);
    GX_BeginLoadTex();
    CHECK(GX_GetBankForTex() == 0);
    GX_LoadTex(data, 128 * 1024 - 64, sizeof(data));
    CHECK(memcmp((void *)(HW_LCDC_VRAM_A + 128 * 1024 - 64), data, 64) == 0);
    CHECK(memcmp((void *)HW_LCDC_VRAM_C, (u8 *)data + 64, 64) == 0);
    GX_EndLoadTex();
    CHECK(GX_GetBankForTex() == GX_VRAM_TEX_01_AC);
    CHECK(s_SIM_GXVRamTex == GX_VRAM_TEX_01_AC);
    GX_SetBankForTexPltt(GX_VRAM_TEXPLTT_0123_E);
    GX_BeginLoadTexPltt();
    CHECK(GX_GetBankForTexPltt() == 0);
    GX_LoadTexPltt(data, 16, sizeof(data));
    CHECK(memcmp((void *)(HW_LCDC_VRAM_E + 16), data, sizeof(data)) == 0);
    GX_EndLoadTexPltt();
    CHECK(GX_GetBankForTexPltt() == GX_VRAM_TEXPLTT_0123_E);
    CHECK(s_SIM_GXVRamTexPltt == GX_VRAM_TEXPLTT_0123_E);
    puts("Real GX uploads: 15 texture bank combinations x 2 paths, split offset, palette and bank restoration passed");
    return 0;
}

