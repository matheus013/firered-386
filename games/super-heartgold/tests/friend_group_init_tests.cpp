#include "test.hpp"
#include "hg/friend_group_init.hpp"

#include <algorithm>

TEST_CASE("friend_group_init: six empty groups have empty names and zero metadata") {
    const auto block = hg::initialize_friend_group_block();
    CHECK_EQ(block.size(), 0x108u);
    for (std::size_t group = 0; group < 6; ++group) {
        const auto start = group * 44;
        CHECK(block[start] == std::byte{0xff});
        CHECK(block[start + 1] == std::byte{0xff});
        CHECK(block[start + 16] == std::byte{0xff});
        CHECK(block[start + 17] == std::byte{0xff});
        CHECK(std::all_of(block.begin() + start + 2, block.begin() + start + 16,
                          [](std::byte byte) { return byte == std::byte{0}; }));
        CHECK(std::all_of(block.begin() + start + 18, block.begin() + start + 44,
                          [](std::byte byte) { return byte == std::byte{0}; }));
    }
}
