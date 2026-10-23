#define TRAINING_UI_FIXTURE
#include "hardcore_smoke.c"
#undef main
#include <assert.h>
static struct mCore *ui;
static void Frames(unsigned n){while(n--)ui->runFrame(ui);}
static void Key(unsigned k){ui->setKeys(ui,k);Frames(4);ui->setKeys(ui,0);Frames(45);}
int main(int argc,char **argv)
{
    color_t *pixels;assert(argc==12);ui=OpenTestCore(argv[1],&pixels);assert(ui);
    static struct mLogger logger={.log=SmokeLog};mLogSetDefaultLogger(&logger);
    unsigned addrCase=strtoul(argv[3],0,0),addrDone=strtoul(argv[4],0,0);
    unsigned addrCount=strtoul(argv[5],0,0),addrRows=strtoul(argv[6],0,0);
    for(unsigned test=1;test<=5;test++){
        ui->reset(ui);Frames(400);ui->busWrite32(ui,addrCase,test);
        for(unsigned f=0;f<2400&&!ui->busRead32(ui,addrDone);f++)Frames(1);
        assert(ui->busRead32(ui,addrDone)==1);
        if(test==1){
            unsigned count=ui->busRead32(ui,addrCount);assert(count>4000&&count<5000);
            printf("measured=%u\n",count);
            unsigned failures=ui->busRead32(ui,strtoul(argv[7],0,0));
            for(unsigned i=0;i<failures;i++){
                unsigned row[5];for(unsigned j=0;j<5;j++)row[j]=ui->busRead16(ui,addrRows+10*i+2*j);
                if(row[2]>row[3])printf("overflow=%u,%u,%u,%u,%u\n",row[0],row[1],row[2],row[3],row[4]);
            }
            fflush(stdout);
            continue;
        }
        Frames(1200);
        char label[60];
        if(test<=3){
            unsigned funcs=strtoul(argv[8],0,0),action=strtoul(argv[9],0,0)&~1u,move=strtoul(argv[10],0,0)&~1u;
            for(unsigned i=0;i<160&&(ui->busRead32(ui,funcs)&~1u)!=action;i++)Key(1);
            if((ui->busRead32(ui,funcs)&~1u)!=action){fprintf(stderr,"test=%u controller=%08x action=%08x move=%08x\n",test,ui->busRead32(ui,funcs),action,move);Shot(argv[2],"failed-action",pixels);}
            assert((ui->busRead32(ui,funcs)&~1u)==action);Key(1);
            assert((ui->busRead32(ui,funcs)&~1u)==move);
            for(unsigned i=0;i<4;i++){
                unsigned expected[]={0,1,3,2};
                assert(ui->busRead8(ui,strtoul(argv[11],0,0))==expected[i]);
                snprintf(label,sizeof(label),"battle-%s-%u",test==2?"ptbr":"en",i);Shot(argv[2],label,pixels);
                Key(i==0?16:i==1?128:32);
            }
        }else{
            for(unsigned i=0;i<50;i++){
                if(i==0||i==15||i==49){snprintf(label,sizeof(label),"tm-%s-%u",test==4?"ptbr":"en",i+1);Shot(argv[2],label,pixels);}
                Key(128);
            }
        }
    }
    ui->deinit(ui);free(pixels);return 0;
}
