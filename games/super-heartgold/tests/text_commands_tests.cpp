#include "test.hpp"
#include "hg/text_commands.hpp"

#include <array>
#include <stdexcept>

namespace {
using Units = std::vector<std::uint16_t>;
template<class F> bool rejects(F&& fn) {
    try { fn(); return false; } catch (const std::runtime_error&) { return true; }
}
Units flatten(const std::vector<hg::TextToken>& tokens) {
    Units units;
    for (const auto& token : tokens) {
        units.push_back(token.code);
        if (token.kind == hg::TextTokenKind::ExtendedControl) {
            units.push_back(token.command);
            units.push_back(static_cast<std::uint16_t>(token.arguments.size()));
        }
        units.insert(units.end(), token.arguments.begin(), token.arguments.end());
    }
    return units;
}
}

TEST_CASE("text commands: preserves glyph IDs and classifies original flow markers") {
    const Units input{0, 0x123, 0xe000, 0x25bc, 0x25bd, 0xffff};
    const auto tokens = hg::tokenize_text_commands(input);
    const std::array kinds{hg::TextTokenKind::Glyph, hg::TextTokenKind::Glyph,
        hg::TextTokenKind::LineFeed, hg::TextTokenKind::ClearPage,
        hg::TextTokenKind::Scroll, hg::TextTokenKind::End};
    CHECK_EQ(tokens.size(), kinds.size());
    for (std::size_t i = 0; i < tokens.size(); ++i) {
        CHECK(tokens[i].kind == kinds[i]);
        CHECK_EQ(tokens[i].code, input[i]);
        CHECK_EQ(tokens[i].unit_offset, i);
        CHECK(tokens[i].arguments.empty());
    }
    CHECK(flatten(tokens) == input);
}

TEST_CASE("text commands: controls own arguments and preserve embedded markers") {
    const Units expected{0xfffe, 0xfedc, 5, 0xffff, 0xfffe, 0xe000, 0xf100, 0x25bc,
        0xf0fd, 0xffff, 0xfffe, 0x0205, 0, 0x123, 0xffff};
    auto input = expected;
    const auto tokens = hg::tokenize_text_commands(input);
    input.assign(1, 0); // Tokens must not retain references to the input vector.
    CHECK_EQ(tokens.size(), 5u);
    CHECK(tokens[0].kind == hg::TextTokenKind::ExtendedControl);
    CHECK_EQ(tokens[0].command, 0xfedc);
    CHECK_EQ(tokens[0].arguments.size(), 5u);
    CHECK_EQ(tokens[1].unit_offset, 8u);
    CHECK(tokens[1].kind == hg::TextTokenKind::SkipOperand);
    CHECK(tokens[1].arguments == Units{0xffff});
    CHECK_EQ(tokens[2].command, 0x0205);
    CHECK(tokens[2].arguments.empty());
    CHECK(flatten(tokens) == expected);
}

TEST_CASE("text commands: rejects every truncated command and missing top-level EOS") {
    const Units input{0x123, 0xfffe, 0x100, 3, 0xffff, 0xfffe, 0x25bc, 0xf0fd, 9, 0xffff};
    for (std::size_t size = 0; size < input.size(); ++size) {
        CHECK(rejects([&] { hg::tokenize_text_commands(std::span(input).first(size)); }));
    }
    CHECK(flatten(hg::tokenize_text_commands(input)) == input);
    CHECK(rejects([] { hg::tokenize_text_commands(Units{0xf0fd}); }));
    CHECK(rejects([] { hg::tokenize_text_commands(Units{0xf0fd, 0xffff}); }));
    CHECK(rejects([] { hg::tokenize_text_commands(Units{0xfffe, 7, 1, 0xffff}); }));
    CHECK(rejects([] { hg::tokenize_text_commands(Units{0xfffe, 7, 0xffff, 0xffff}); }));
}

TEST_CASE("text commands: rejects trailing payload and undecoded trainer names") {
    for (const auto& input : {Units{0xffff, 1}, Units{0xffff, 0xffff},
             Units{0xf100, 0xffff}, Units{1, 0xf100, 0xffff}})
        CHECK(rejects([&] { hg::tokenize_text_commands(input); }));
    CHECK_EQ(hg::tokenize_text_commands(Units{0xffff}).size(), 1u);
}

TEST_CASE("text commands: bounds units tokens per-command and total arguments") {
    const Units input{0xfffe, 7, 2, 1, 2, 0xf0fd, 3, 0xffff};
    const hg::TextCommandLimits exact{8, 3, 2, 3};
    CHECK(flatten(hg::tokenize_text_commands(input, exact)) == input);
    for (const auto limits : {hg::TextCommandLimits{7, 3, 2, 3},
             hg::TextCommandLimits{8, 2, 2, 3}, hg::TextCommandLimits{8, 3, 1, 3},
             hg::TextCommandLimits{8, 3, 2, 2}, hg::TextCommandLimits{8, 3, 2, 0}})
        CHECK(rejects([&] { hg::tokenize_text_commands(input, limits); }));
    CHECK(rejects([] { hg::tokenize_text_commands(Units{0xffff}, {1, 0, 0, 0}); }));
    CHECK_EQ(hg::tokenize_text_commands(Units{0xffff}, {1, 1, 0, 0}).size(), 1u);
}

TEST_CASE("text commands: preserves all command IDs and arbitrary operand words") {
    for (std::uint32_t id = 0; id <= 0xffff; ++id) {
        const Units input{0xfffe, static_cast<std::uint16_t>(id), 2,
            static_cast<std::uint16_t>(id ^ 0xffff), 0xffff, 0xffff};
        CHECK(flatten(hg::tokenize_text_commands(input)) == input);
    }
}

TEST_CASE("text commands: accepts the full unsigned 16-bit argument count without wrapping") {
    Units input{0xfffe, 0xffff, 0xffff};
    input.insert(input.end(), 65535, 0xffff);
    input.push_back(0xffff);
    const auto tokens = hg::tokenize_text_commands(input);
    CHECK_EQ(tokens.size(), 2u);
    CHECK_EQ(tokens[0].arguments.size(), 65535u);
    CHECK_EQ(tokens[1].unit_offset, 65538u);
    CHECK(flatten(tokens) == input);
    CHECK(rejects([&] {
        hg::tokenize_text_commands(input, {input.size(), 2, 65534, 65535});
    }));
}
