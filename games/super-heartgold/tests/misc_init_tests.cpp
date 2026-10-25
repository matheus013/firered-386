#include "test.hpp"
#include "hg/misc_init.hpp"
#include <algorithm>
#include <stdexcept>

TEST_CASE("misc_init: greeting sentinels and rival name preserve original byte widths") {
    const auto bytes = hg::initialize_misc({26,10,25,0,12,34,56});
    CHECK_EQ(bytes.size(), 0x2e0u);
    CHECK(bytes[0x230] == std::byte{26});
    CHECK(bytes[0x240] == std::byte{12});
    CHECK(bytes[0x248] == std::byte{56});
    CHECK(std::all_of(bytes.begin(), bytes.begin()+0x230, [](auto b){ return b==std::byte{0}; }));
    CHECK(std::all_of(bytes.begin()+0x270, bytes.begin()+0x278, [](auto b){ return b==std::byte{255}; }));
    CHECK(std::all_of(bytes.begin()+0x278, bytes.begin()+0x280, [](auto b){ return b==std::byte{0}; }));
    CHECK(bytes[0x2a0] == std::byte{4});
    CHECK(bytes[0x2a4] == std::byte{1});
    CHECK(bytes[0x2a5] == std::byte{5});
    CHECK(std::all_of(bytes.begin()+0x2a6, bytes.begin()+0x2d0, [](auto b){ return b==std::byte{255}; }));
    CHECK(std::all_of(bytes.begin()+0x2d0, bytes.end(), [](auto b){ return b==std::byte{0}; }));
}
TEST_CASE("misc_init: invalid RTC is rejected before generating save bytes") {
    bool rejected = false;
    try { (void)hg::initialize_misc({1,2,29,0,0,0,0}); }
    catch (const std::runtime_error&) { rejected = true; }
    CHECK(rejected);
    CHECK(hg::initialize_misc({0,2,29,2,0,0,0})[0x238] == std::byte{29});
}
