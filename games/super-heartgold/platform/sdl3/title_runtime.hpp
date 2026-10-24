#pragma once
#include "hg/input.hpp"
#include "hg/title_session.hpp"
#include "hg/checksave_session.hpp"
#include <SDL3/SDL.h>

namespace hg::sdl {
// Presentation adapter for the private original-C title decision function.
class TitleRuntime {
public:
    explicit TitleRuntime(std::uint32_t controlled_flags=0) : controlled_flags_(controlled_flags) {}
    void advance(const InputState& input,int steps);
    void draw(SDL_Renderer* renderer,const SDL_FRect& image,int window_height) const;
    bool menu_handoff_complete() const;
private:
    TitleSession session_;
    TitleSession::Frame frame_;
    std::uint32_t pending_keys_{};
    std::uint32_t controlled_flags_{};
    std::unique_ptr<ChecksaveSession> checksave_;
    ChecksaveSession::Frame check_frame_;
};
}
