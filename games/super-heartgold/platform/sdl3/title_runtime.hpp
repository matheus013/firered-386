#pragma once
#include "hg/input.hpp"
#include "hg/title_session.hpp"
#include "hg/checksave_session.hpp"
#include "preview_texture.hpp"
#include <SDL3/SDL.h>
#include <array>
#include <filesystem>
#include <vector>

namespace hg::sdl {
// Presentation adapter for the private original-C title decision function.
class TitleRuntime {
public:
    TitleRuntime(SDL_Renderer* renderer, std::uint32_t controlled_flags,
                 const std::filesystem::path& text_root = {});
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
    std::array<std::vector<PreviewTexture>,6> messages_;
    std::uint32_t active_message_{ChecksaveSession::NoMessage};
    std::size_t message_page_{};
};
}
