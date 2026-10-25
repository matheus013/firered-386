#pragma once
#include "hg/system_info_init.hpp"

namespace hg {
using InitialMisc = std::array<std::byte, 0x2e0>;
// Raw SoulSilver USA Misc initializer; no complete save or global binding.
[[nodiscard]] InitialMisc initialize_misc(RtcSnapshot snapshot);
} // namespace hg
