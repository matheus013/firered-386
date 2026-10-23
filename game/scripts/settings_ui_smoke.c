#define TRAINING_UI_FIXTURE
#include "hardcore_smoke.c"
#undef main
#include <assert.h>
static struct mCore *sCore;
static void Frames(unsigned count){while(count--)sCore->runFrame(sCore);}
static void Key(unsigned key){sCore->setKeys(sCore,key);Frames(4);sCore->setKeys(sCore,0);Frames(45);}
int main(int argc,char **argv)
{
    assert(argc>=6);
    color_t *pixels;sCore=OpenTestCore(argv[1],&pixels);assert(sCore);
    static struct mLogger logger={.log=SmokeLog};mLogSetDefaultLogger(&logger);
    sCore->reset(sCore);
    if(!strcmp(argv[3],"intro"))
    {
        assert(argc==9);
        uint32_t tasks=strtoul(argv[4],0,0),choose=strtoul(argv[5],0,0),review=strtoul(argv[6],0,0),randomizer=strtoul(argv[7],0,0),savePtr=strtoul(argv[8],0,0);
        for(unsigned frame=0;frame<3600 && !TaskExists(sCore,tasks,choose);frame++)
        {sCore->setKeys(sCore,frame%60==0?8:frame%60==30?1:0);sCore->runFrame(sCore);}
        sCore->setKeys(sCore,0);Frames(90);assert(TaskExists(sCore,tasks,choose));
        Shot(argv[2],"mode-en",pixels);Key(1);assert(TaskExists(sCore,tasks,review));
        Key(1);Frames(60);assert(TaskExists(sCore,tasks,randomizer));Shot(argv[2],"randomizer-off-en",pixels);
        Key(2);assert(TaskExists(sCore,tasks,choose));
        Key(128);Key(1);assert(TaskExists(sCore,tasks,review));Shot(argv[2],"hardcore-review-en",pixels);
        Key(1);assert(TaskExists(sCore,tasks,randomizer));Key(128);Shot(argv[2],"randomizer-on-en",pixels);Key(1);Frames(90);
        assert(!TaskExists(sCore,tasks,randomizer));
        uint32_t save=sCore->busRead32(sCore,savePtr);
        assert(sCore->busRead8(sCore,save+0xB24)==1 && sCore->busRead8(sCore,save+0xB75)==1 && sCore->busRead32(sCore,save+0xB70));
        Shot(argv[2],"controls-after-choices",pixels);
        puts("PASS production-ROM new game: mode review/back, randomizer back/off/on, hardcore selection and controls continuation");
    }
    else
    {
        assert(argc==7);
        uint32_t menuPtr=strtoul(argv[4],0,0),result=strtoul(argv[5],0,0),savePtr=strtoul(argv[6],0,0);
        Frames(240);uint32_t menu=sCore->busRead32(sCore,menuPtr);assert(menu);
        Shot(argv[2],"options-en",pixels);Key(128);Key(128);Key(16);
        assert(sCore->busRead16(sCore,menu+4)==1); // Hardcore SET cannot be toggled.
        for(unsigned i=0;i<4;i++)Key(128);
        Key(16);Shot(argv[2],"options-ptbr",pixels);
        assert(sCore->busRead16(sCore,menu+12)==1);
        Key(2);Frames(180);
        assert(!sCore->busRead32(sCore,menuPtr) && sCore->busRead32(sCore,result)==1);
        uint32_t save=sCore->busRead32(sCore,savePtr);
        assert(sCore->busRead8(sCore,save+0xB74)==1 && sCore->busRead8(sCore,save+0xB75)==1);
        puts("PASS native options: EN/PT-BR switch, live redraw, exit persistence, Hardcore SET and randomizer preserved");
    }
    sCore->deinit(sCore);free(pixels);return 0;
}
