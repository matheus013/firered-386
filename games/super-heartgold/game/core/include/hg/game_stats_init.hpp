#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace hg {
// Raw DS GameStats initializer only. The caller supplies a logical VBlank
// counter held constant during initialization. No wall clock or global counter.
// Does not implement statistic updates, score rules or a complete save.
using InitialGameStats = std::array<std::byte, 0x1c0>;
[[nodiscard]] InitialGameStats initialize_game_stats(std::uint32_t vblank_count);
} // namespace hg
