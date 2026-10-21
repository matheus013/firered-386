#define TRAINING_UI_FIXTURE
#include "hardcore_smoke.c"
#undef main
#include <assert.h>
static struct mCore *core;
static color_t *pixels;
static uint32_t command,selected,ready,result,stage,credits,loaded,mainAddress,outcome,caseType,audit;
static const char *prefix;
static int frames;
static void Run(int count)
{
    while(count--){sLogFrame=frames++;core->setKeys(core,!core->busRead32(core,ready)&&frames%16==0?1:0);core->runFrame(core);}
}
static void ReloadCore(const char *rom)
{
    void *save=NULL;size_t size=core->savedataClone(core,&save);assert(save&&size==131072);
    core->deinit(core);free(pixels);core=OpenTestCore(rom,&pixels);assert(core);
    assert(core->savedataRestore(core,save,size,false));free(save);sLogCore=core;sLogPixels=pixels;core->reset(core);
    for(int f=0;f<6000&&!core->busRead32(core,ready);f++)Run(1);
    if(!core->busRead32(core,ready)||!core->busRead32(core,loaded))exit(4);
}
static void SaveSnapshot(int index)
{
    void *save=NULL;size_t size=core->savedataClone(core,&save);assert(save&&size==131072);
    char path[4096];snprintf(path,sizeof(path),"%s-checkpoint-%02d-test-only.sav",prefix,index);
    FILE *file=fopen(path,"wb");assert(file);assert(fwrite(save,1,size,file)==size);fclose(file);free(save);
    printf("STATE checkpoint=%d",index);
    for(int i=0;i<11;i++)printf(" %u",core->busRead32(core,audit+4*i));
    puts("");fflush(stdout);
}
static void Fail(const char *message,int index)
{
    fprintf(stderr,"FAIL %s case=%d frames=%d ready=%u result=%u stage=%u credits=%u callback2=%08x outcome=%u\n",message,index,frames,core->busRead32(core,ready),core->busRead32(core,result),core->busRead32(core,stage),core->busRead32(core,credits),core->busRead32(core,mainAddress+4),core->busRead8(core,outcome));
    Shot(prefix,"failure",pixels);exit(1);
}
int main(int argc,char **argv)
{
    assert(argc==17);prefix=argv[2];core=OpenTestCore(argv[1],&pixels);assert(core);
    int count=atoi(argv[3]),start=atoi(argv[15]);uint32_t *addresses[]={&command,&selected,&ready,&result,&stage,&credits,&loaded,&mainAddress,&outcome,&caseType,&audit};
    for(int i=0;i<11;i++)*addresses[i]=strtoul(argv[4+i],0,0);
    static struct mLogger logger={.log=TestLog};sLogCore=core;sLogPixels=pixels;sLogPrefix=prefix;mLogSetDefaultLogger(&logger);
    if(argv[16][0]){FILE *file=fopen(argv[16],"rb");assert(file);void *save=malloc(131072);assert(fread(save,1,131072,file)==131072);fclose(file);assert(core->savedataRestore(core,save,131072,false));free(save);}
    core->reset(core);
    for(int f=0;f<3000&&!core->busRead32(core,ready);f++)Run(1);
    if(!core->busRead32(core,ready))Fail("initial-field",0);
    if(argv[16][0]&&!core->busRead32(core,loaded))Fail("resume-not-loaded",start);
    for(int index=start;index<count;index++)
    {
        core->busWrite32(core,selected,index);core->busWrite32(core,result,0);core->busWrite32(core,command,1);
        for(int f=0;f<6000&&!core->busRead32(core,result)&&!core->busRead32(core,credits);f++)Run(1);
        if(core->busRead32(core,caseType))
        {
            for(int f=0;f<60000&&!core->busRead32(core,credits);f++)Run(1);
            if(!core->busRead32(core,credits))Fail("champion-credits",index);
            printf("PASS checkpoint=%d champion, Hall of Fame save and complete credits\n",index);fflush(stdout);
            SaveSnapshot(index);ReloadCore(argv[1]);
            printf("PASS fresh-instance reload after credits\n");fflush(stdout);
            continue;
        }
        if(core->busRead32(core,result)!=1)Fail("warp",index);
        core->busWrite32(core,result,0);core->busWrite32(core,command,2);
        for(int f=0;f<30000&&!core->busRead32(core,result);f++)Run(1);
        if(core->busRead32(core,result)!=1)Fail("script-battle-reward",index);
        printf("PASS checkpoint=%d native script/battle/reward frames=%d\n",index,frames);fflush(stdout);
        core->busWrite32(core,result,0);core->busWrite32(core,command,3);
        for(int f=0;f<1800&&!core->busRead32(core,result);f++)Run(1);
        if(core->busRead32(core,result)!=1)Fail("save",index);
        SaveSnapshot(index);
        if(index==7)
        {
            ReloadCore(argv[1]);
            printf("PASS fresh-instance reload at checkpoint=%d\n",index);fflush(stdout);
        }
    }
    if(count>50)
    {
        ReloadCore(argv[1]);
        if(core->busRead32(core,audit)!=255 || !core->busRead32(core,audit+4) || !core->busRead32(core,audit+12)
            || !core->busRead32(core,audit+16) || !core->busRead32(core,audit+20))Fail("final-postgame-reload",count);
        printf("PASS final postgame fresh-instance reload: badges, game-clear, Celio, cave and Tanoby\n");fflush(stdout);
    }
    Shot(prefix,"completed",pixels);
    void *save=NULL;size_t size=core->savedataClone(core,&save);assert(save&&size==131072);
    char path[4096];snprintf(path,sizeof(path),"%s-postleague-test-only.sav",prefix);FILE *file=fopen(path,"wb");assert(file);assert(fwrite(save,1,size,file)==size);fclose(file);free(save);
    printf("PASS assisted campaign integration frames=%d save_bytes=%zu loaded=%u\n",frames,size,core->busRead32(core,loaded));
    core->deinit(core);free(pixels);return 0;
}
