// Exercises the pinned original C decision function through the host adapter.
#include "hg/title_session.hpp"

#include <iostream>
#include <stdexcept>
#include <string>

namespace {
using Session = hg::TitleSession;
using Frame = Session::Frame;

void require(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
}

Frame reach_play(Session& session) {
    for (unsigned count = 0; count < 10; ++count) {
        const auto frame = session.tick();
        if (frame.phase == Session::Play) return frame;
    }
    throw std::runtime_error("Original controller failed to enter PLAY");
}

Frame ready(Session& session) {
    auto frame = reach_play(session);
    require(frame.initial_delay == 30, "PLAY must start with the original 30-frame delay");
    for (unsigned count = 0; count < 30; ++count) frame = session.tick();
    require(frame.initial_delay == 0 && frame.play_frames == 0, "Initial delay changed play timer");
    return frame;
}

Frame finish(Session& session, unsigned expected_destination, unsigned limit = 100) {
    auto frame = session.frame();
    for (unsigned count = 0; !frame.completed && count < limit; ++count) frame = session.tick();
    require(frame.completed, "Original exit/fade did not complete within its bound");
    require(frame.destination == expected_destination, "Exit callback lost or changed the destination");
    require(!frame.prompt_visible, "Completed session kept the start prompt visible");
    require(Session::allocations() == 0, "Completed original overlay leaked allocations");
    const auto repeated = session.tick(Session::Start, Session::B | Session::Select | Session::Up, true);
    require(repeated.completed && repeated.destination == frame.destination &&
            repeated.phase == frame.phase && repeated.play_frames == frame.play_frames &&
            repeated.tint_alpha == frame.tint_alpha && repeated.tint_rgb555 == frame.tint_rgb555,
            "Tick after completion restarted or mutated the session");
    return frame;
}

void initial_delay_and_input_edges() {
    Session session;
    auto frame = reach_play(session);
    for (unsigned count = 0; count < 30; ++count) {
        frame = session.tick(Session::Start, Session::B | Session::Select | Session::Up, true);
        require(frame.destination == Session::Unset && !frame.prompt_visible,
                "Original initial delay accepted input or showed the prompt");
        require(frame.initial_delay == 29 - count, "Initial delay did not count down one frame at a time");
    }
    frame = session.tick(0, Session::A | Session::Start);
    require(frame.destination == Session::Unset && frame.play_frames == 1,
            "Held A/Start must not act as newly pressed keys");
    for (unsigned count = 0; !frame.prompt_visible && count < 45; ++count) frame = session.tick();
    require(frame.prompt_visible, "Start instruction never blinked after the delay");
    session.tick(Session::Start);
    finish(session, Session::Menu);
}

void menu_inputs_and_fades() {
    for (const auto input : {Session::A, Session::Start}) {
        Session session;
        ready(session);
        auto frame = session.tick(input);
        require(frame.destination == Session::Menu && frame.phase == Session::ProceedFlash,
                "New A/Start must select MENU through the flash phase");
        require(frame.tint_rgb555 == 32767 && frame.tint_alpha == 0 && !frame.prompt_visible,
                "Menu flash did not start from transparent white");
        for (unsigned count = 0; count < 4; ++count) {
            frame = session.tick();
            require(frame.phase == Session::ProceedFlash && frame.tint_alpha > 0,
                    "Five-frame outgoing flash finished too early");
        }
        frame = session.tick();
        require(frame.phase == Session::ProceedFlashAgain && frame.tint_alpha == 255,
                "White flash must switch to its incoming fade after five frames");
        for (unsigned count = 0; count < 12; ++count) frame = session.tick();
        require(frame.tint_alpha == 0 && !frame.completed,
                "Incoming flash did not fade away or skipped the sound timer");
        for (unsigned count = 0; frame.phase != Session::FadeOut && count < 60; ++count) frame = session.tick();
        require(frame.phase == Session::FadeOut && frame.tint_rgb555 == 0 && frame.tint_alpha == 0,
                "Menu must wait for sound then start a black fade");
        finish(session, Session::Menu);
    }
    Session session;
    ready(session);
    require(session.tick(0, 0, true).destination == Session::Menu, "Touch did not select MENU");
    finish(session, Session::Menu);
}

void diagnostic_destinations_and_priority() {
    constexpr auto delete_keys = Session::B | Session::Select | Session::Up;
    constexpr auto microphone_keys = Session::X | Session::Y | Session::Down;
    for (const auto destination : {Session::DeleteSave, Session::Microphone}) {
        Session session;
        ready(session);
        const auto keys = destination == Session::DeleteSave ? delete_keys : microphone_keys;
        const auto frame = session.tick(0, keys);
        require(frame.destination == destination && frame.phase == Session::FadeOut,
                "Diagnostic key combination selected an incorrect destination/phase");
        require(frame.tint_rgb555 == 0 && !frame.prompt_visible, "Diagnostic exit must hide prompt and fade black");
        // This adapter never opens a save or microphone; only the destination is tested.
        finish(session, destination, 10);
    }
    {
        Session session;
        ready(session);
        require(session.tick(Session::Start, delete_keys | microphone_keys).destination == Session::Menu,
                "Start must have priority over both held-key combinations");
        finish(session, Session::Menu);
    }
    {
        Session session;
        ready(session);
        require(session.tick(0, delete_keys | microphone_keys).destination == Session::DeleteSave,
                "Clear-save combination must have original priority over microphone");
        finish(session, Session::DeleteSave, 10);
    }
    {
        Session session;
        ready(session);
        require(session.tick(0, Session::B | Session::Up).destination == Session::Unset,
                "Partial combination incorrectly selected a destination");
    }
}

void original_timeout() {
    Session session;
    ready(session);
    Frame frame;
    for (unsigned count = 0; count < 2340; ++count) frame = session.tick();
    require(frame.play_frames == 2340 && frame.destination == Session::Unset,
            "Timeout must not occur at or before 2340 play frames");
    frame = session.tick();
    require(frame.play_frames == 2341 && frame.destination == Session::Intro &&
            frame.phase == Session::ProceedWithoutFlash && !frame.prompt_visible,
            "Original timeout must select intro after 2340 frames without flash");
    finish(session, Session::Intro);
}

void lifetime_and_exclusive_session() {
    require(Session::allocations() == 0, "Previous test leaked original C allocations");
    for (unsigned cancellation_frame : {0u, 1u, 3u, 40u}) {
        {
            Session session;
            for (unsigned frame = 0; frame < cancellation_frame; ++frame) session.tick();
            bool rejected = false;
            try { Session duplicate; } catch (const std::logic_error&) { rejected = true; }
            require(rejected, "Concurrent session must be rejected without resetting the active context");
            require(!session.frame().completed, "Rejected constructor corrupted the first session");
        }
        require(Session::allocations() == 0, "Early cancellation leaked original overlay/data");
    }
    {
        Session session;
        ready(session);
        session.tick(Session::Start);
        finish(session, Session::Menu);
        bool rejected = false;
        try { Session duplicate; } catch (const std::logic_error&) { rejected = true; }
        require(rejected, "Completed but live session must retain exclusive ownership");
    }
    Session next;
    require(next.frame().destination == Session::Unset && next.frame().play_frames == 0,
            "Sequential session inherited previous controller state");
    ready(next);
    next.tick(Session::Start);
    finish(next, Session::Menu);
}
}

int main() {
    try {
        initial_delay_and_input_edges();
        menu_inputs_and_fades();
        diagnostic_destinations_and_priority();
        original_timeout();
        lifetime_and_exclusive_session();
        require(Session::allocations() == 0, "Final original C allocation count is not zero");
        std::cout << "Original title C controller: input priority, delay, fades, timeout and RAII PASS\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "Title session test failed: " << error.what() << '\n';
        return 1;
    }
}
