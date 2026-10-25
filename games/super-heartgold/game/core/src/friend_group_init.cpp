#include "hg/friend_group_init.hpp"

namespace hg {

FriendGroupBlock initialize_friend_group_block() {
    FriendGroupBlock block{};
    for (std::size_t group = 0; group < 6; ++group) {
        block[group * 44] = std::byte{0xff};
        block[group * 44 + 1] = std::byte{0xff};
        block[group * 44 + 16] = std::byte{0xff};
        block[group * 44 + 17] = std::byte{0xff};
    }
    return block;
}

} // namespace hg
