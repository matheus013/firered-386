// Real field interaction and button-driven editor regression in disposable ROMs.
#define TRAINING_UI_FIXTURE
#include "hardcore_smoke.c"
#undef main
#include <assert.h>
static struct mCore *ui;
static uint32_t screen, party;
static unsigned Page(void);
static void Frames(int count) { while (count--) ui->runFrame(ui); }
static void Key(unsigned key) { unsigned before=Page(); ui->setKeys(ui,key); Frames(4); ui->setKeys(ui,0); Frames(30); fprintf(stderr,"key=%u page=%u->%u\n",key,before,Page()); }
static unsigned Page(void) { uint32_t ptr=ui->busRead32(ui,screen); return ptr ? ui->busRead8(ui,ptr) : 255; }
int main(int argc, char **argv)
{
    assert(argc==6);
    color_t *pixels;
    ui=OpenTestCore(argv[1],&pixels); assert(ui);
    screen=strtoul(argv[3],0,0); party=strtoul(argv[4],0,0);
    static struct mLogger logger={.log=TestLog};
    sLogCore=ui; sLogPixels=pixels; sLogPrefix=argv[2]; mLogSetDefaultLogger(&logger);
    ui->reset(ui); Frames(300); Shot(argv[2],"field",pixels);
    Key(1u<<6); // face the coach immediately north of the player
    for (int i=0;i<40 && Page()==255;i++) Key(1);
    Frames(60); Shot(argv[2],"party",pixels); assert(Page()==0);
    Key(1); Shot(argv[2],"selected",pixels); assert(Page()==1);
    Key(1); assert(Page()==2);
    Key(1u<<7); Shot(argv[2],"nature",pixels);
    Key(1); assert(Page()==5); Key(1); Shot(argv[2],"applied",pixels); assert(Page()==1);
    Key(1u<<7); Key(1); assert(Page()==3);
    Key(1u<<8); Shot(argv[2],"evs",pixels);
    Key(2); assert(Page()==1); // staging must not alter the mon on cancel
    uint32_t result=strtoul(argv[5],0,0);
    ui->busWrite32(ui,result,1);
    Frames(2); assert(ui->busRead32(ui,result)==101);
    Key(1u<<7); Key(1); assert(Page()==3); Key(1u<<8);
    Key(1); Shot(argv[2],"confirm",pixels); Key(1); assert(Page()==1);
    Key(1u<<7); Key(1u<<7); Key(1); assert(Page()==4);
    Key(1u<<8); Shot(argv[2],"ivs",pixels); Key(1); Key(1);
    ui->busWrite32(ui,result,2); Frames(2); assert(ui->busRead32(ui,result)==102);
    Key(2); Key(2); Frames(180); assert(Page()==255);
    Shot(argv[2],"returned",pixels);
    for (int i=0;i<40 && Page()==255;i++) Key(1);
    assert(Page()==0); Frames(60); Key(2); Frames(180); assert(Page()==255);
    // Walk from the coach to the nurse's counter, using the actual map events.
    // First press changes facing; the following three move three tiles.
    for (int i=0;i<4;i++) { ui->setKeys(ui,1u<<5); Frames(8); ui->setKeys(ui,0); Frames(24); }
    for (int i=0;i<4;i++) { ui->setKeys(ui,1u<<6); Frames(8); ui->setKeys(ui,0); Frames(24); }
    Shot(argv[2],"nurse",pixels);
    for (int visit=0;visit<4;visit++) {
        ui->busWrite32(ui,result,10); Frames(4);
        for (int i=0;i<200 && ui->busRead32(ui,result)==110;i++) Key(1);
        unsigned expected=visit<3 ? 1201+visit : 203;
        fprintf(stderr,"nurse visit=%d result=%u expected=%u\n",visit+1,ui->busRead32(ui,result),expected);
        Shot(argv[2],visit==3 ? "nurse-blocked" : "nurse-healed",pixels);
        assert(ui->busRead32(ui,result)==expected);
    }
    puts("PASS: field NPC, nature apply, EV cancel/apply, IV apply, exit and reopen.");
    puts("PASS: actual nurse heals three times, refuses fourth, and leaves HP at one.");
    ui->deinit(ui); free(pixels); return 0;
}
