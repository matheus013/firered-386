#define TRAINING_UI_FIXTURE
#include "hardcore_smoke.c"
#undef main
int main(int argc,char **argv)
{
    color_t *pixels;struct mCore *core;unsigned frame,done,checks,fail;
    if(argc!=7)return 2;
    core=OpenTestCore(argv[1],&pixels);if(!core)return 3;
    core->reset(core);
    for(frame=0;frame<5000;frame++)
    {
        core->setKeys(core,frame%12==0?1:0);core->runFrame(core);
        if(core->busRead32(core,strtoul(argv[2],0,0)))break;
    }
    done=core->busRead32(core,strtoul(argv[2],0,0));
    checks=core->busRead32(core,strtoul(argv[3],0,0));fail=core->busRead32(core,strtoul(argv[4],0,0));
    printf("done=%u checks=%u failures=%u bits=%u frames=%u first=%u\n",done,checks,fail,core->busRead32(core,strtoul(argv[5],0,0)),frame,core->busRead32(core,strtoul(argv[6],0,0)));
    core->deinit(core);free(pixels);return !done||fail;
}
