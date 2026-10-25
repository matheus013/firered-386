#pragma once

#include "hg/random.hpp"
#include <array>
#include <cstddef>

namespace hg {

// Isolated raw DS save block 21. This is not a complete save/new-game state.
struct InitializedRoamerBlock {
    std::array<std::byte, 0x68> bytes;
};

// Consumes exactly two consecutive 32-bit outputs from the caller's MT stream.
// Does not reseed it, use a global RNG, activate roamers, or read a clock.
[[nodiscard]] InitializedRoamerBlock initialize_roamer_block(Mt19937& random);

} // namespace hg
