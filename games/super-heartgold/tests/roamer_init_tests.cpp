#include "test.hpp"
#include "hg/roamer_init.hpp"

#include <algorithm>
#include <cstdint>

namespace {
std::uint32_t word(const hg::InitializedRoamerBlock& block, std::size_t offset) {
    std::uint32_t result = 0;
    for (unsigned byte = 0; byte < 4; ++byte)
        result |= std::to_integer<std::uint32_t>(block.bytes[offset + byte]) << (byte * 8);
    return result;
}
}

TEST_CASE("roamer_init: known MT outputs populate two little endian words only") {
    hg::Mt19937 random(5489);
    const auto block = hg::initialize_roamer_block(random);
    CHECK_EQ(block.bytes.size(), 104u);
    CHECK_EQ(word(block, 0), 3499211612u);
    CHECK_EQ(word(block, 4), 581869302u);
    CHECK_EQ(random.next32(), 3890346734u);
    CHECK(std::all_of(block.bytes.begin() + 8, block.bytes.end(),
                      [](std::byte value) { return value == std::byte{0}; }));
}

TEST_CASE("roamer_init: caller stream position survives twist boundaries") {
    for (unsigned advance : {0u, 1u, 622u, 623u, 624u, 1247u}) {
        hg::Mt19937 random(0x12345678u), expected(0x12345678u);
        for (unsigned index = 0; index < advance; ++index) {
            random.next32();
            expected.next32();
        }
        const auto block = hg::initialize_roamer_block(random);
        CHECK_EQ(word(block, 0), expected.next32());
        CHECK_EQ(word(block, 4), expected.next32());
        for (unsigned index = 0; index < 4; ++index) CHECK_EQ(random.next32(), expected.next32());
    }
}

TEST_CASE("roamer_init: independent streams and consecutive initializations retain ownership") {
    hg::Mt19937 first(1), second(0xffffffffu), expected(1);
    auto original = hg::initialize_roamer_block(first);
    CHECK_EQ(word(original, 0), expected.next32());
    CHECK_EQ(word(original, 4), expected.next32());
    const auto other = hg::initialize_roamer_block(second);
    CHECK(word(other, 0) != word(original, 0));
    const auto next = hg::initialize_roamer_block(first);
    CHECK_EQ(word(next, 0), expected.next32());
    CHECK_EQ(word(next, 4), expected.next32());
    original.bytes.fill(std::byte{0xff});
    CHECK(next.bytes[103] == std::byte{0});
}
