#pragma once

#include <cstddef>
#include <cstdint>
#include <span>
#include <vector>

namespace hg {

enum class TextTokenKind {
    Glyph,
    LineFeed,
    End,
    ClearPage,
    Scroll,
    SkipOperand,
    ExtendedControl,
};

struct TextToken {
    TextTokenKind kind{};
    std::uint16_t code{};
    std::uint16_t command{}; // Only ExtendedControl has a command word.
    std::size_t unit_offset{};
    // Owned arguments, including values which resemble EOS/control markers.
    // SkipOperand has exactly one argument; other non-control tokens have none.
    std::vector<std::uint16_t> arguments;
};

struct TextCommandLimits {
    std::size_t max_units = 131072;
    std::size_t max_tokens = 131072;
    std::size_t max_arguments_per_command = 65535;
    std::size_t max_total_arguments = 131072;
};

// Tokenizes decoded HG/SS glyph IDs, without Unicode mapping, variable expansion
// or executing controls. The input is borrowed only during this call. Results
// own every argument. Requires exactly one top-level final EOS; rejects missing
// operands, trailing units and undecoded packed trainer-name markers. Unknown
// extended commands are preserved. Throws std::runtime_error on invalid input
// or a configured resource limit. Command operands are never interpreted as EOS.
std::vector<TextToken> tokenize_text_commands(std::span<const std::uint16_t> units,
    TextCommandLimits limits = {});

} // namespace hg
