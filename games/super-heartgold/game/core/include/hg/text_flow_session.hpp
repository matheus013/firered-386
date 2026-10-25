#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <span>

namespace hg {
// Private original-C RenderText decision flow with an authorial pixel window.
// No DS CPU, VRAM, global printer scheduler, script variables or sound playback.
class TextFlowSession {
public:
    struct Options {
        unsigned width = 224; // Whole tiles, 8..256 pixels.
        unsigned height = 64; // 1..192 pixels.
        unsigned character_delay = 1; // Original textSpeedBottom, 0..127.
        unsigned font_line_height = 16; // Original font 0 maxLetterHeight; fixture override allowed.
        unsigned line_spacing = 0;
        bool ab_speedup = true;
        bool auto_advance = false;
        bool auto_advance_speedup = false;
    };
    struct Input { bool a_pressed{}, b_pressed{}, a_held{}, b_held{}; };
    struct Frame {
        bool completed{};
        unsigned state{}, x{}, y{}, delay{}, scroll_remaining{};
        unsigned glyphs_drawn{}, clears{}, pixels_scrolled{}, confirmation_sound_requests{};
        bool waiting_for_confirmation{}, arrow_requested{};
    };

    TextFlowSession(std::span<const std::byte> font, std::span<const std::uint16_t> units,
        Options options);
    ~TextFlowSession();
    TextFlowSession(const TextFlowSession&) = delete;
    TextFlowSession& operator=(const TextFlowSession&) = delete;
    Frame tick(Input input);
    Frame frame() const;
    // Palette indices: 0 background, 1 foreground, 2 shadow. Borrowed until tick/destruction.
    std::span<const std::uint8_t> pixels() const;
    unsigned width() const;
    unsigned height() const;
private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};
} // namespace hg
