#pragma once
#include "hg/input.hpp"
#include "hg/title_session.hpp"
#include <SDL3/SDL.h>

namespace hg::sdl {
// Presentation adapter for the private original-C title decision function.
class TitleRuntime {
public:
    void advance(const InputState& input,int steps);
    void draw(SDL_Renderer* renderer,const SDL_FRect& image,int window_height) const;
    bool menu_handoff_complete() const;
private:
    TitleSession session_;
    TitleSession::Frame frame_;
    std::uint32_t pending_keys_{};
};
}
