#include "test.hpp"
#include "hg/identity_init.hpp"
#include <algorithm>

TEST_CASE("identity_init: mailbox contains empty slots with explicit identity") {
    const auto block = hg::initialize_precleared_mailbox({2, 8});
    CHECK_EQ(block.bytes.size(), 1120u);
    for (std::size_t slot = 0; slot < 20; ++slot) {
        const auto start = slot * 56;
        CHECK(block.bytes[start + 4] == std::byte{0});
        CHECK(block.bytes[start + 5] == std::byte{2});
        CHECK(block.bytes[start + 6] == std::byte{8});
        CHECK(block.bytes[start + 7] == std::byte{0xff});
        CHECK(std::all_of(block.bytes.begin() + start + 8, block.bytes.begin() + start + 30,
                          [](std::byte value) { return value == std::byte{0xff}; }));
        for (std::size_t message = 0; message < 3; ++message) {
            const auto at = start + 32 + message * 8;
            CHECK(block.bytes[at] == std::byte{0xff});
            CHECK(block.bytes[at + 2] == std::byte{0});
            CHECK(block.bytes[at + 3] == std::byte{0});
            CHECK(block.bytes[at + 4] == std::byte{0xff});
        }
    }
}

TEST_CASE("identity_init: Safari is unlinked with no areas or progress invented") {
    const auto block = hg::initialize_precleared_safari({2, 7});
    CHECK_EQ(block.bytes.size(), 1532u);
    CHECK(std::all_of(block.bytes.begin(), block.bytes.begin() + 0x5e2,
                      [](std::byte value) { return value == std::byte{0}; }));
    CHECK(block.bytes[0x5e2] == std::byte{2});
    CHECK(block.bytes[0x5e3] == std::byte{7});
    CHECK(std::all_of(block.bytes.begin() + 0x5e8, block.bytes.begin() + 0x5f8,
                      [](std::byte value) { return value == std::byte{0xff}; }));
    CHECK(std::all_of(block.bytes.begin() + 0x5f8, block.bytes.end(),
                      [](std::byte value) { return value == std::byte{0}; }));
}

TEST_CASE("identity_init: independent identities do not leak through globals or snapshots") {
    auto first = hg::initialize_precleared_mailbox({1, 7});
    const auto second = hg::initialize_precleared_mailbox({255, 255});
    const auto safari = hg::initialize_precleared_safari({0, 0});
    CHECK(first.bytes[5] == std::byte{1});
    CHECK(second.bytes[5] == std::byte{255});
    CHECK(second.bytes[6] == std::byte{255});
    CHECK(safari.bytes[0x5e2] == std::byte{0});
    first.bytes.fill(std::byte{0});
    CHECK(second.bytes[7] == std::byte{0xff});
    CHECK(safari.bytes[0x5f7] == std::byte{0xff});
}
