#pragma once

#include "hg/input.hpp"
#include "hg/text_flow_session.hpp"
#include "preview_texture.hpp"

#include <filesystem>
#include <memory>
#include <vector>

namespace hg::sdl {

// SDL presentation of the private original-C printer, with an authorial frame.
class TextRuntime {
public:
    TextRuntime(SDL_Renderer* renderer, const std::filesystem::path& root,
                unsigned bank, unsigned message);
    void advance(const InputState& input, int steps);
    void draw(SDL_Renderer* renderer, const SDL_FRect& destination, int window_height);
    TextFlowSession::Frame frame() const;
private:
    void refresh_texture(SDL_Renderer* renderer);
    std::unique_ptr<TextFlowSession> session_;
    PreviewTexture texture_;
    std::vector<std::uint8_t> previous_pixels_;
    bool pending_a_{}, pending_b_{};
};

} // namespace hg::sdl
