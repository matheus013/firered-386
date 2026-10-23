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
    assert(argc==6);ui=OpenTestCore(argv[1],&pixels);assert(ui);
    static struct mLogger logger={.log=SmokeLog};mLogSetDefaultLogger(&logger);
    ui->reset(ui);Frames(1000);
    unsigned done=ui->busRead32(ui,strtoul(argv[3],0,0)),passed=ui->busRead32(ui,strtoul(argv[4],0,0)),count=ui->busRead32(ui,strtoul(argv[5],0,0));
    printf("{\"done\":%u,\"passed\":%u,\"checks\":%u}\n",done,passed,count);fflush(stdout);
    Shot(argv[2],"info",pixels);
    Key(16);Key(16); // RIGHT twice: skills, then known moves
    Shot(argv[2],"moves",pixels);
    assert(done==1&&count>0&&passed==(1u<<count)-1);
    ui->deinit(ui);free(pixels);return 0;
}
