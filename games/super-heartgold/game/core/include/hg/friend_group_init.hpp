#pragma once

#include <array>
#include <cstddef>

namespace hg {

using FriendGroupBlock = std::array<std::byte, 0x108>;

// Produces the pre-cleared SoulSilver USA FriendGroup block. The original also
// binds a process-global pointer to this block; the host integration must keep
// that ownership/binding explicit rather than relying on a DS global.
[[nodiscard]] FriendGroupBlock initialize_friend_group_block();

} // namespace hg
