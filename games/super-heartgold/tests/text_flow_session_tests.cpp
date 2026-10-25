#include "test.hpp"
#include "hg/text_flow_session.hpp"

#include <algorithm>
#include <stdexcept>
#include <vector>

namespace {
using Units = std::vector<std::uint16_t>;
std::vector<std::byte> font() {
    std::vector<std::byte> bytes(50);
    const auto put = [&](unsigned offset, unsigned value) {
        for (unsigned i = 0; i < 4; ++i) bytes[offset+i] = std::byte((value >> (8*i)) & 255);
    };
    put(0,16); put(4,48); put(8,2);
    bytes[12] = bytes[13] = std::byte{8};
    bytes[14] = bytes[15] = std::byte{1};
    std::fill(bytes.begin()+16,bytes.begin()+32,std::byte{0x55});
    std::fill(bytes.begin()+32,bytes.begin()+48,std::byte{0xaa});
    bytes[48] = bytes[49] = std::byte{8};
    return bytes;
}
hg::TextFlowSession::Options immediate() {
    hg::TextFlowSession::Options options;
    options.character_delay = 0;
    return options;
}
template<class F> bool rejects(F&& action) {
    try { action(); return false; } catch (const std::runtime_error&) { return true; }
}
}

TEST_CASE("text flow: original character delay and finish are tick exact") {
    hg::TextFlowSession session(font(),Units{1,2,0xffff},{});
    CHECK_EQ(session.tick({}).glyphs_drawn,1u);
    CHECK_EQ(session.tick({}).glyphs_drawn,1u);
    const auto third = session.tick({});
    CHECK_EQ(third.glyphs_drawn,2u); CHECK_EQ(third.x,16u);
    CHECK(!session.tick({}).completed);
    CHECK(session.tick({}).completed);
    CHECK(session.tick({true}).completed);
    CHECK_EQ(session.frame().glyphs_drawn,2u);
}

TEST_CASE("text flow: clear waits for new input then resets actual pixels") {
    hg::TextFlowSession session(font(),Units{1,0x25bc,2,0xffff},immediate());
    session.tick({});
    CHECK_EQ(session.pixels()[0],1);
    CHECK_EQ(session.tick({}).state,2u);
    CHECK(session.tick({}).arrow_requested);
    CHECK_EQ(session.tick({false,false,true,false}).clears,0u);
    const auto confirmed = session.tick({true});
    CHECK_EQ(confirmed.clears,1u); CHECK_EQ(confirmed.x,0u);
    CHECK(!confirmed.arrow_requested);
    CHECK_EQ(confirmed.confirmation_sound_requests,1u);
    CHECK(std::all_of(session.pixels().begin(), session.pixels().end(), [](auto p) { return p == 0; }));
    session.tick({}); CHECK_EQ(session.pixels()[0],2);
    CHECK(session.tick({}).completed);
}

TEST_CASE("text flow: LF and scroll move window pixels four rows per tick") {
    auto options = immediate(); options.width = 8; options.height = 16; options.font_line_height = 8;
    hg::TextFlowSession session(font(),Units{1,0xe000,2,0x25bd,0xffff},options);
    session.tick({}); const auto second = session.tick({});
    CHECK_EQ(second.y,8u); CHECK_EQ(second.glyphs_drawn,2u);
    CHECK_EQ(session.pixels()[0],1); CHECK_EQ(session.pixels()[64],2);
    CHECK_EQ(session.tick({}).state,3u);
    CHECK_EQ(session.tick({true}).scroll_remaining,8u);
    CHECK_EQ(session.tick({}).pixels_scrolled,4u);
    const auto scrolled = session.tick({});
    CHECK_EQ(scrolled.pixels_scrolled,8u); CHECK_EQ(scrolled.state,4u);
    CHECK_EQ(session.pixels()[0],2); CHECK_EQ(session.pixels()[64],0);
    CHECK_EQ(session.tick({}).state,0u);
    CHECK(session.tick({}).completed);
}

TEST_CASE("text flow: original delay control has a separate resume tick") {
    hg::TextFlowSession session(font(),Units{0xfffe,0x201,1,2,1,0xffff},immediate());
    CHECK_EQ(session.tick({}).delay,2u);
    CHECK_EQ(session.tick({}).delay,1u);
    CHECK_EQ(session.tick({}).delay,0u);
    const auto resumed = session.tick({});
    CHECK_EQ(resumed.state,0u); CHECK_EQ(resumed.glyphs_drawn,0u);
    CHECK_EQ(session.tick({}).glyphs_drawn,1u);
    CHECK(session.tick({}).completed);
}

TEST_CASE("text flow: extended page control consumes its following LF") {
    hg::TextFlowSession session(font(),Units{0xfffe,0x207,0,0xe000,1,0xffff},immediate());
    CHECK_EQ(session.tick({}).state,7u);
    CHECK_EQ(session.tick({true}).state,0u);
    const auto drawn = session.tick({});
    CHECK_EQ(drawn.y,0u); CHECK_EQ(drawn.glyphs_drawn,1u);
}

TEST_CASE("text flow: auto advance waits 100 increments and releases on next tick") {
    auto options = immediate(); options.auto_advance = true;
    hg::TextFlowSession session(font(),Units{0x25bc,1,0xffff},options);
    CHECK_EQ(session.tick({}).state,2u);
    for (unsigned i = 0; i < 100; ++i) CHECK_EQ(session.tick({}).state,2u);
    CHECK_EQ(session.tick({}).state,0u);
    CHECK_EQ(session.frame().confirmation_sound_requests,0u);
    CHECK_EQ(session.tick({}).glyphs_drawn,1u);
}

