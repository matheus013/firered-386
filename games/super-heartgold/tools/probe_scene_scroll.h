/* Exercise original finite scroll tasks and the original BG commit paths. */
#include "intro_movie_internal.h"
static BgConfig scroll_config;
static IntroMovieBgScrollAnim scroll_anims[8];
static void update_scroll(unsigned frame) {
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
