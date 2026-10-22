#define TRAINING_UI_FIXTURE
#include "hardcore_smoke.c"
#undef main
#include <assert.h>
static void Quiet(struct mLogger *logger,int category,enum mLogLevel level,const char *format,va_list args){(void)logger;(void)category;(void)level;(void)format;(void)args;}
int main(int argc,char **argv)
{
    if(argc!=7)return 2;
    color_t *pixels;struct mCore *core=OpenTestCore(argv[1],&pixels);if(!core)return 3;
    uint32_t command=strtoul(argv[2],0,0),ready=strtoul(argv[3],0,0),input=strtoul(argv[4],0,0),output=strtoul(argv[5],0,0);
    static struct mLogger logger={.log=Quiet};mLogSetDefaultLogger(&logger);
    core->reset(core);for(int frame=0;frame<3000&&!core->busRead32(core,ready);frame++)core->runFrame(core);
    if(core->busRead32(core,ready)!=1)return 4;
    unsigned params[112],counter=0;
    while(scanf("%u",&params[0])==1)
    {
        for(int i=1;i<112;i++)if(scanf("%u",&params[i])!=1)return 5;
        for(int i=0;i<112;i++)core->busWrite32(core,input+4*i,params[i]);
        core->busWrite32(core,ready,0);core->busWrite32(core,command,1);
        unsigned frame,bagStage=0;
        for(frame=0;frame<120000&&core->busRead32(core,ready)!=2;frame++)
        {
            unsigned key=0;
            if(frame%12==0)
            {
                unsigned menu=params[15]?core->busRead32(core,output+44):0;
                if(menu==3){key=bagStage?1:16;bagStage=1;}
                else if(menu==4){key=32;bagStage=0;}
                else if(menu==6){key=16;bagStage=0;}
                else if(menu==7){key=32;bagStage=0;}
                else{key=1;bagStage=0;}
            }
            core->setKeys(core,key);core->runFrame(core);
        }
        printf("%u %u",frame,core->busRead32(core,ready));
        for(int i=0;i<192;i++)printf(" %u",core->busRead32(core,output+4*i));
        puts("");fflush(stdout);
        if(core->busRead32(core,ready)!=2){char label[50];snprintf(label,sizeof(label),"timeout-%u",counter);Shot(argv[6],label,pixels);return 6;}
        counter++;
    }
    core->deinit(core);free(pixels);return 0;
}
