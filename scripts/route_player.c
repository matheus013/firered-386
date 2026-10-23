// Production-ROM playthrough: buttons only; memory reads are observation only.
#define main BootSmokeMain
#include "smoke.c"
#undef main
#include <mgba/core/serialize.h>
#include <mgba-util/vfs.h>
#include <fcntl.h>

// Addresses are read-only observations from the reference ELF for SHA d8808450.
static unsigned ButtonForBattle(struct mCore *c)
{
    for (int i = 0; i < 4; i += 2) {
        uint32_t function = c->busRead32(c, 0x03004fe0 + 4*i) & ~1u;
        if (function == 0x0802e688) {
            unsigned cursor = c->busRead8(c, 0x02023ff8+i);
            return cursor & 1 ? 32 : cursor & 2 ? 64 : 1;
        }
        if (function == 0x0802ec60) {
            uint32_t attacker = 0x02023be4 + 88*i;
            unsigned best = 0; double scoreBest = -1;
            for (unsigned slot = 0; slot < 4; ++slot) {
                unsigned move = c->busRead16(c, attacker + 12 + 2*slot);
                if (!move || !c->busRead8(c, attacker+36+slot)) continue;
                uint32_t info = 0x08254da4 + 12*move;
                unsigned power = c->busRead8(c, info+1), type = c->busRead8(c, info+2);
                double score = 0;
                for (unsigned enemy = 1; enemy < c->busRead8(c, 0x02023bcc); enemy+=2) {
                    uint32_t defender = 0x02023be4+88*enemy;
                    if (!c->busRead16(c, defender+40)) continue;
                    double multiplier = 1;
                    unsigned t1 = c->busRead8(c,defender+33),t2 = c->busRead8(c,defender+34);
                    for (unsigned row=0;row<512;row+=3) {
                        unsigned t=c->busRead8(c,0x082531f0+row);
                        if (t==255) break;
                        unsigned d=c->busRead8(c,0x082531f0+row+1);
                        if (t==type && (d==t1 || d==t2)) multiplier*=c->busRead8(c,0x082531f0+row+2)/10.0;
                    }
                    unsigned ability=c->busRead8(c,defender+32);
                    if ((type==4 && ability==26)||(type==13 && ability==10)||(type==11 && ability==11)||(type==10 && ability==18)||(ability==25 && multiplier<=1)) multiplier=0;
                    unsigned atk=c->busRead16(c,attacker+(type<9?2:8)),def=c->busRead16(c,defender+(type<9?4:10));
                    double value=power*multiplier*atk/(def?def:1);
                    if (type==c->busRead8(c,attacker+33)||type==c->busRead8(c,attacker+34)) value*=1.5;
                    if (value>score)score=value;
                }
                if(score>scoreBest){scoreBest=score;best=slot;}
            }
            unsigned cursor=c->busRead8(c,0x02023ffc+i);
            return (cursor&1)!=(best&1)?((best&1)?16:32):(cursor&2)!=(best&2)?((best&2)?128:64):1;
        }
    }
    // The battle's native party menu still handles eligibility and confirmation.
    uint32_t callback=c->busRead32(c,0x030030f4)&~1u;
    if (callback!=0x0801113c && (c->busRead8(c,0x030030f0+0x439)&2)) {
        int selected=(int8_t)c->busRead8(c,0x0203b0a9),best=-1;
        for (int i=0;i<c->busRead8(c,0x02024029);i++) {
            if (!c->busRead16(c,0x02024284+100*i+86))continue;
            if (i==c->busRead16(c,0x02023bce)||i==c->busRead16(c,0x02023bd2))continue;
            if(best<0||c->busRead8(c,0x02024284+100*i+84)>c->busRead8(c,0x02024284+100*best+84))best=i;
        }
        if(best>=0 && selected!=best)return 128;
    }
    return 1;
}

