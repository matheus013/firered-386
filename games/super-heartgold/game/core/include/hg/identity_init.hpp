#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace hg {

// Explicit byte-valued metadata; accepting bytes does not validate an edition
// or promise localization support. No host identity or implicit defaults.
struct GameIdentity {
    std::uint8_t language;
    std::uint8_t version;
};

struct PreclearedMailboxBlock { std::array<std::byte, 0x460> bytes; };
struct PreclearedSafariBlock { std::array<std::byte, 0x5fc> bytes; };

// Each function creates a zeroed raw DS block, then applies that initializer's
// effects. This explicitly includes the caller's pre-clear step: standalone
// original initializers leave some fields unchanged. Neither is a complete save.
[[nodiscard]] PreclearedMailboxBlock initialize_precleared_mailbox(GameIdentity identity);
[[nodiscard]] PreclearedSafariBlock initialize_precleared_safari(GameIdentity identity);

} // namespace hg
