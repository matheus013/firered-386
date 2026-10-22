// Disposable-fixture runner with a budget large enough to enumerate every NPC.
#define TRAINING_UI_FIXTURE
#include "hardcore_smoke.c"
#undef main
int main(int argc,char **argv)
{
    if(argc!=5)return 2;
    color_t *pixels;struct mCore *core=OpenTestCore(argv[1],&pixels);
    if(!core)return 3;
    static struct mLogger logger={.log=SmokeLog};mLogSetDefaultLogger(&logger);
    uint32_t resultAddr=strtoul(argv[3],NULL,0),audit=strtoul(argv[4],NULL,0);
    core->reset(core);
    for(unsigned phase=1;phase<=2;phase++)
    {
        uint32_t result=0;
        for(unsigned frame=0;frame<120000 && (!result || result>=100);frame++)
        {
            core->runFrame(core);result=core->busRead32(core,resultAddr);
            if(frame && frame%10000==0){printf("progress frame=%u phase=%u result=%u trainer=%u randomizer=%u\n",frame,phase,result,core->busRead32(core,audit+8),core->busRead32(core,audit));fflush(stdout);}
        }
        printf("phase=%u result=%u audit=",phase,result);
        for(unsigned i=0;i<12;i++)printf("%u ",core->busRead32(core,audit+4*i));
        puts("");fflush(stdout);
        if(result!=phase)return 4;
        if(phase==1)
        {
            void *data=NULL;size_t size=core->savedataClone(core,&data);
            if(size!=131072 || !data)return 5;
            FILE *file=fopen(argv[2],"wb");if(!file)return 6;
            fwrite(data,1,size,file);fclose(file);
            core->deinit(core);free(pixels);core=OpenTestCore(argv[1],&pixels);
            if(!core || !core->savedataRestore(core,data,size,false))return 7;
            free(data);core->reset(core);
        }
    }
    core->deinit(core);free(pixels);
    puts("PASS native settings, all NPC parties, localization and fresh-instance persistence");return 0;
}