int main(int argc, char **argv)
{
    if (argc < 3 || argc > 4) return 2;
    static struct mLogger logger = {.log = SmokeLog};
    mLogSetDefaultLogger(&logger);
    struct mCore *core = mCoreFind(argv[1]);
    if (!core || !core->init(core)) return 3;
    mCoreInitConfig(core, "firered-production-route");
    if (!mCoreLoadFile(core, argv[1])) return 4;
    color_t *pixels = calloc(240 * 160, sizeof(*pixels));
    core->setVideoBuffer(core, pixels, 240);
    core->setAudioBufferSize(core, 1024);
    core->reset(core);
    char path[4096];
    snprintf(path, sizeof(path), "%s/state.bin", argv[2]);
    struct VFile *state = VFileOpen(path, O_RDONLY);
    if (state) {
        if (!mCoreLoadStateNamed(core, state, SAVESTATE_SAVEDATA | SAVESTATE_RTC)) return 5;
        state->close(state);
    }
    unsigned keys, frames;
    snprintf(path,sizeof(path),"%s/executed-buttons.log",argv[2]);
    FILE *inputLog=fopen(path,"a");if(!inputLog)return 11;
    unsigned lastKey=~0u;
    uint32_t sb = core->busRead32(core, 0x03005008);
    uint16_t map = core->busRead16(core, sb + 4);
    while (scanf("%u %u", &keys, &frames) == 2) {
        if (frames > 36000 || keys > 1024) return 6;
        if (argc == 4 && !strcmp(argv[3],"walk") && (core->busRead8(core, 0x03000f9c)
            || (core->busRead8(core, 0x030030f0 + 0x439) & 2)
            || core->busRead16(core, core->busRead32(core, 0x03005008) + 4) != map)) {
            puts("navigation interrupted by field lock, battle or map transition");
            break;
        }
        int seenBattle = 0;
        for (unsigned i = 0; i < frames; ++i) {
            unsigned pressed=keys;
            if(keys==1024) {
                int battle=core->busRead8(core,0x030030f0+0x439)&2;
                if(battle)seenBattle=1;
                if(seenBattle && !battle)break;
                if(!seenBattle && i>=1200)break;
                pressed=battle && i%12==0?ButtonForBattle(core):0;
            }
            if(pressed!=lastKey){fprintf(inputLog,"%u %u\n",core->frameCounter(core),pressed);lastKey=pressed;}
            core->setKeys(core,pressed);
            core->runFrame(core);
        }
    }
    core->setKeys(core, 0);
    fclose(inputLog);
    snprintf(path,sizeof(path),"%s/state.bin",argv[2]);
    state = VFileOpen(path, O_RDWR | O_CREAT | O_TRUNC);
    if (!state || !mCoreSaveStateNamed(core, state, SAVESTATE_SAVEDATA | SAVESTATE_RTC)) return 7;
    state->close(state);
    void *saveData=NULL;size_t saveSize=core->savedataClone(core,&saveData);
    if (saveData && saveSize) {
        snprintf(path,sizeof(path),"%s/native-save.sav",argv[2]);
        FILE *file=fopen(path,"wb");if(!file)return 10;
        fwrite(saveData,1,saveSize,file);fclose(file);free(saveData);
    }
    snprintf(path, sizeof(path), "%s/screen.png", argv[2]);
    if (!WriteImage(path, pixels, 240, 160)) return 8;
    const uint32_t base[] = {0x02000000, 0x03000000};
    const unsigned size[] = {0x40000, 0x8000};
    for (unsigned region = 0; region < 2; ++region) {
        snprintf(path, sizeof(path), "%s/%s.bin", argv[2], region ? "iwram" : "ewram");
        FILE *file = fopen(path, "wb");
        if (!file) return 9;
        for (unsigned i = 0; i < size[region]; ++i) fputc(core->busRead8(core, base[region] + i), file);
        fclose(file);
    }
    printf("frame=%u\n", core->frameCounter(core));
    core->deinit(core);
    free(pixels);
    return 0;
}
