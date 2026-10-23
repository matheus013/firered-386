#define TRAINING_UI_FIXTURE
#include "hardcore_smoke.c"
#undef main
#include <assert.h>
int main(int argc,char **argv)
{
 assert(argc==7);color_t *pixels;struct mCore *core=OpenTestCore(argv[1],&pixels);assert(core);
 static struct mLogger logger={.log=TestLog};sLogCore=core;sLogPixels=pixels;sLogPrefix=argv[2];mLogSetDefaultLogger(&logger);
 uint32_t done=strtoul(argv[3],0,0),pass=strtoul(argv[4],0,0),fail=strtoul(argv[5],0,0),observed=strtoul(argv[6],0,0);
 core->reset(core);int f;for(f=0;f<18000&&!core->busRead32(core,done);f++){sLogFrame=f;core->runFrame(core);}
 if(!core->busRead32(core,done)){fprintf(stderr,"fixture timeout\n");return 2;}
 printf("{\"passed\":%u,\"failed\":%u,\"frames\":%d,\"observed\":[",core->busRead32(core,pass),core->busRead32(core,fail),f);
 for(int i=0;i<6;i++)
     printf("%s%u",i?",":"",core->busRead32(core,observed+4*i));
 puts("]}");
 core->deinit(core);free(pixels);return 0;
}
