#include "test.hpp"

#include "hg/fixed_step.hpp"
#include "hg/input.hpp"
#include "hg/log.hpp"
#include "hg/viewport.hpp"

#include <string>
#include <vector>

using namespace hg;

// --- viewport ---

TEST_CASE("viewport: stacked integer scale centers both screens")
{
    // 1280x720 com telas empilhadas (256x384): 720/384 = 1,875 -> escala inteira 1.
    const auto p = place_screens({1280, 720}, ScreenLayout::Stacked, ScaleMode::Integer);
    CHECK_EQ(p.scale, 1.0f);
    CHECK(p.top == (Rect{512, 168, 256, 192}));
    CHECK(p.bottom == (Rect{512, 360, 256, 192}));
}

TEST_CASE("viewport: stacked fit scale fills height without distortion")
{
    const auto p = place_screens({1280, 720}, ScreenLayout::Stacked, ScaleMode::Fit);
    CHECK_EQ(p.scale, 1.875f);
    CHECK_EQ(p.top.w, 480);
    CHECK_EQ(p.top.h, 360);
    CHECK_EQ(p.top.y, 0);
    CHECK_EQ(p.bottom.y + p.bottom.h, 720);
    CHECK_EQ(p.top.x, (1280 - 480) / 2);
}

TEST_CASE("viewport: side by side uses widescreen 1920x1080")
{
    // 512x192 lógico: 1920/512 = 3,75 e 1080/192 = 5,625 -> escala inteira 3.
    const auto p = place_screens({1920, 1080}, ScreenLayout::SideBySide, ScaleMode::Integer);
    CHECK_EQ(p.scale, 3.0f);
    CHECK(p.top == (Rect{192, 252, 768, 576}));
    CHECK(p.bottom == (Rect{960, 252, 768, 576}));
}

TEST_CASE("viewport: unified keeps the original 4:3 view and gives the sides to the HUD")
{
    // 1080/192 = 5,625 -> escala inteira 5 -> 1280x960, faixas de 320 px de cada lado.
    const auto p = place_screens({1920, 1080}, ScreenLayout::Unified, ScaleMode::Integer);
    CHECK_EQ(p.scale, 5.0f);
    CHECK(p.top == (Rect{320, 60, 1280, 960}));
    CHECK_EQ(p.bottom.w, 0);
    CHECK(p.hud_left == (Rect{0, 0, 320, 1080}));
    CHECK(p.hud_right == (Rect{1600, 0, 320, 1080}));
}

TEST_CASE("viewport: unified fit fills the height at 720p")
{
    const auto p = place_screens({1280, 720}, ScreenLayout::Unified, ScaleMode::Fit);
    CHECK_EQ(p.scale, 3.75f);
    CHECK(p.top == (Rect{160, 0, 960, 720}));
    CHECK(p.hud_left == (Rect{0, 0, 160, 720}));
    CHECK(p.hud_right == (Rect{1120, 0, 160, 720}));
}

TEST_CASE("viewport: unified on a window narrower than 4:3 leaves no side HUD")
{
    const auto p = place_screens({800, 900}, ScreenLayout::Unified, ScaleMode::Fit);
    CHECK_EQ(p.top.w, 800);
    CHECK_EQ(p.top.h, 600);
    CHECK_EQ(p.hud_left.w, 0);
    CHECK_EQ(p.hud_right.w, 0);
}

TEST_CASE("viewport: aspect ratio is always 4:3 per screen")
{
    const Size sizes[] = {{800, 600}, {1366, 768}, {2560, 1080}, {3024, 1964}, {300, 1000}};
    for (const auto s : sizes) {
        for (const auto layout : {ScreenLayout::Unified, ScreenLayout::Stacked, ScreenLayout::SideBySide}) {
            const auto p = place_screens(s, layout, ScaleMode::Fit);
            // w/h == 4/3, tolerando 1 pixel de arredondamento.
            const int diff = p.top.w * 3 - p.top.h * 4;
            CHECK(diff >= -4 && diff <= 4);
            CHECK(p.top.x >= 0 && p.top.y >= 0);
            CHECK(p.bottom.x + p.bottom.w <= s.w && p.bottom.y + p.bottom.h <= s.h);
        }
    }
}

