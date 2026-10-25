#pragma once

#include <array>
#include <cstddef>
#include <span>

namespace hg {

struct PreclearedPcStorageBlock {
    std::array<std::byte, 0x122fc> bytes;
};

// Explicit pre-clear + PCStorage initialization using encrypted message bank 24
// (the NARC member, not the outer archive). Reads names 6..23, at most 20 u16
// units each including EOS. No filesystem, heap services, RNG, globals or save IO.
// This is an isolated block, not a complete new game. Throws on malformed names.
[[nodiscard]] PreclearedPcStorageBlock initialize_precleared_pc_storage(
    std::span<const std::byte> encrypted_name_bank);

} // namespace hg
