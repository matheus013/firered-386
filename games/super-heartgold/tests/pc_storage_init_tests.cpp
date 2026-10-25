#include "test.hpp"
#include "hg/pc_storage_init.hpp"
#include <algorithm>
#include <cstdint>
#include <stdexcept>
#include <vector>

namespace {
using Bytes = std::vector<std::byte>;
void put16(Bytes& bytes, std::size_t at, std::uint16_t value) {
    bytes[at] = static_cast<std::byte>(value & 255);
    bytes[at + 1] = static_cast<std::byte>(value >> 8);
}
void put32(Bytes& bytes, std::size_t at, std::uint32_t value) {
    put16(bytes, at, static_cast<std::uint16_t>(value));
    put16(bytes, at + 2, static_cast<std::uint16_t>(value >> 16));
}
Bytes fixture(unsigned name_length = 3, bool packed = false) {
    const std::size_t header = 4 + 24 * 8;
    Bytes bytes(header + (6 + 18 * name_length) * 2);
    put16(bytes, 0, 24); // Key zero keeps allocation entries independently plain.
    std::size_t payload = header;
    for (unsigned message = 0; message < 24; ++message) {
        const unsigned length = message < 6 ? 1 : name_length;
        put32(bytes, 4 + message * 8, static_cast<std::uint32_t>(payload));
        put32(bytes, 8 + message * 8, length);
        for (unsigned unit = 0; unit < length; ++unit) {
            const auto code = unit + 1 == length ? 0xffffu :
                packed && unit == 0 ? 0xf100u : message * 16 + unit;
            put16(bytes, payload, static_cast<std::uint16_t>(code ^
                (((message + 1) * 596947u + unit * 18749u) & 0xffff)));
            payload += 2;
        }
    }
    return bytes;
}
template<class F> bool rejects(F&& function) {
    try { function(); return false; } catch (const std::runtime_error&) { return true; }
}
}

TEST_CASE("pc_storage_init: 540 empty encrypted records retain DS headers and box padding") {
    const auto block = hg::initialize_precleared_pc_storage(fixture());
    CHECK_EQ(block.bytes.size(), 0x122fcu);
    for (std::size_t box = 0; box < 18; ++box) {
        for (std::size_t mon = 0; mon < 30; ++mon) {
            const auto start = box * 0x1000 + mon * 136;
            CHECK(std::all_of(block.bytes.begin() + start, block.bytes.begin() + start + 10,
                              [](std::byte value) { return value == std::byte{0}; }));
            CHECK(block.bytes[start + 10] == std::byte{0x7e});
            CHECK(block.bytes[start + 11] == std::byte{0xe9});
            CHECK(std::equal(block.bytes.begin() + start, block.bytes.begin() + start + 136,
                             block.bytes.begin()));
        }
        CHECK(std::all_of(block.bytes.begin() + box * 0x1000 + 0xff0,
                          block.bytes.begin() + (box + 1) * 0x1000,
                          [](std::byte value) { return value == std::byte{0}; }));
    }
}

TEST_CASE("pc_storage_init: names use exact rows and defaults do not invent progression") {
    const auto block = hg::initialize_precleared_pc_storage(fixture());
    for (std::size_t box = 0; box < 18; ++box) {
        const auto at = 0x12008 + box * 40;
        const auto first = std::to_integer<unsigned>(block.bytes[at]) |
            (std::to_integer<unsigned>(block.bytes[at + 1]) << 8);
        CHECK_EQ(first, (box + 6) * 16);
        CHECK(block.bytes[at + 4] == std::byte{0xff});
        CHECK(block.bytes[at + 5] == std::byte{0xff});
        CHECK(std::all_of(block.bytes.begin() + at + 6, block.bytes.begin() + at + 40,
                          [](std::byte value) { return value == std::byte{0}; }));
        CHECK_EQ(std::to_integer<unsigned>(block.bytes[0x122d8 + box]), box % 16);
    }
    CHECK(std::all_of(block.bytes.begin() + 0x12000, block.bytes.begin() + 0x12008,
                      [](std::byte value) { return value == std::byte{0}; }));
    CHECK(std::all_of(block.bytes.begin() + 0x122ea, block.bytes.end(),
                      [](std::byte value) { return value == std::byte{0}; }));
}

TEST_CASE("pc_storage_init: full length names fit and malformed input is rejected") {
    const auto block = hg::initialize_precleared_pc_storage(fixture(20));
    CHECK(block.bytes[0x12008 + 17 * 40 + 38] == std::byte{0xff});
    CHECK(rejects([] { (void)hg::initialize_precleared_pc_storage(fixture(21)); }));
    CHECK(rejects([] { (void)hg::initialize_precleared_pc_storage(fixture(3, true)); }));
    CHECK(rejects([] { (void)hg::initialize_precleared_pc_storage({}); }));
    auto truncated = fixture(); truncated.pop_back();
    CHECK(rejects([&] { (void)hg::initialize_precleared_pc_storage(truncated); }));
    auto missing = fixture(); put16(missing, 0, 23);
    CHECK(rejects([&] { (void)hg::initialize_precleared_pc_storage(missing); }));
}
