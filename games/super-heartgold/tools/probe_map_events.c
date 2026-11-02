/* Diagnostic only: include unchanged upstream unit to exercise its static parser. */
#include "map_events.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(x) do { if (!(x)) { fprintf(stderr,"check line %d\n",__LINE__); abort(); } } while (0)
void GF_AssertFail(void) { abort(); }
void SIM_handleAssertionFailureMsg(const char *file,unsigned line,const char *fmt,...) {
    (void)fmt; fprintf(stderr,"assert %s:%u\n",file,line); abort();
}
static void section(FILE *out, u32 count, const void *data, size_t size) {
    CHECK((count == 0) == (data == NULL));
    CHECK(fwrite(&count,4,1,out)==1);
    if (count) CHECK(fwrite(data,size,count,out)==count);
}
int main(int argc,char **argv) {
    CHECK(argc==3);
    _Static_assert(sizeof(BgEvent)==20,"BG disk ABI");
    _Static_assert(sizeof(ObjectEvent)==32,"Object disk ABI");
    _Static_assert(sizeof(WarpEvent)==12,"Warp disk ABI");
    _Static_assert(sizeof(CoordEvent)==16,"Coord disk ABI");
    FieldSystem field={0}; MapEvents events; field.mapEvents=&events;
    unsigned members=strtoul(argv[2],NULL,10);
    for (unsigned n=0;n<members;n++) {
        char path[4096]; CHECK(snprintf(path,sizeof(path),"%s/%u.bin",argv[1],n)<sizeof(path));
        FILE *in=fopen(path,"rb"); CHECK(in);
        memset(&events,0xA5,sizeof(events));
        size_t length=fread(events.event_data,1,sizeof(events.event_data),in);
        CHECK(length>=16 && length<sizeof(events.event_data) && !ferror(in)); fclose(in);
        /* Inputs are independently bounded by the Python runner before execution. */
        MapEvents_ComputeRamHeader(&events);
        CHECK(snprintf(path,sizeof(path),"%s/%u.out",argv[1],n)<sizeof(path));
        FILE *out=fopen(path,"wb"); CHECK(out);
        section(out,Field_GetNumBgEvents(&field),Field_GetBgEvents(&field),sizeof(BgEvent));
        section(out,Field_GetNumObjectEvents(&field),Field_GetObjectEvents(&field),sizeof(ObjectEvent));
        section(out,events.num_warp_events,Field_GetWarpEventI(&field,0),sizeof(WarpEvent));
        section(out,Field_GetNumCoordEvents(&field),Field_GetCoordEvents(&field),sizeof(CoordEvent));
        CHECK(!fclose(out));
        CHECK(!Field_GetWarpEventI(&field,events.num_warp_events));
        CHECK(!Field_GetWarpEventI(&field,0xFFFFFFFF));
        CHECK(Field_GetWarpEventAtXYPos(&field,-1,-1)==-1);
        for (unsigned i=0;i<events.num_warp_events;i++) {
            const WarpEvent *w=Field_GetWarpEventI(&field,i);
            int first=0;
            while (events.warp_events[first].x!=w->x || events.warp_events[first].z!=w->z) first++;
            CHECK(Field_GetWarpEventAtXYPos(&field,w->x,w->z)==first);
        }
    }
    puts("original event parser/getters/warp lookup passed"); return 0;
}
