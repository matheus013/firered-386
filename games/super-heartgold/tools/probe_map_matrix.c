/* Original map loader with explicit private-file and map-header boundaries. */
#include "map_matrix.h"
#include "filesystem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(x) do { if (!(x)) { fprintf(stderr,"check line %d\n",__LINE__); abort(); } } while(0)
static const char *input;
static u16 selected;
static unsigned allocations;
void GF_AssertFail(void) { abort(); }
void SIM_handleAssertionFailureMsg(const char *file,unsigned line,const char *fmt,...) {
    (void)fmt; fprintf(stderr,"assert %s:%u\n",file,line); abort();
}
void *Heap_Alloc(enum HeapID heap,u32 size) {
    (void)heap; void *p=malloc(size); CHECK(p); memset(p,0xA5,size); ++allocations; return p;
}
void Heap_Free(void *p) { CHECK(p && allocations); --allocations; free(p); }
u16 MapHeader_GetMatrixId(u32 map) { CHECK(map==64); return selected; }
void *AllocAtEndAndReadWholeNarcMemberByIdPair(NarcId narc,s32 member,enum HeapID heap) {
    CHECK(narc==NARC_fielddata_mapmatrix_map_matrix && member==selected);
    FILE *f=fopen(input,"rb"); CHECK(f); CHECK(!fseek(f,0,SEEK_END));
    long size=ftell(f); CHECK(size>0 && size<65536); rewind(f);
    void *p=Heap_Alloc(heap,size); CHECK(fread(p,1,size,f)==(size_t)size); fclose(f); return p;
}
static void word(FILE *f,unsigned value) { fputc(value&255,f); fputc(value>>8,f); }
int main(int argc,char **argv) {
    CHECK(argc==4); input=argv[1]; selected=strtoul(argv[2],NULL,10);
    FILE *f=fopen(argv[3],"wb"); CHECK(f);
    MapMatrix *m=MapMatrix_New();
    for (unsigned cycle=0;cycle<64;++cycle) {
        memset(&m->data,0xA5,sizeof(m->data));
        MapMatrix_Load(64,m);
        unsigned size=m->width*m->height; CHECK(size && size<=MAP_MATRIX_MAX_SIZE);
        CHECK(MapMatrix_GetWidth(m)==m->data.width && MapMatrix_GetHeight(m)==m->data.height);
        for (unsigned i=size;i<MAP_MATRIX_MAX_SIZE;++i)
            CHECK(!m->data.headers[i] && !m->data.altitudes[i] && !m->data.maps.models[i]);
        CHECK(allocations==1);
    }
    fputc(m->width,f); fputc(m->height,f);
    fwrite(m->data.name,1,MAP_MATRIX_MAX_NAME_LENGTH,f);
    for (unsigned i=0;i<m->width*m->height;++i) {
        word(f,MapMatrix_GetMapHeader(m,i%m->width,i/m->width));
        fputc(m->data.altitudes[i],f); word(f,MapMatrix_GetMapModelNo(i,m));
    }
    CHECK(!fclose(f)); MapMatrix_Free(m); CHECK(!allocations);
    puts("map matrix: 64 reloads passed"); return 0;
}
