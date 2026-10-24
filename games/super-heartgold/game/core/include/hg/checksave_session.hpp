#pragma once
#include <cstdint>
#include <memory>

namespace hg {
// Original save-status decision flow with CONTROLLED flags. This does not read,
// parse, validate, recover, erase, or write a save. Text/fades use host timing.
class ChecksaveSession {
public:
    static constexpr std::uint32_t NoMessage = UINT32_MAX;
    enum State : unsigned {
        CheckFlags, SelectMessage, FadeIn, WaitFadeIn,
        PrintMessage, WaitFadeOut, Exit
    };
    struct Frame {
        bool completed{};
        unsigned state{};
        std::uint32_t message{NoMessage};
        std::uint32_t remaining_flags{};
        bool awaiting_ack{};
    };

    explicit ChecksaveSession(std::uint32_t controlled_status_flags);
    ~ChecksaveSession();
    ChecksaveSession(const ChecksaveSession&) = delete;
    ChecksaveSession& operator=(const ChecksaveSession&) = delete;
    Frame tick(bool acknowledge = false);
    Frame frame() const;

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};
}
