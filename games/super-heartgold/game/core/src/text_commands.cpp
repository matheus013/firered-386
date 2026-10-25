#include "hg/text_commands.hpp"

#include <stdexcept>
#include <utility>

namespace hg {
namespace {
constexpr std::uint16_t LineFeed = 0xe000;
constexpr std::uint16_t ClearPage = 0x25bc;
constexpr std::uint16_t Scroll = 0x25bd;
constexpr std::uint16_t SkipOperand = 0xf0fd;
constexpr std::uint16_t PackedTrainerName = 0xf100;
constexpr std::uint16_t ExtendedControl = 0xfffe;
constexpr std::uint16_t End = 0xffff;

TextTokenKind classify(std::uint16_t code) {
    switch (code) {
    case LineFeed: return TextTokenKind::LineFeed;
    case ClearPage: return TextTokenKind::ClearPage;
    case Scroll: return TextTokenKind::Scroll;
    case SkipOperand: return TextTokenKind::SkipOperand;
    case ExtendedControl: return TextTokenKind::ExtendedControl;
    case End: return TextTokenKind::End;
    default: return TextTokenKind::Glyph;
    }
}
} // namespace

std::vector<TextToken> tokenize_text_commands(std::span<const std::uint16_t> units,
    TextCommandLimits limits) {
    if (units.size() > limits.max_units)
        throw std::runtime_error("Message exceeds text command unit limit");
    std::vector<TextToken> result;
    std::size_t cursor = 0;
    std::size_t total_arguments = 0;
    while (cursor < units.size()) {
        if (result.size() >= limits.max_tokens)
            throw std::runtime_error("Message exceeds text token limit");
        const auto offset = cursor;
        const auto code = units[cursor++];
        if (code == PackedTrainerName)
            throw std::runtime_error("Packed trainer name must be decoded before tokenization");
        TextToken token{classify(code), code, 0, offset, {}};
        std::size_t argument_count = 0;
        if (code == ExtendedControl) {
            if (units.size() - cursor < 2)
                throw std::runtime_error("Truncated extended text command header");
            token.command = units[cursor++];
            argument_count = units[cursor++];
        } else if (code == SkipOperand) {
            argument_count = 1;
        }
        if (argument_count > limits.max_arguments_per_command ||
            argument_count > limits.max_total_arguments - total_arguments)
            throw std::runtime_error("Message exceeds text command argument limit");
        if (argument_count > units.size() - cursor)
            throw std::runtime_error("Truncated text command arguments");
        token.arguments.assign(units.begin() + cursor, units.begin() + cursor + argument_count);
        cursor += argument_count;
        total_arguments += argument_count;
        result.push_back(std::move(token));
        if (code == End) {
            if (cursor != units.size())
                throw std::runtime_error("Trailing units after text terminator");
            return result;
        }
    }
    throw std::runtime_error("Message has no top-level text terminator");
}
} // namespace hg