TEST_CASE("viewport: integer mode falls back to fit when window is tiny")
{
    const auto p = place_screens({128, 192}, ScreenLayout::Stacked, ScaleMode::Integer);
    CHECK_EQ(p.scale, 0.5f);
    CHECK_EQ(p.top.w, 128);
}

TEST_CASE("viewport: gap separates screens and empty window yields nothing")
{
    const auto p = place_screens({256, 400}, ScreenLayout::Stacked, ScaleMode::Integer, 16);
    CHECK_EQ(p.bottom.y - (p.top.y + p.top.h), 16);
    const auto none = place_screens({0, 0}, ScreenLayout::Stacked, ScaleMode::Fit);
    CHECK_EQ(none.top.w, 0);
}

// --- input ---

TEST_CASE("input: bindings map codes to actions")
{
    InputBindings b;
    b.bind(120, Action::A);
    b.bind(122, Action::B);
    CHECK(b.lookup(120) == Action::A);
    CHECK(!b.lookup(999).has_value());
    b.bind(120, Action::Start); // reassociar substitui
    CHECK(b.lookup(120) == Action::Start);
    b.unbind(122);
    CHECK(!b.lookup(122).has_value());
    CHECK_EQ(b.size(), 1u);
}

TEST_CASE("input: pressed and released are edge-triggered per frame")
{
    InputState s;
    s.begin_frame();
    s.press(Action::A);
    CHECK(s.is_down(Action::A));
    CHECK(s.was_pressed(Action::A));

    s.begin_frame();
    s.press(Action::A); // repetição não gera nova borda
    CHECK(s.is_down(Action::A));
    CHECK(!s.was_pressed(Action::A));

    s.begin_frame();
    s.release(Action::A);
    CHECK(!s.is_down(Action::A));
    CHECK(s.was_released(Action::A));
    CHECK(!s.is_down(Action::B));
}

TEST_CASE("input: action names round-trip")
{
    for (std::size_t i = 0; i < kActionCount; ++i) {
        const auto a = static_cast<Action>(i);
        CHECK(action_from_name(action_name(a)) == a);
    }
    CHECK(!action_from_name("Turbo").has_value());
}

// --- fixed step ---

TEST_CASE("fixed_step: runs DS frames at about 59.83 Hz")
{
    FixedStep clock;
    int steps = 0;
    for (int i = 0; i < 1000; ++i) {
        steps += clock.advance(1'000'000); // 1 s em fatias de 1 ms
    }
    CHECK(steps == 59 || steps == 60);
}

TEST_CASE("fixed_step: speed multiplies steps without changing step size")
{
    FixedStep clock(10, 100);
    clock.set_speed(2);
    CHECK_EQ(clock.advance(100), 20);
    CHECK_EQ(clock.step_ns(), 10);
    clock.set_speed(0); // limitado a 1x
    CHECK_EQ(clock.speed(), 1);
}

TEST_CASE("fixed_step: long stalls are clamped")
{
    FixedStep clock(10, 4);
    CHECK_EQ(clock.advance(1'000'000), 4);
    CHECK(clock.advance(0) <= 1); // o atraso excedente foi descartado
}

// --- log ---

TEST_CASE("log: sink receives messages at or above min level")
{
    std::vector<std::string> got;
    set_log_sink([&](LogLevel level, std::string_view msg) {
        got.push_back(std::string(log_level_name(level)) + ":" + std::string(msg));
    });
    set_log_level(LogLevel::Info);
    log_debug("escondida");
    log_info("ola");
    log_error("erro");
    set_log_sink(nullptr);
    CHECK_EQ(got.size(), 2u);
    if (got.size() == 2) {
        CHECK_EQ(got[0], std::string("INFO:ola"));
        CHECK_EQ(got[1], std::string("ERROR:erro"));
    }
}
