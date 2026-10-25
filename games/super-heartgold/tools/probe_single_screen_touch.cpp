#include "reference_compat/single_screen_touch.h"
#include <cassert>
#include <climits>
#include <cstdio>
int main() {
    unsigned short x=999,y=999;
    for(int py=0;py<384;++py) for(int px=0;px<512;++px) {
        assert(hg_single_screen_touch(1,1,px+100,py+50,100,50,512,384,&x,&y));
        assert(x==px/2 && y==py/2);
        assert(!hg_single_screen_touch(0,1,px,py,0,0,512,384,&x,&y));
    }
    assert(!hg_single_screen_touch(1,1,512,0,0,0,512,384,&x,&y));
    assert(!hg_single_screen_touch(1,1,0,384,0,0,512,384,&x,&y));
    assert(!hg_single_screen_touch(1,1,99,50,100,50,512,384,&x,&y));
    assert(!hg_single_screen_touch(1,0,100,50,100,50,512,384,&x,&y));
    assert(!hg_single_screen_touch(1,1,0,0,0,0,0,384,&x,&y));
    assert(!hg_single_screen_touch(1,1,INT_MAX,INT_MAX,INT_MIN,INT_MIN,512,384,&x,&y));
    assert(hg_single_screen_touch(1,1,0,0,0,0,1,1,&x,&y));
    assert(x==0 && y==0);
    std::puts("single-screen touch passed: 196608 visible and hidden pixel positions plus bounds");
}
