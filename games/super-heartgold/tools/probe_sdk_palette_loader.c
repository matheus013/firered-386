#include <nitro.h>
#include <nnsys/g2d/load/g2d_NCL_load.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#define CHECK(x) do { if (!(x)) { fprintf(stderr,"check failed line %d\n",__LINE__); exit(42); } } while (0)
static u32 le32(const u8 *p) { return p[0] | ((u32)p[1]<<8) | ((u32)p[2]<<16) | ((u32)p[3]<<24); }
int main(int argc, char **argv) {
    for (int i=1;i<argc;++i) {
        FILE *file=fopen(argv[i],"rb"); CHECK(file);
        CHECK(fseek(file,0,SEEK_END)==0);
        long length=ftell(file); CHECK(length>=40 && length<1024*1024);
        rewind(file);
        u8 *data=malloc(length), *before=malloc(length); CHECK(data && before);
        CHECK(fread(data,1,length,file)==(size_t)length); fclose(file);
        memcpy(before,data,length);
        /* Verified fixture: first block is PLTT, on-disk descriptor starts at 24. */
        CHECK(memcmp(data+16,"TTLP",4)==0);
        u32 size=le32(data+32), offset=le32(data+36);
        CHECK(offset <= (u32)length-24 && size <= (u32)length-24-offset);
        NNSG2dPaletteData *palette=NULL;
        CHECK(NNS_G2dGetUnpackedPaletteData(data,&palette));
        CHECK(palette && palette->szByte==size);
        CHECK((u32)palette->fmt==le32(before+24));
        CHECK(memcmp(palette->pRawData,before+24+offset,size)==0);
        CHECK(memcmp(data,before,length)==0);
        free(before); free(data);
    }
    printf("%d original scene palettes decoded without changing input bytes\n",argc-1);
}
