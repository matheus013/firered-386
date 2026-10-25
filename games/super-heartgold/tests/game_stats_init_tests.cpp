#include "test.hpp"
#include "hg/game_stats_init.hpp"
#include "hg/pokemon_crypto.hpp"

#include <algorithm>
#include <span>

TEST_CASE("game_stats_init: raw size unencrypted words and checksum match DS layout") {
    const auto bytes = hg::initialize_game_stats(0);
    CHECK_EQ(bytes.size(),448u);
    CHECK(std::all_of(bytes.begin(),bytes.begin()+8,[](auto byte) { return byte == std::byte{0}; }));
    CHECK(bytes[444] == std::byte{0}); CHECK(bytes[445] == std::byte{0});
    CHECK(bytes[446] == std::byte{0}); CHECK(bytes[447] == std::byte{0});
    CHECK(bytes[10] == std::byte{0x7e}); CHECK(bytes[11] == std::byte{0xe9});
}

TEST_CASE("game_stats_init: counter key truncates after OR and encrypted state is zero") {
    for (const auto counter : {0u,1u,255u,256u,65535u,0x12345678u,0xffffffffu}) {
        auto bytes = hg::initialize_game_stats(counter);
        const auto key = static_cast<std::uint16_t>(counter | (counter << 8));
        CHECK_EQ(std::to_integer<unsigned>(bytes[446]),key & 255u);
        CHECK_EQ(std::to_integer<unsigned>(bytes[447]),static_cast<unsigned>(key) >> 8);
        hg::xor_pokemon_segment(std::span(bytes).subspan(8,436),static_cast<std::uint32_t>(key) << 16);
        CHECK(std::all_of(bytes.begin(),bytes.begin()+446,[](auto byte) { return byte == std::byte{0}; }));
    }
    const auto mixed = hg::initialize_game_stats(0x12345678);
    CHECK(mixed[446] == std::byte{0x78}); CHECK(mixed[447] == std::byte{0x7e});
}
