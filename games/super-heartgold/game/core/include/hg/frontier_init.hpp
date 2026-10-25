#pragma once
#include <array>
#include <cstddef>
namespace hg {
using FrontierBlock = std::array<std::byte, 0x1628>;
// The owner must explicitly keep/commit its working copy; no process-global state.
[[nodiscard]] FrontierBlock initialize_frontier_block();
}
