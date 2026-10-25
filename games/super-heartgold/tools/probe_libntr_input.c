#define _GNU_SOURCE
#include <SDL2/SDL.h>
#include <dlfcn.h>
// Deterministic mouse position with events processed by the real SDL loop.
Uint32 SDL_GetMouseState(int *x,int *y) {
    if(x) *x=256;
    if(y) *y=192;
    return 0;
}
void SDL_GL_SwapWindow(SDL_Window *window) {
    static unsigned frame;
    void (*swap)(SDL_Window*)=dlsym(RTLD_NEXT,"SDL_GL_SwapWindow");
    swap(window);
    ++frame;
    SDL_Event event={0};
    if(frame==10 || frame==30) {
        event.type=frame==10?SDL_MOUSEBUTTONDOWN:SDL_MOUSEBUTTONUP;
        event.button.windowID=SDL_GetWindowID(window);
        event.button.button=SDL_BUTTON_LEFT;
        event.button.state=frame==10?SDL_PRESSED:SDL_RELEASED;
        event.button.x=256;event.button.y=192;
        SDL_PushEvent(&event);
    }
    if(frame==50) {
        event.type=SDL_WINDOWEVENT;
        event.window.windowID=SDL_GetWindowID(window);
        event.window.event=SDL_WINDOWEVENT_CLOSE;
        SDL_PushEvent(&event);
    }
}
