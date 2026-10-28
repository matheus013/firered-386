#define _GNU_SOURCE
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#ifndef HG_CAPTURE_LAST_FRAME
#define HG_CAPTURE_LAST_FRAME 120
#endif
// Linux-only diagnostic interposer. No game assets or runtime dependency.
void SDL_GL_SwapWindow(SDL_Window *window) {
    static unsigned frame;
    void (*swap)(SDL_Window*) = dlsym(RTLD_NEXT,"SDL_GL_SwapWindow");
    ++frame;
    if (frame==60 || frame==120 || (HG_CAPTURE_LAST_FRAME > 120 && frame % 20 == 0)) {
        int w,h; SDL_GL_GetDrawableSize(window,&w,&h);
        if(w<=0 || h<=0 || w>4096 || h>4096) abort();
        unsigned char *pixels=malloc((size_t)w*h*3);
        if(!pixels) abort();
        void (*readpixels)(GLint,GLint,GLsizei,GLsizei,GLenum,GLenum,void*)=SDL_GL_GetProcAddress("glReadPixels");
        void (*pack)(GLenum,GLint)=SDL_GL_GetProcAddress("glPixelStorei");
        pack(GL_PACK_ALIGNMENT,1);
        readpixels(0,0,w,h,GL_RGB,GL_UNSIGNED_BYTE,pixels);
        char name[64]; snprintf(name,sizeof(name),"libntr-frame-%u.ppm",frame);
        FILE *file=fopen(name,"wb"); if(!file) abort();
        fprintf(file,"P6\n%d %d\n255\n",w,h);
        for(int y=h-1;y>=0;--y) if(fwrite(pixels+(size_t)y*w*3,3,w,file)!=(size_t)w) abort();
        fclose(file); free(pixels);
        fprintf(stderr,"HG_CAPTURE frame=%u size=%dx%d\n",frame,w,h);
    }
    swap(window);
    if(frame==HG_CAPTURE_LAST_FRAME) { SDL_Event event={0}; event.type=SDL_WINDOWEVENT; event.window.windowID=SDL_GetWindowID(window); event.window.event=SDL_WINDOWEVENT_CLOSE; SDL_PushEvent(&event); }
}
