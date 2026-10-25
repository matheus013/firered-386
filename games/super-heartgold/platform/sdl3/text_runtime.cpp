#include "text_runtime.hpp"
#include "hg/message_data.hpp"
#include "hg/resource_store.hpp"

#include <algorithm>
#include <array>
#include <stdexcept>

namespace hg::sdl {
namespace {
std::vector<std::byte> load_archive(const ResourceStore& store, const char* path) {
    auto file = store.open(path);
    if (file.size() > 16 * 1024 * 1024)
        throw std::runtime_error("Text archive exceeds diagnostic limit");
    std::vector<std::byte> bytes(static_cast<std::size_t>(file.size()));
    file.read(0, bytes);
    return bytes;
}
void pixel(RgbaImage& image, unsigned x, unsigned y, const std::array<std::uint8_t, 4>& color) {
    const auto at = (y * image.width + x) * 4;
    std::copy(color.begin(), color.end(), image.pixels.begin() + at);
}
}

TextRuntime::TextRuntime(SDL_Renderer* renderer, const std::filesystem::path& root,
                         unsigned bank, unsigned message) {
    const ResourceStore store(root);
    const auto fonts = load_archive(store, "a/0/1/6");
    const auto messages = load_archive(store, "a/0/2/7");
    const auto decoded = decode_message_data(narc_member(messages, bank), message);
    TextFlowSession::Options options;
    options.width = 224;
    options.height = 144;
    session_ = std::make_unique<TextFlowSession>(narc_member(fonts, 0), decoded.code_units, options);
    refresh_texture(renderer);
}

void TextRuntime::advance(const InputState& input, int steps) {
    pending_a_ = pending_a_ || input.was_pressed(Action::A);
    pending_b_ = pending_b_ || input.was_pressed(Action::B);
    // Keep edges from render frames with zero logical ticks. Consume once only;
    // held keys remain available for every following catch-up tick.
    for (int step = 0; step < steps; ++step) {
        session_->tick({pending_a_, pending_b_, input.is_down(Action::A), input.is_down(Action::B)});
        pending_a_ = pending_b_ = false;
    }
}

void TextRuntime::refresh_texture(SDL_Renderer* renderer) {
    const auto pixels = session_->pixels();
    if (texture_ && std::equal(pixels.begin(), pixels.end(), previous_pixels_.begin(), previous_pixels_.end()))
        return;
    RgbaImage image{256, 192, std::vector<std::uint8_t>(256 * 192 * 4)};
    constexpr std::array<std::uint8_t, 4> background{248, 250, 252, 255};
    constexpr std::array<std::array<std::uint8_t, 4>, 3> palette{{
        background, {12, 16, 24, 255}, {128, 144, 160, 255}
    }};
    for (unsigned y = 0; y < image.height; ++y) for (unsigned x = 0; x < image.width; ++x) {
        const bool panel = x >= 8 && x < 248 && y >= 8 && y < 184;
        const bool interior = x >= 10 && x < 246 && y >= 10 && y < 182;
        pixel(image, x, y, interior ? background : panel ?
              std::array<std::uint8_t, 4>{80, 176, 208, 255} :
              std::array<std::uint8_t, 4>{20, 26, 40, 255});
    }
    for (unsigned y = 0; y < session_->height(); ++y)
        for (unsigned x = 0; x < session_->width(); ++x)
            pixel(image, x + 16, y + 20, palette.at(pixels[y * session_->width() + x]));
    // Build first, then replace, preserving RAII on upload errors.
    auto next = make_preview_texture(renderer, image);
    texture_ = std::move(next);
    previous_pixels_.assign(pixels.begin(), pixels.end());
}

void TextRuntime::draw(SDL_Renderer* renderer, const SDL_FRect& destination, int height) {
    refresh_texture(renderer);
    if (destination.w > 0 && destination.h > 0 &&
        !SDL_RenderTexture(renderer, texture_.get(), nullptr, &destination))
        throw std::runtime_error(SDL_GetError());
    const auto state = session_->frame();
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDebugText(renderer, 8, static_cast<float>(std::max(0, height - 24)),
        "ORIGINAL C TEXT FLOW / HOST PIXELS - DIAGNOSTIC, CAMPAIGN PENDING");
    SDL_RenderDebugText(renderer, 8, static_cast<float>(std::max(0, height - 12)),
        state.completed ? "TEXT COMPLETE - NO GAME OR SAVE STARTED" :
        state.waiting_for_confirmation ? "X / Z: ADVANCE PAGE" : "X / Z: HOLD TO SPEED UP TEXT");
    if (state.arrow_requested && destination.w > 0 && destination.h > 0) {
        const float scale_x = destination.w / 256, scale_y = destination.h / 192;
        SDL_SetRenderDrawColor(renderer, 12, 16, 24, 255);
        for (unsigned row = 0; row < 4; ++row) {
            const SDL_FRect arrow{destination.x + (232 + static_cast<float>(row)) * scale_x,
                destination.y + (172 + static_cast<float>(row)) * scale_y,
                (8 - 2 * static_cast<float>(row)) * scale_x, scale_y};
            SDL_RenderFillRect(renderer, &arrow);
        }
    }
}

TextFlowSession::Frame TextRuntime::frame() const { return session_->frame(); }
} // namespace hg::sdl
