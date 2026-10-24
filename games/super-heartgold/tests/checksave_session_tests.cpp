#include "hg/checksave_session.hpp"
#include <iostream>
#include <stdexcept>
#include <vector>

namespace {
using Session = hg::ChecksaveSession;
void require(bool passed, const char* description) {
    if (!passed) throw std::runtime_error(description);
}
std::vector<std::uint32_t> run(Session& session) {
    std::vector<std::uint32_t> messages;
    bool was_waiting = false;
    for (unsigned tick = 0; tick < 300; ++tick) {
        const auto frame = session.tick(was_waiting);
        if (frame.awaiting_ack && !was_waiting) messages.push_back(frame.message);
        was_waiting = frame.awaiting_ack;
        if (frame.completed) return messages;
    }
    throw std::runtime_error("Save-status decision exceeded finite tick bound");
}
void no_flags() {
    Session session(0);
    require(run(session).empty(), "Zero controlled flags displayed an error");
    const auto completed = session.frame();
    require(completed.completed && completed.state == Session::Exit, "Zero flags did not exit");
    const auto repeated = session.tick(true);
    require(repeated.completed && repeated.state == completed.state &&
            repeated.message == completed.message && repeated.remaining_flags == 0,
            "Completed session restarted");
}
void every_flag_and_priority() {
    const std::uint32_t expected[] = {0, 1, 4, 5, 2, 3};
    for (unsigned bit = 0; bit < 6; ++bit) {
        Session session(1u << bit);
        require(run(session) == std::vector<std::uint32_t>{expected[bit]}, "Single bit selected wrong original message");
        require(session.frame().remaining_flags == 0, "Single known bit was not consumed");
    }
    for (unsigned bit : {0u, 2u, 4u}) {
        Session session(3u << bit);
        require(run(session) == std::vector<std::uint32_t>{expected[bit + 1]}, "High bit must consume its lower paired bit");
    }
    Session all(63);
    require(run(all) == std::vector<std::uint32_t>{1, 5, 3}, "All flags violated original message priority");
    Session lower(21);
    require(run(lower) == std::vector<std::uint32_t>{0, 4, 2}, "Recoverable flags violated original priority");
}
void acknowledgement_and_timing() {
    Session session(1);
    auto frame = session.tick(true);
    require(frame.state == Session::SelectMessage, "Initial flag check skipped selection");
    frame = session.tick(true);
    require(frame.state == Session::FadeIn && !frame.awaiting_ack, "Selection skipped fade");
    frame = session.tick(true);
    require(frame.state == Session::WaitFadeIn, "Fade did not begin");
    for (unsigned tick = 0; tick < 5; ++tick) {
        frame = session.tick(true);
        require(frame.state == Session::WaitFadeIn, "Original six-frame fade ended early");
    }
    require(session.tick(true).state == Session::PrintMessage, "Six-frame fade did not finish");
    frame = session.tick(true);
    require(frame.awaiting_ack && frame.message == 0, "Early acknowledgement skipped message");
    for (unsigned tick = 0; tick < 40; ++tick) {
        frame = session.tick();
        require(frame.awaiting_ack && !frame.completed && frame.message == 0, "Message did not wait for acknowledgement");
    }
    frame = session.tick(true);
    require(!frame.awaiting_ack && frame.state == Session::WaitFadeOut && frame.message == Session::NoMessage,
            "Acknowledgement did not start outgoing fade");
    require(run(session).empty(), "Acknowledged message reappeared");
}
void unknown_bits_and_session_isolation() {
    Session unknown(0x80000000u);
    require(run(unknown).empty() && unknown.frame().remaining_flags == 0x80000000u,
            "Unknown flags must finish without fabricating a message or clearing flags");
    Session mixed(0x80000001u);
    require(run(mixed) == std::vector<std::uint32_t>{0} && mixed.frame().remaining_flags == 0x80000000u,
            "Known plus unknown flags did not terminate correctly");
    Session first(1), second(32);
    for (unsigned tick = 0; tick < 25; ++tick) {
        first.tick();
        second.tick();
    }
    require(first.frame().message == 0 && second.frame().message == 3, "Interleaved sessions contaminated state");
    require(first.frame().awaiting_ack && second.frame().awaiting_ack, "Session fixture unexpectedly acknowledged");
    run(first);
    require(second.frame().awaiting_ack, "Finishing one session changed the other");
    run(second);
}
}
int main() {
    try {
        no_flags();
        every_flag_and_priority();
        acknowledgement_and_timing();
        unknown_bits_and_session_isolation();
        std::cout << "Original check-save decision: controlled flags, priority, acknowledgement, timing and isolation PASS\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "Controlled check-save decision failed: " << error.what() << '\n';
        return 1;
    }
}
