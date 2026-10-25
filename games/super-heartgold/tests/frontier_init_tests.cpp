#include "test.hpp"
#include "hg/frontier_init.hpp"
#include <algorithm>
TEST_CASE("frontier_init: independent working copies preserve reset state") {
    auto working = hg::initialize_frontier_block();
    const auto saved = working;
    CHECK_EQ(saved.size(), 0x1628u);
    CHECK(saved[0x957] == std::byte{1});
    CHECK(saved[0xac0] == std::byte{0x59});
    CHECK(saved[0xad6] == std::byte{4});
    CHECK(std::all_of(saved.begin()+0xadc,saved.end(),[](auto b){return b==std::byte{0};}));
    working[0] = std::byte{42};
    CHECK(saved[0] == std::byte{0});
    CHECK(hg::initialize_frontier_block() == saved);
}
