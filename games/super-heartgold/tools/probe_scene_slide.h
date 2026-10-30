/* Scene 4 window/scroll parameters, using original helpers together. */
static IntroMovieBgWindowAnim slide_windows[2];
static void prepare_slide(void) {
    for (unsigned i=0;i<2;++i) {
        GFBgLayer layer=i ? GF_BG_LYR_SUB_1 : GF_BG_LYR_MAIN_1;
        BgSetPosTextAndCommit(&scroll_config,layer,BG_POS_OP_SET_X,0);
        IntroMovie_StartBgScroll_VBlank(&scroll_config,scroll_anims,layer,i ? -192 : 192,0,0);
        IntroMovie_WindowsOn_SetInsideOutsidePlanes(30,28,1,1,i ? PM_LCD_BOTTOM : PM_LCD_TOP);
        IntroMovie_SetBgWindowsPosition(255,0,255,192,i ? PM_LCD_BOTTOM : PM_LCD_TOP);
    }
    G2_SetWnd1Position(0,0,0,0); G2S_SetWnd1Position(0,0,0,0);
}
static void update_slide(unsigned frame) {
    if (frame==80) prepare_slide();
    if (frame==1 || frame==40 || frame==80 || frame==120) {
        BOOL opening=frame==1 || frame==80;
        IntroMovieBgWindowAnimParam main_open={255,0,255,192,0,0,255,192,30,28,1,1};
        IntroMovieBgWindowAnimParam sub_open={0,0,0,192,0,0,255,192,30,28,1,1};
        IntroMovieBgWindowAnimParam main_close={0,0,255,192,0,0,0,192,30,28,1,1};
        IntroMovieBgWindowAnimParam sub_close={0,0,255,192,255,0,255,192,30,28,1,1};
        if (slide_windows[0].active || slide_windows[1].active) abort();
        IntroMovie_StartWindowPanEffect(slide_windows,10,PM_LCD_TOP,opening ? &main_open : &main_close);
        IntroMovie_StartWindowPanEffect(slide_windows,10,PM_LCD_BOTTOM,opening ? &sub_open : &sub_close);
        IntroMovie_StartBgScroll_VBlank(&scroll_config,scroll_anims,GF_BG_LYR_MAIN_1,-192,0,10);
        IntroMovie_StartBgScroll_VBlank(&scroll_config,scroll_anims,GF_BG_LYR_SUB_1,192,0,10);
    }
    if (frame==30 || frame==70 || frame==110 || frame==150) {
        BOOL opening=frame==30 || frame==110;
        for (unsigned i=0;i<2;++i) {
            GFBgLayer layer=i ? GF_BG_LYR_SUB_1 : GF_BG_LYR_MAIN_1;
            const IntroMovieBgScrollAnim *anim=&scroll_anims[layer];
            if (anim->active || anim->task || !anim->finished || anim->counter!=10 ||
                Bg_GetXpos(&scroll_config,layer)!=(opening ? 0 : i ? 192 : -192) ||
                slide_windows[i].active || slide_windows[i].task || slide_windows[i].counter!=10) abort();
        }
        printf("SCROLL completed frame=%u slide=1\n",frame); fflush(stdout);
    }
}
