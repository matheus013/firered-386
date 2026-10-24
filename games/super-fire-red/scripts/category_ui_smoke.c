#define TRAINING_UI_FIXTURE
#include "hardcore_smoke.c"
#undef main
#include <assert.h>
static struct mCore *ui;
static void Frames(unsigned n){while(n--)ui->runFrame(ui);}
static void Key(unsigned k){ui->setKeys(ui,k);Frames(4);ui->setKeys(ui,0);Frames(60);}
int main(int argc,char **argv)
{
    color_t *pixels;
#ifdef EXPANDED_MOVES
    unsigned expected[]={1,2,0,1,0};
#else
    unsigned expected[]={1,2,0,1,2};
#endif
    assert(argc==7);ui=OpenTestCore(argv[1],&pixels);assert(ui);
    static struct mLogger logger={.log=SmokeLog};mLogSetDefaultLogger(&logger);
    ui->reset(ui);Frames(1000);
    unsigned tooLong=ui->busRead32(ui,strtoul(argv[5],0,0));
    printf("overflow_descriptions=%u ids:",tooLong);
    for(unsigned i=0;i<tooLong&&i<128;i++)printf(" %u",ui->busRead16(ui,strtoul(argv[6],0,0)+2*i));
    puts("");fflush(stdout);assert(tooLong==0);
    for(unsigned i=0;i<5;i++)
    {
        if(i)Key(128);
        unsigned observed=ui->busRead32(ui,strtoul(argv[3],0,0));
        char label[30];snprintf(label,sizeof(label),"category-%u",i);Shot(argv[2],label,pixels);
        printf("slot=%u observed=%u expected=%u setup=%u\n",i,observed,10*i+expected[i]+1,ui->busRead32(ui,strtoul(argv[4],0,0)));fflush(stdout);
        assert(observed==10*i+expected[i]+1);
    }
    puts("PASS: actual move selection UI, physical/special/status across all five slots.");
    ui->deinit(ui);free(pixels);return 0;
}