TEST_CASE("text flow: AB acceleration distinguishes pressed from held input") {
    hg::TextFlowSession session(font(),Units{1,2,1,0xffff},{});
    session.tick({});
    CHECK_EQ(session.tick({false,false,true,false}).glyphs_drawn,1u);
    session.tick({});
    CHECK_EQ(session.tick({true,false,true,false}).glyphs_drawn,2u);
    CHECK_EQ(session.tick({false,false,true,false}).glyphs_drawn,3u);
    CHECK(session.tick({false,false,true,false}).completed);
}

TEST_CASE("text flow: interleaved sessions isolate flags pixels and cursors") {
    hg::TextFlowSession first(font(),Units{1,0x25bc,1,0xffff},immediate());
    hg::TextFlowSession second(font(),Units{2,2,0xffff},immediate());
    first.tick({}); second.tick({}); first.tick({}); second.tick({});
    CHECK_EQ(first.frame().glyphs_drawn,1u); CHECK_EQ(second.frame().glyphs_drawn,2u);
    CHECK_EQ(first.pixels()[0],1); CHECK_EQ(second.pixels()[0],2);
    CHECK_EQ(first.tick({true}).clears,1u);
    CHECK_EQ(second.frame().clears,0u);
    CHECK(second.tick({}).completed);
}

TEST_CASE("text flow: alternate wait commands and skipped marker operands remain bounded") {
    for (const auto field : {0xfe00u,0xfe01u}) {
        hg::TextFlowSession session(font(),Units{0xfffe,0xfe06,1,static_cast<std::uint16_t>(field),0xffff},immediate());
        CHECK_EQ(session.tick({}).state,field == 0xfe00 ? 3u : 2u);
        CHECK_EQ(session.tick({true}).state,field == 0xfe00 ? 4u : 0u);
    }
    hg::TextFlowSession skipped(font(),Units{0xf0fd,0xffff,1,0xffff},immediate());
    CHECK_EQ(skipped.tick({}).glyphs_drawn,1u);
    CHECK(skipped.tick({}).completed);
    hg::TextFlowSession scroll(font(),Units{0xfffe,0x208,0,0xe000,1,0xffff},immediate());
    CHECK_EQ(scroll.tick({}).state,8u);
    CHECK_EQ(scroll.tick({true}).scroll_remaining,16u);
    for (unsigned i = 0; i < 5; ++i) scroll.tick({});
    CHECK_EQ(scroll.tick({}).y,0u); // Following LF was consumed by control 0x208.
}

TEST_CASE("text flow: AB speedup can be disabled and auto confirmation enabled separately") {
    hg::TextFlowSession::Options options;
    options.ab_speedup = false;
    hg::TextFlowSession normal(font(),Units{1,2,1,0xffff},options);
    normal.tick({}); normal.tick({true,false,true,false});
    CHECK_EQ(normal.tick({false,false,true,false}).glyphs_drawn,2u);
    CHECK_EQ(normal.tick({false,false,true,false}).glyphs_drawn,2u);
    options = immediate(); options.auto_advance = true; options.auto_advance_speedup = true;
    hg::TextFlowSession auto_wait(font(),Units{0x25bc,1,0xffff},options);
    auto_wait.tick({});
    CHECK_EQ(auto_wait.tick({true}).state,0u);
    CHECK_EQ(auto_wait.frame().confirmation_sound_requests,1u);
}

TEST_CASE("text flow: original opaque background and transparent ink compose across cursor wrap") {
    auto bytes = font();
    std::fill(bytes.begin()+32,bytes.begin()+40,std::byte{0xff}); // Role 3, opaque background.
    std::fill(bytes.begin()+40,bytes.begin()+48,std::byte{0}); // Role 0, transparent.
    Units text(8192,1); text.push_back(2); text.push_back(0xffff);
    auto options = immediate(); options.width = 8; options.height = 8; options.font_line_height = 8;
    hg::TextFlowSession session(bytes,text,options);
    for (unsigned i = 0; i < 8192; ++i) session.tick({});
    CHECK_EQ(session.frame().x,0u); // Original u16 cursor wraps.
    CHECK_EQ(session.pixels()[0],1);
    session.tick({});
    CHECK_EQ(session.pixels()[0],0);
    CHECK_EQ(session.pixels()[4*8],1);
}

TEST_CASE("text flow: unsupported controls malformed text and bad dimensions reject before C") {
    for (const auto& text : {Units{0xfffe,0x201,0,0xffff}, Units{0xfffe,0x201,1,256,0xffff},
             Units{0xfffe,0x202,1,1,0xffff}, Units{0xfffe,0xff00,1,1,0xffff},
             Units{0xfffe,0x205,0,0xffff}, Units{0xf100,0xffff}, Units{1}})
        CHECK(rejects([&] { hg::TextFlowSession session(font(),text,immediate()); }));
    for (unsigned width : {0u,7u,257u}) {
        auto options = immediate(); options.width = width;
        CHECK(rejects([&] { hg::TextFlowSession session(font(),Units{1,0xffff},options); }));
    }
}
