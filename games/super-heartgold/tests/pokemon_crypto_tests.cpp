#include "test.hpp"
#include "hg/pokemon_crypto.hpp"

#include <algorithm>
#include <array>
#include <stdexcept>
#include <vector>

namespace {
using Bytes = std::vector<std::byte>;
template<class F> bool rejects(F&& action) {
    try { action(); return false; } catch (const std::runtime_error&) { return true; }
}
}

TEST_CASE("pokemon crypto: known seed vectors advance before each little-endian word") {
    struct Example { std::uint32_t seed; std::array<unsigned, 8> bytes; };
    const std::array examples{
        Example{0, {0x00, 0x00, 0x7e, 0xe9, 0x71, 0x52, 0xb0, 0x31}},
        Example{1, {0xc6, 0x41, 0x21, 0xac, 0xee, 0xd2, 0xb7, 0x1f}},
        Example{0xffffffff, {0x3a, 0xbe, 0xdb, 0x26, 0xf3, 0xd1, 0xaa, 0x43}},
        Example{0x12345678, {0x71, 0x0b, 0xea, 0x84, 0x8a, 0xd9, 0xe0, 0xf4}},
    };
    for (const auto& example : examples) {
        Bytes zero(8);
        hg::xor_pokemon_segment(zero, example.seed);
        for (std::size_t at = 0; at < zero.size(); ++at)
            CHECK_EQ(std::to_integer<unsigned>(zero[at]), example.bytes[at]);
        hg::xor_pokemon_segment(zero, example.seed);
        CHECK(std::all_of(zero.begin(), zero.end(), [](auto byte) { return byte == std::byte{0}; }));
    }
}

TEST_CASE("pokemon crypto: nonzero data uses XOR and calls restart independently") {
    const Bytes initial{std::byte{0x12}, std::byte{0x34}, std::byte{0x56}, std::byte{0x78}};
    auto first = initial;
    auto second = initial;
    hg::xor_pokemon_segment(first, 1);
    hg::xor_pokemon_segment(second, 0xffffffff);
    CHECK_EQ(std::to_integer<unsigned>(first[0]), 0x12u ^ 0xc6u);
    CHECK_EQ(std::to_integer<unsigned>(first[1]), 0x34u ^ 0x41u);
    CHECK_EQ(std::to_integer<unsigned>(first[2]), 0x56u ^ 0x21u);
    CHECK_EQ(std::to_integer<unsigned>(first[3]), 0x78u ^ 0xacu);
    auto independent = initial;
    hg::xor_pokemon_segment(independent, 1);
    CHECK(independent == first);
    CHECK(second != first);
    hg::xor_pokemon_segment(first, 1);
    CHECK(first == initial);
}

TEST_CASE("pokemon crypto: checksum sums little-endian words with u16 wrap") {
    CHECK_EQ(hg::pokemon_checksum(Bytes{}), 0u);
    CHECK_EQ(hg::pokemon_checksum(Bytes{std::byte{0x12}, std::byte{0x34}}), 0x3412u);
    CHECK_EQ(hg::pokemon_checksum(Bytes{std::byte{0xff}, std::byte{0xff}, std::byte{2}, std::byte{0}}), 1u);
    CHECK_EQ(hg::pokemon_checksum(Bytes{std::byte{0}, std::byte{1}, std::byte{1}, std::byte{0}}), 0x101u);
}

TEST_CASE("pokemon crypto: unaligned subspans preserve sentinel bytes") {
    Bytes bytes{std::byte{0xaa}, std::byte{0x12}, std::byte{0x34}, std::byte{0x56},
        std::byte{0x78}, std::byte{0xbb}};
    const auto initial = bytes;
    auto segment = std::span(bytes).subspan(1, 4);
    CHECK_EQ(hg::pokemon_checksum(segment), 0xac68u);
    hg::xor_pokemon_segment(segment, 0x12345678);
    CHECK_EQ(std::to_integer<unsigned>(bytes.front()), 0xaau);
    CHECK_EQ(std::to_integer<unsigned>(bytes.back()), 0xbbu);
    hg::xor_pokemon_segment(segment, 0x12345678);
    CHECK(bytes == initial);
}

TEST_CASE("pokemon crypto: rejects odd or oversized input before mutation") {
    for (const auto size : {1u, 3u, 127u, 131071u, 131073u, 131074u}) {
        Bytes bytes(size, std::byte{0xa5});
        const auto initial = bytes;
        CHECK(rejects([&] { hg::xor_pokemon_segment(bytes, 123); }));
        CHECK(bytes == initial);
        CHECK(rejects([&] { hg::pokemon_checksum(bytes); }));
    }
}

TEST_CASE("pokemon crypto: empty and maximum-sized segments are supported") {
    Bytes empty;
    hg::xor_pokemon_segment(empty, 0xffffffff);
    CHECK(empty.empty());
    Bytes maximum(hg::PokemonCryptoMaxBytes, std::byte{0xff});
    CHECK_EQ(hg::pokemon_checksum(maximum), 0u);
    hg::xor_pokemon_segment(maximum, 0xdeadbeef);
    CHECK(maximum != Bytes(hg::PokemonCryptoMaxBytes, std::byte{0xff}));
    hg::xor_pokemon_segment(maximum, 0xdeadbeef);
    CHECK(maximum == Bytes(hg::PokemonCryptoMaxBytes, std::byte{0xff}));
}
