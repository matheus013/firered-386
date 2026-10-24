#include "hg/checksave_session.hpp"
#include "checksave_bridge.h"
#include <algorithm>
#include <cstdlib>

namespace {
struct HostContext {
    unsigned fade_remaining{};
    std::uint32_t message{hg::ChecksaveSession::NoMessage};
    bool awaiting_ack{};
    bool acknowledge{};
};
thread_local HostContext* active_context{};
HostContext& context() {
    if (!active_context) std::abort();
    return *active_context;
}
class ContextScope {
public:
    explicit ContextScope(HostContext& current) : previous_(active_context) { active_context = &current; }
    ~ContextScope() { active_context = previous_; }
    ContextScope(const ContextScope&) = delete;
    ContextScope& operator=(const ContextScope&) = delete;
private:
    HostContext* previous_;
};
}

extern "C" {
u32 HGCheck_GetStatusFlags(HGCheck_ControlledFlags* flags) { return flags->status; }
void HGCheck_BeginFade(int, int, int, int, int duration, int, int) {
    context().fade_remaining = static_cast<unsigned>(std::max(1, duration));
}
BOOL HGCheck_FadeFinished() { return context().fade_remaining == 0; }
void HGCheck_SetMaskColor(int, int) {}
BOOL HGCheck_PrintMessage(HGCheck_Data*, u32 message, BOOL skip_waiting, u32) {
    auto& host = context();
    // Host text is instantaneous; acknowledgement starts on the next tick.
    // This substitutes only the printer service, not original decision logic.
    if (!host.awaiting_ack) {
        host.message = message;
        host.awaiting_ack = true;
        return FALSE;
    }
    if (!skip_waiting && !host.acknowledge) return FALSE;
    host.awaiting_ack = false;
    host.message = hg::ChecksaveSession::NoMessage;
    return TRUE;
}
}

namespace hg {
struct ChecksaveSession::Impl {
    HGCheck_ControlledFlags fixture;
    HGCheck_Data data{};
    HostContext host;
    Frame current;

    explicit Impl(std::uint32_t flags) : fixture{flags} {
        data.saveData = &fixture;
        current.remaining_flags = flags;
    }

    Frame tick(bool acknowledge) {
        if (current.completed) return current;
        if (host.fade_remaining) --host.fade_remaining;
        host.acknowledge = acknowledge;
        ContextScope scope(host);
        current.completed = HGCheck_Run(&data) != FALSE;
        current.state = static_cast<unsigned>(data.mainState);
        current.message = host.message;
        current.remaining_flags = data.saveStatusFlags;
        current.awaiting_ack = host.awaiting_ack;
        return current;
    }
};
ChecksaveSession::ChecksaveSession(std::uint32_t flags) : impl_(std::make_unique<Impl>(flags)) {}
ChecksaveSession::~ChecksaveSession() = default;
ChecksaveSession::Frame ChecksaveSession::tick(bool acknowledge) { return impl_->tick(acknowledge); }
ChecksaveSession::Frame ChecksaveSession::frame() const { return impl_->current; }
}
