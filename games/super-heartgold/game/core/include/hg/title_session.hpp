#pragma once

#include <cstdint>
#include <memory>

namespace hg {
// Game-specific adapter for the original title decision function.
// Only the diagnostic 2D renderer is connected. Audio is counted, not played;
// palette fade timing approximates the DS service in host frames.
// The original C ABI has global state, so sessions are single-threaded and only
// one session can exist at a time, including after it reaches a destination.
class TitleSession {
public:
    enum Button : std::uint32_t {
        A = 1, B = 2, Select = 4, Start = 8,
        Up = 64, Down = 128, X = 1024, Y = 2048
    };
    enum Destination : unsigned {
        Unset = 0, Menu = 1, DeleteSave = 2, Intro = 3, Unused = 4, Microphone = 5
    };
    enum Phase : unsigned {
        WaitFade = 0, StartMusic = 1, Play = 2, ProceedFlash = 3,
        ProceedFlashAgain = 4, ProceedWithoutFlash = 5, FadeOut = 6
    };
    struct Frame {
        unsigned phase{}, initial_delay{}, play_frames{}, destination{};
        unsigned tint_rgb555{}, tint_alpha{};
        bool completed{}, prompt_visible{};
    };

    TitleSession();
    ~TitleSession();
    TitleSession(const TitleSession&) = delete;
    TitleSession& operator=(const TitleSession&) = delete;

    // pressed: new key edges; held: current keys. Destinations are diagnostic
    // results only: DeleteSave never reads, modifies or deletes a save file.
    Frame tick(std::uint32_t pressed = 0, std::uint32_t held = 0, bool touch = false);
    Frame frame() const;
    static unsigned allocations();

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};
}
