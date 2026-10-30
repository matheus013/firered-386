/* Exercise original finite scroll tasks and the original BG commit paths. */
#include "intro_movie_internal.h"
static BgConfig scroll_config;
static IntroMovieBgScrollAnim scroll_anims[8];
#ifdef HG_SCENE4_SLIDE
#include "probe_scene_slide.h"
#endif
static void verify_scroll_lifecycle(void) {
    BOOL irq=OS_DisableIrq();
    for (unsigned queue=0;queue<2;++queue) {
        SysTaskQueue *tasks=queue ? gSystem.mainTaskQueue : gSystem.vblankTaskQueue;
        void (*start)(BgConfig *,IntroMovieBgScrollAnim *,GFBgLayer,s16,s16,int)=
            queue ? IntroMovie_StartBgScroll_NotVBlank : IntroMovie_StartBgScroll_VBlank;
        for (unsigned layer=0;layer<8;++layer) {
            for (unsigned cycle=0;cycle<32;++cycle) {
                IntroMovieBgScrollAnim *anim=&scroll_anims[layer];
                BgSetPosTextAndCommit(&scroll_config,layer,BG_POS_OP_SET_X,0);
                BgSetPosTextAndCommit(&scroll_config,layer,BG_POS_OP_SET_Y,0);
                start(&scroll_config,scroll_anims,layer,13,-7,0);
                if (anim->active || anim->task || Bg_GetXpos(&scroll_config,layer)!=13 ||
                    Bg_GetYpos(&scroll_config,layer)!=-7) abort();
                start(&scroll_config,scroll_anims,layer,3,-2,-1);
                if (IntroMovie_WaitBgScrollAnim(scroll_anims,layer)) abort();
                for (unsigned tick=0;tick<12;++tick) {
                    SysTaskQueue_RunTasks(tasks);
                    DoScheduledBgGpuUpdates(&scroll_config);
                }
                if (!anim->active || !anim->task || anim->finished ||
                    Bg_GetXpos(&scroll_config,layer)!=49 || Bg_GetYpos(&scroll_config,layer)!=-31) abort();
                IntroMovie_CancelBgScrollAnim(scroll_anims,layer);
                IntroMovie_CancelBgScrollAnim(scroll_anims,layer);
                for (unsigned tick=0;tick<4;++tick) {
                    SysTaskQueue_RunTasks(tasks);
                    DoScheduledBgGpuUpdates(&scroll_config);
                }
                if (anim->active || anim->task || !IntroMovie_WaitBgScrollAnim(scroll_anims,layer) ||
                    Bg_GetXpos(&scroll_config,layer)!=49 || Bg_GetYpos(&scroll_config,layer)!=-31) abort();
                start(&scroll_config,scroll_anims,layer,-9,6,3);
                for (unsigned tick=0;tick<3;++tick) {
                    SysTaskQueue_RunTasks(tasks);
                    DoScheduledBgGpuUpdates(&scroll_config);
                }
                if (anim->active || anim->task || !anim->finished || anim->counter!=3 ||
                    Bg_GetXpos(&scroll_config,layer)!=40 || Bg_GetYpos(&scroll_config,layer)!=-25) abort();
            }
            BgSetPosTextAndCommit(&scroll_config,layer,BG_POS_OP_SET_X,0);
            BgSetPosTextAndCommit(&scroll_config,layer,BG_POS_OP_SET_Y,0);
        }
    }
    OS_RestoreIrq(irq);
    puts("SCROLL lifecycle cycles=512 layers=8 queues=2 continuous_ticks=6144 passed");
    fflush(stdout);
}
static void update_scroll(unsigned frame) {
#ifdef HG_SCENE4_SLIDE
    update_slide(frame);
    return;
#endif
    const GFBgLayer layers[]={GF_BG_LYR_MAIN_1,GF_BG_LYR_SUB_1};
    if (frame==1 || frame==40 || frame==80 || frame==120) {
        int x=frame==1 ? -48 : frame==40 ? 48 : frame==80 ? 37 : -37;
        int y=frame==1 ? 32 : frame==40 ? -32 : frame==80 ? -24 : 24;
        for (unsigned i=0;i<2;++i) {
            if (!IntroMovie_WaitBgScrollAnim(scroll_anims,layers[i])) abort();
            if (frame<80)
                IntroMovie_StartBgScroll_VBlank(&scroll_config,scroll_anims,layers[i],x,y,10);
            else
                IntroMovie_StartBgScroll_NotVBlank(&scroll_config,scroll_anims,layers[i],x,y,10);
        }
    }
    SysTaskQueue_RunTasks(gSystem.mainTaskQueue);
    if (frame==30 || frame==70 || frame==110 || frame==150) {
        int x=frame==30 ? -48 : frame==110 ? 37 : 0;
        int y=frame==30 ? 32 : frame==110 ? -24 : 0;
        for (unsigned i=0;i<2;++i) {
            const IntroMovieBgScrollAnim *anim=&scroll_anims[layers[i]];
            if (!IntroMovie_WaitBgScrollAnim(scroll_anims,layers[i]) || anim->task ||
                anim->active || !anim->finished || anim->counter!=10 ||
                Bg_GetXpos(&scroll_config,layers[i])!=x || Bg_GetYpos(&scroll_config,layers[i])!=y)
                abort();
        }
        printf("SCROLL completed frame=%u x=%d y=%d\n",frame,x,y); fflush(stdout);
    }
}
