// Button-driven regular shop regression using disposable ROMs and fresh cores.
#define TRAINING_UI_FIXTURE
#include "hardcore_smoke.c"
#undef main
#include <assert.h>
static struct mCore *core;
static uint32_t tasks,command,money,done,qty,info;
static const char *prefix;
static color_t *pixels;
static int frames;
static void Run(int count){while(count--){sLogFrame=frames++;core->runFrame(core);}}
static void Key(unsigned key){core->setKeys(core,key);Run(3);core->setKeys(core,0);Run(17);}
static int Has(uint32_t function){return TaskExists(core,tasks,function);}
static void Fail(const char *stage){fprintf(stderr,"FAIL %s frames=%d money=%u done=%u\n",stage,frames,core->busRead32(core,money),core->busRead32(core,done));Shot(prefix,stage,pixels);exit(1);}
static void Wait(uint32_t function,const char *stage){for(int i=0;i<1800&&!Has(function);i++)Run(1);if(!Has(function))Fail(stage);Run(40);}
static void Drive(unsigned key,uint32_t function,const char *stage){for(int i=0;i<180&&!Has(function);i++)Key(key);if(!Has(function))Fail(stage);Run(40);}
static void ExitShop(uint32_t menu){Drive(2,menu,"return-menu");Key(2);for(int i=0;i<300&&!core->busRead32(core,done);i++)Run(1);if(core->busRead32(core,done)!=1)Fail("quit-shop");}
int main(int argc,char **argv)
{
    assert(argc==19);
    core=OpenTestCore(argv[1],&pixels);assert(core);prefix=argv[2];
    static struct mLogger logger={.log=TestLog};sLogCore=core;sLogPixels=pixels;sLogPrefix=prefix;mLogSetDefaultLogger(&logger);
    uint32_t selected=strtoul(argv[3],0,0), mode=strtoul(argv[4],0,0);
    command=strtoul(argv[5],0,0);money=strtoul(argv[6],0,0);done=strtoul(argv[7],0,0);qty=strtoul(argv[8],0,0);info=strtoul(argv[9],0,0);tasks=strtoul(argv[10],0,0);
    uint32_t shop=strtoul(argv[11],0,0),buy=strtoul(argv[12],0,0),bag=strtoul(argv[13],0,0);
    uint32_t selectedCase=strtoul(argv[14],0,0),selectedMode=strtoul(argv[15],0,0),last=strtoul(argv[16],0,0),firstprice=strtoul(argv[17],0,0),closed=strtoul(argv[18],0,0);
    core->reset(core);Run(30);core->busWrite32(core,selected,selectedCase);core->busWrite32(core,mode,selectedMode);core->busWrite32(core,command,1);Run(400);
    // Cancel without a transaction, then close and reopen.
    core->busWrite32(core,command,2);Run(2);Wait(shop,"open-cancel");Key(1);Wait(buy,"cancel-buy");Key(2);ExitShop(shop);
    core->busWrite32(core,command,2);Run(2);Wait(shop,"open-purchase");Key(1);Wait(buy,"purchase-list");
    uint32_t before=core->busRead32(core,money), beforeQty=core->busRead32(core,qty), price=core->busRead32(core,firstprice);
    for(int i=0;i<160&&core->busRead32(core,money)==before;i++)Key(1);
    if(core->busRead32(core,money)!=before-price||core->busRead32(core,qty)!=beforeQty+1)Fail("purchase-accounting");
    Drive(2,buy,"purchase-back-list");Key(2);Drive(1,shop,"purchase-return-menu");
    // Enter Sell; Potion was added first to the Items pocket by the fixture.
    Key(1u<<7);Key(1);Wait(bag,"sell-bag");
    before=core->busRead32(core,money);
    for(int i=0;i<160&&core->busRead32(core,money)==before;i++)Key(1);
    if(core->busRead32(core,money)!=before+150)Fail("sale-accounting");
    Drive(2,bag,"sale-back-bag");Key(2);Drive(1,shop,"sale-return-menu");
    Key(1u<<7);Key(1u<<7);Key(1); // Explicit SEE YA rather than B.
    for(int i=0;i<300&&!core->busRead32(core,done);i++)Run(1);
    if(core->busRead32(core,done)!=1)Fail("quit-after-sale");
    // Reopen again; the full stock must survive field heap resets.
    core->busWrite32(core,command,2);Run(2);Wait(shop,"reopen-after-sale");Key(1);Wait(buy,"reopen-list");
    if(core->busRead32(core,info)!=1)Fail("stock-changed");
    if(selectedMode){
        uint32_t itemCount=core->busRead32(core,last);
        for(uint32_t i=1;i<itemCount;i++)Key(1u<<7);
        before=core->busRead32(core,money);
        for(int i=0;i<160&&core->busRead32(core,money)==before;i++)Key(1);
        if(core->busRead32(core,money)!=before-1)Fail("candy-price");
        Drive(2,buy,"candy-back-list");
    }
    Key(2);ExitShop(shop);
    if(core->busRead32(core,closed)<3)Fail("script-not-resumed");
    Shot(prefix,"passed",pixels);
    printf("PASS catalog=%u mode=%s: cancel, buy, sell, B/SEE YA, three open/exit cycles%s.\n",selectedCase,selectedMode?"Hardcore":"Normal",selectedMode?", Candy=1":"");
    core->deinit(core);free(pixels);return 0;
}
