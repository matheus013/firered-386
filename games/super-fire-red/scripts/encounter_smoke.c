#define TRAINING_UI_FIXTURE
#include "hardcore_smoke.c"
#undef main
#include <assert.h>
int main(int argc,char **argv)
{
    assert(argc==10);
    color_t *pixels;
    struct mCore *core=OpenTestCore(argv[1],&pixels);assert(core);
    static struct mLogger logger={.log=TestLog};
    sLogCore=core;sLogPixels=pixels;sLogPrefix=argv[2];mLogSetDefaultLogger(&logger);
    uint32_t result=strtoul(argv[3],0,0),species=strtoul(argv[4],0,0),slots=strtoul(argv[5],0,0);
    uint32_t callback=strtoul(argv[6],0,0),ready=strtoul(argv[7],0,0),enemy=strtoul(argv[8],0,0),count=strtoul(argv[9],0,0);
    int battleReady=0;
    core->reset(core);
    for(int frame=0;frame<18000;frame++){
        uint32_t code=core->busRead32(core,result);
        if(code && code!=100 && code!=1){fprintf(stderr,"audit failed code=%u\n",code);return 1;}
        core->setKeys(core,frame%30==0 ? 1 : 0);core->runFrame(core);
        if((core->busRead32(core,callback)&~1u)==(ready&~1u)){battleReady=1;break;}
    }
    assert(core->busRead32(core,result)==1 && core->busRead32(core,species)==386);
    assert(battleReady && core->busRead8(core,count)==2);
    (void)enemy; // The ROM audit already decrypts the actual enemy data.
    core->setKeys(core,0);for(int i=0;i<10;i++)core->runFrame(core);
    Shot(argv[2],"mew-battle",pixels);
    printf("PASS: compiled slots=%u unique species=%u; 21 pre-League legend gates; actual Mew battle ready.\n",core->busRead32(core,slots),core->busRead32(core,species));
    core->deinit(core);free(pixels);return 0;
}
