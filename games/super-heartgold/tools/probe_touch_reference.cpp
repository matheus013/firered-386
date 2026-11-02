// Compare our native touch bridge with ssrecomp's external layout implementation.
#include "layout.h"
#include "single_screen_touch.h"
#include <cstdio>
#include <cstdlib>
#include <vector>
int main() {
    const int sizes[][2]={{256,192},{512,384},{1280,720},{1920,1080},{320,240},{100,100},{320,568},{1,1}};
    unsigned checks=0;
    for (auto &size:sizes) for (int mode=0;mode<6;++mode) for (int swap=0;swap<2;++swap) {
        Layout layout; layout.mode=(LayoutMode)mode; layout.swapped=swap;
        SDL_Rect rects[2]; LayoutRects(layout,size[0],size[1],rects);
        const SDL_Rect &r=rects[1];
        std::vector<int> xs={-1,0,size[0]-1,size[0],r.x-1,r.x,r.x+r.w/2,r.x+r.w-1,r.x+r.w};
        std::vector<int> ys={-1,0,size[1]-1,size[1],r.y-1,r.y,r.y+r.h/2,r.y+r.h-1,r.y+r.h};
        for(int i=0;i<=16;++i) { xs.push_back(i*size[0]/16); ys.push_back(i*size[1]/16); }
        for(int x:xs) for(int y:ys) {
            int ex=-1,ey=-1; unsigned short ax=65535,ay=65535;
            bool expected=LayoutTouch(r,x,y,ex,ey);
            bool actual=hg_single_screen_touch(1,1,x,y,r.x,r.y,r.w,r.h,&ax,&ay);
            if(actual!=expected || (actual && (ax!=ex || ay!=ey))) return 1;
            if(hg_single_screen_touch(0,1,x,y,r.x,r.y,r.w,r.h,&ax,&ay)) return 2;
            if(hg_single_screen_touch(1,0,x,y,r.x,r.y,r.w,r.h,&ax,&ay)) return 3;
            ++checks;
        }
    }
    std::printf("%u\n",checks);
}
