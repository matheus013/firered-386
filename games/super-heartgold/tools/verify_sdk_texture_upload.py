"""Execute original/adapted direct upload functions with bounded copy hooks."""
import argparse
import json
import subprocess
from pathlib import Path
from prepare_sdk_texture_upload import prepare

PRELUDE = r'''
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
typedef uint8_t u8;
typedef uint32_t u32;
typedef uint64_t u64;
typedef unsigned GXVRamTex;
typedef unsigned GXVRamTexPltt;
#define SDK_PORT 1
#define CHECK(x) do { if (!(x)) exit(42); } while (0)
#define SDK_ALIGN4_ASSERT(x) CHECK(((uintptr_t)(x) & 3) == 0)
#define SDK_ASSERT(x) CHECK(x)
#define SDK_ASSERTMSG(x, msg) CHECK(x)
#define SDK_NULL_ASSERT(x) CHECK(x)
#define GX_VRAM_TEX_ASSERT(x) CHECK(x == 1)
#define GX_VRAM_TEXPLTT_ASSERT(x) CHECK(x == 16)
static unsigned char bank1[8192], bank2[8192], palette[8192];
static struct { u64 blk1, blk2; unsigned szBlk1; } sTexStartAddrTable[16];
static u64 sTexPlttStartAddrTable[16];
static unsigned GXi_DmaId;
static unsigned GX_GetBankForLCDC(void) { return 17; }
static void GX_RegionCheck_Tex(unsigned bank, u32 first, u32 last) { CHECK(first <= last && last <= 8192); }
static void GX_RegionCheck_TexPltt(unsigned bank, u32 first, u32 last) { CHECK(first <= last && last <= 8192); }
static int within(void *p, u32 n, unsigned char *base) {
    uintptr_t a = (uintptr_t)p, b = (uintptr_t)base;
    return a >= b && a - b <= 8192 && n <= 8192 - (a - b);
}
static void GXi_DmaCopy32(unsigned id, const void *s, void *d, u32 n) {
    CHECK(within(d,n,bank1) || within(d,n,bank2) || within(d,n,palette));
    memcpy(d,s,n);
}
static void GXi_DmaCopy32Async(unsigned id, const void *s, void *d, u32 n, void *cb, void *arg) {
    GXi_DmaCopy32(id,s,d,n);
}
'''
MAIN = r'''
int main(void) {
    u32 data[32];
    for (unsigned i=0;i<32;++i) data[i]=0x12340000+i;
    CHECK((uintptr_t)bank1 > UINT32_MAX);
    sTexStartAddrTable[1].blk1=(uintptr_t)bank1;
    GX_LoadTexEx(1,data,16,sizeof(data));
    CHECK(memcmp(bank1+16,data,sizeof(data))==0);
    sTexStartAddrTable[1].blk2=(uintptr_t)bank2;
    sTexStartAddrTable[1].szBlk1=1;
    GX_LoadTexEx(1,data,4096-64,sizeof(data));
    CHECK(memcmp(bank1+4096-64,data,64)==0);
    CHECK(memcmp(bank2,(u8*)data+64,64)==0);
    GX_LoadTexEx(1,data,4096+32,sizeof(data));
    CHECK(memcmp(bank2+32,data,sizeof(data))==0);
    sTexPlttStartAddrTable[1]=(uintptr_t)palette;
    GX_LoadTexPlttEx(16,data,16,sizeof(data));
    CHECK(memcmp(palette+16,data,sizeof(data))==0);
    puts("SDK direct uploads: single bank, split bank, second bank and palette passed");
}
'''


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source', type=Path, required=True)
    p.add_argument('--output', type=Path, required=True)
    a = p.parse_args()
    a.output.mkdir(parents=True, exist_ok=False)
    adapted = a.output / 'gx_load3d-native.c'
    prepare(a.source, adapted)
    for label, source in [('original', a.source), ('adapted', adapted)]:
        text = source.read_text()
        functions = text[text.index('void GX_LoadTexEx ('):text.index('void GX_BeginLoadTex (')]
        functions += text[text.index('void GX_LoadTexPlttEx ('):text.index('void GX_BeginLoadTexPltt (')]
        unit, binary = a.output / (label+'.c'), a.output / label
        unit.write_text(PRELUDE + functions + MAIN)
        subprocess.run(['gcc','-g','-fsanitize=address,undefined','-fno-sanitize-recover=all',str(unit),'-o',str(binary)],check=True)
        result = subprocess.run([str(binary)],capture_output=True,text=True)
        (a.output/(label+'.log')).write_text(result.stdout+result.stderr)
        if result.returncode != (42 if label=='original' else 0):
            raise RuntimeError(label+' returned unexpected status: '+str(result.returncode))
    (a.output/'result.json').write_text(json.dumps({'passed':True,'original_rejected_bad_address':True,
        'adapted_cases':4,'sanitizers':['address','undefined'],
        'scope':'two extracted SDK functions, controlled bank state and copy hooks',
        'gpu_tested':False},indent=2)+'\n')
    print('Original bad addresses reproduced; all four adapted cases passed')


if __name__ == '__main__': main()
