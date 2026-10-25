#include "test.hpp"
#include "hg/newgame_blocks.hpp"

#include <algorithm>
#include <span>
#include <cstdint>

TEST_CASE("newgame_blocks: selected defaults match original DS block bytes") {
    const auto blocks = hg::initialize_soulsilver_usa_partial_blocks();
    CHECK_EQ(blocks.player_data.size(), 44u);
    CHECK_EQ(blocks.vars_flags.size(), 1100u);
    CHECK_EQ(blocks.local_field_data.size(), 128u);
    CHECK(blocks.player_data[0] == std::byte{1});
    CHECK(blocks.player_data[29] == std::byte{2});
    CHECK(blocks.player_data[32] == std::byte{8});
    CHECK(blocks.local_field_data[104] == std::byte{1});
    CHECK(blocks.local_field_data[105] == std::byte{0});
    CHECK(std::all_of(blocks.vars_flags.begin(), blocks.vars_flags.end(),
                      [](std::byte value) { return value == std::byte{0}; }));
}

TEST_CASE("newgame_blocks: no position money trainer or progress is invented") {
    auto blocks = hg::initialize_soulsilver_usa_partial_blocks();
    blocks.player_data[0] = std::byte{0};
    blocks.player_data[29] = std::byte{0};
    blocks.player_data[32] = std::byte{0};
    blocks.local_field_data[104] = std::byte{0};
    CHECK(std::all_of(blocks.player_data.begin(), blocks.player_data.end(),
                      [](std::byte value) { return value == std::byte{0}; }));
    CHECK(std::all_of(blocks.local_field_data.begin(), blocks.local_field_data.end(),
                      [](std::byte value) { return value == std::byte{0}; }));
}

TEST_CASE("newgame_blocks: snapshots are independent and initialized each time") {
    auto first = hg::initialize_soulsilver_usa_partial_blocks();
    first.player_data.fill(std::byte{0xff});
    first.vars_flags.fill(std::byte{0xff});
    first.local_field_data.fill(std::byte{0xff});
    const auto second = hg::initialize_soulsilver_usa_partial_blocks();
    CHECK(second.player_data[0] == std::byte{1});
    CHECK(second.vars_flags[0] == std::byte{0});
    CHECK(second.local_field_data[0] == std::byte{0});
    CHECK(second.local_field_data[104] == std::byte{1});
}

TEST_CASE("newgame_blocks: Pal Pad has sixteen empty EOS names and cleared records") {
    auto blocks = hg::initialize_soulsilver_usa_partial_blocks();
    CHECK_EQ(blocks.pal_pad.size(), 2176u);
    for (std::size_t entry = 0; entry < 16; ++entry) {
        const auto start = entry * 136;
        CHECK(blocks.pal_pad[start] == std::byte{0xff});
        CHECK(blocks.pal_pad[start + 1] == std::byte{0xff});
        blocks.pal_pad[start] = blocks.pal_pad[start + 1] = std::byte{0};
    }
    CHECK(std::all_of(blocks.pal_pad.begin(), blocks.pal_pad.end(),
                      [](std::byte value) { return value == std::byte{0}; }));
}

TEST_CASE("newgame_blocks: deterministic empty blocks include no uninitialized payload") {
    const auto blocks = hg::initialize_soulsilver_usa_partial_blocks();
    for (const auto bytes : {std::span<const std::byte>(blocks.bag),
                             std::span<const std::byte>(blocks.map_objects),
                             std::span<const std::byte>(blocks.link_battle_ruleset),
                             std::span<const std::byte>(blocks.seal_case),
                             std::span<const std::byte>(blocks.chatot),
                             std::span<const std::byte>(blocks.special_ribbons),
                             std::span<const std::byte>(blocks.gts),
                             std::span<const std::byte>(blocks.pokeathlon_friendship_records),
                             std::span<const std::byte>(blocks.follow_mon)}) {
        CHECK(std::all_of(bytes.begin(), bytes.end(),
                          [](std::byte value) { return value == std::byte{0}; }));
    }
    CHECK_EQ(blocks.special_ribbons.size(), 14u); // raw size, no save alignment
    CHECK_EQ(blocks.map_objects.size(), 5120u);
}

TEST_CASE("newgame_blocks: Pokedex retains magic empty form history and locked progression") {
    const auto blocks = hg::initialize_soulsilver_usa_partial_blocks();
    CHECK_EQ(blocks.pokedex.size(), 832u);
    CHECK(blocks.pokedex[0] == std::byte{0xfe});
    CHECK(blocks.pokedex[1] == std::byte{0xca});
    CHECK(blocks.pokedex[2] == std::byte{0xef});
    CHECK(blocks.pokedex[3] == std::byte{0xbe});
    CHECK_EQ(std::count(blocks.pokedex.begin(), blocks.pokedex.end(), std::byte{0xff}), 69);
    // The high bytes in the final caught/seen words are Deoxys history, not
    // caught Pokémon. Dex-enabled and National Dex progression remain clear.
    CHECK(blocks.pokedex[67] == std::byte{0xff});
    CHECK(blocks.pokedex[131] == std::byte{0xff});
    CHECK(blocks.pokedex[822] == std::byte{0});
    CHECK(blocks.pokedex[823] == std::byte{0});
    CHECK(blocks.pokedex[831] == std::byte{0}); // trailing padding
}

TEST_CASE("newgame_blocks: unused photo records names and event scores use EOS sentinels") {
    const auto blocks = hg::initialize_soulsilver_usa_partial_blocks();
    CHECK_EQ(std::count(blocks.photo_album.begin(), blocks.photo_album.end(), std::byte{0xff}), 1440);
    for (std::size_t photo = 0; photo < 36; ++photo)
        CHECK(blocks.photo_album[4 + photo * 132 + 7] == std::byte{0}); // numMons
    CHECK_EQ(std::count(blocks.pokeathlon.begin(), blocks.pokeathlon.end(), std::byte{0xff}), 200);
    CHECK_EQ(std::count(blocks.apricorn_box.begin(), blocks.apricorn_box.end(), std::byte{0xff}), 48);
    CHECK_EQ(std::count(blocks.trainer_house.begin(), blocks.trainer_house.end(), std::byte{0xff}), 1360);
    for (std::size_t set = 0; set < 10; ++set)
        CHECK(blocks.trainer_house[set * 384 + 48] == std::byte{0}); // no party species
}

TEST_CASE("newgame_blocks: badges survey and walker retain original startup defaults") {
    const auto blocks = hg::initialize_soulsilver_usa_partial_blocks();
    for (std::size_t badge = 0; badge < 8; ++badge) {
        CHECK(blocks.trainer_card[badge * 4] == std::byte{0x8c});
        CHECK(blocks.trainer_card[badge * 4 + 1] == std::byte{0});
    }
    CHECK(std::all_of(blocks.trainer_card.begin() + 32, blocks.trainer_card.end(),
                      [](std::byte value) { return value == std::byte{0}; }));
    CHECK(std::all_of(blocks.plaza_survey.begin(), blocks.plaza_survey.begin() + 4,
                      [](std::byte value) { return value == std::byte{0xff}; }));
    CHECK(std::all_of(blocks.plaza_survey.begin() + 4, blocks.plaza_survey.end(),
                      [](std::byte value) { return value == std::byte{0}; }));
    CHECK(blocks.pokewalker[304] == std::byte{3});
    CHECK_EQ(std::count_if(blocks.pokewalker.begin(), blocks.pokewalker.end(),
                          [](std::byte value) { return value != std::byte{0}; }), 1);
}

namespace {
bool decrypts_to_zero(std::span<const std::byte> bytes) {
    // Independent wide-integer recurrence and explicit modulo instead of the
    // production unsigned wrap. Used only to inspect newly initialized data.
    std::uint64_t seed = 0;
    for (std::size_t offset = 0; offset < bytes.size(); offset += 2) {
        seed = (seed * 1103515245ull + 24691ull) % 0x100000000ull;
        const auto ciphertext = std::to_integer<unsigned>(bytes[offset]) |
            (std::to_integer<unsigned>(bytes[offset + 1]) << 8);
        if ((ciphertext ^ (seed >> 16)) != 0) return false;
    }
    return true;
}
}

TEST_CASE("newgame_blocks: empty party has capacity six and encrypted zero records") {
    const auto blocks = hg::initialize_soulsilver_usa_partial_blocks();
    CHECK_EQ(blocks.party.size(), 1456u);
    CHECK(blocks.party[0] == std::byte{6});
    CHECK(std::all_of(blocks.party.begin() + 1, blocks.party.begin() + 8,
                      [](std::byte value) { return value == std::byte{0}; }));
    for (std::size_t slot = 0; slot < 6; ++slot) {
        const auto mon = std::span(blocks.party).subspan(8 + slot * 236, 236);
        CHECK(std::all_of(mon.begin(), mon.begin() + 8,
                          [](std::byte value) { return value == std::byte{0}; }));
        CHECK(decrypts_to_zero(mon.subspan(8, 128)));
        CHECK(decrypts_to_zero(mon.subspan(136, 100)));
        // The box and party segments restart the cipher independently.
        CHECK(std::equal(mon.begin() + 8, mon.begin() + 108, mon.begin() + 136));
    }
    CHECK(std::all_of(blocks.party.begin() + 1424, blocks.party.end(),
                      [](std::byte value) { return value == std::byte{0}; }));
    // Prefix independently observed in the original ARM9 initializer proof.
    constexpr std::array<unsigned, 8> prefix{0, 0, 0x7e, 0xe9, 0x71, 0x52, 0xb0, 0x31};
    for (std::size_t index = 0; index < prefix.size(); ++index)
        CHECK_EQ(std::to_integer<unsigned>(blocks.party[16 + index]), prefix[index]);
}

TEST_CASE("newgame_blocks: daycare clears eggs mail steps and encrypts only box payloads") {
    const auto blocks = hg::initialize_soulsilver_usa_partial_blocks();
    CHECK_EQ(blocks.daycare.size(), 480u);
    for (std::size_t slot = 0; slot < 2; ++slot) {
        const auto mon = std::span(blocks.daycare).subspan(slot * 236, 236);
        CHECK(decrypts_to_zero(mon.subspan(8, 128)));
        CHECK(std::all_of(mon.begin(), mon.begin() + 8,
                          [](std::byte value) { return value == std::byte{0}; }));
        CHECK(std::all_of(mon.begin() + 136, mon.end(),
                          [](std::byte value) { return value == std::byte{0}; }));
    }
    CHECK(std::all_of(blocks.daycare.begin() + 472, blocks.daycare.end(),
                      [](std::byte value) { return value == std::byte{0}; }));
}

TEST_CASE("newgame_blocks: migrated records are empty without invented identity or RTC") {
    const auto blocks = hg::initialize_soulsilver_usa_partial_blocks();
    CHECK_EQ(blocks.migrated_pokemon.size(), 1672u);
    for (std::size_t slot = 0; slot < 6; ++slot) {
        const auto mon = std::span(blocks.migrated_pokemon).subspan(slot * 236, 236);
        CHECK(std::equal(mon.begin(), mon.end(), blocks.party.begin() + 8 + slot * 236));
    }
    CHECK(std::all_of(blocks.migrated_pokemon.begin() + 1416, blocks.migrated_pokemon.end(),
                      [](std::byte value) { return value == std::byte{0}; }));
}

TEST_CASE("newgame_blocks: fashion entries are empty and no wallpapers are unlocked") {
    auto blocks = hg::initialize_soulsilver_usa_partial_blocks();
    CHECK_EQ(blocks.fashion_data.size(), 2100u);
    CHECK_EQ(std::count(blocks.fashion_data.begin(), blocks.fashion_data.end(), std::byte{0x34}), 16);
    CHECK_EQ(std::count(blocks.fashion_data.begin(), blocks.fashion_data.end(), std::byte{0x12}), 34);
    CHECK(std::all_of(blocks.fashion_data.begin() + 0x7f4, blocks.fashion_data.begin() + 0x81c,
                      [](std::byte value) { return value == std::byte{0}; })); // no accessories
    CHECK(std::all_of(blocks.fashion_data.begin() + 0x81c, blocks.fashion_data.begin() + 0x82e,
                      [](std::byte value) { return value == std::byte{18}; }));
    CHECK(std::all_of(blocks.fashion_data.begin() + 0x82e, blocks.fashion_data.end(),
                      [](std::byte value) { return value == std::byte{0}; })); // case padding
}

TEST_CASE("newgame_blocks: Pokegear has Mother only and no progression or gifts") {
    const auto blocks = hg::initialize_soulsilver_usa_partial_blocks();
    CHECK_EQ(blocks.pokegear.size(), 1624u);
    CHECK(blocks.pokegear[0] == std::byte{3});
    CHECK(blocks.pokegear[2] == std::byte{128});
    CHECK(blocks.pokegear[3] == std::byte{128});
    CHECK(std::all_of(blocks.pokegear.begin() + 4, blocks.pokegear.begin() + 0x4b8,
                      [](std::byte value) { return value == std::byte{0}; }));
    for (std::size_t contact = 0; contact < 75; ++contact) {
        CHECK(blocks.pokegear[0x4b8 + contact * 4] == std::byte{4});
        CHECK(blocks.pokegear[0x4ba + contact * 4] == std::byte{0}); // giftItem low
    }
    CHECK(blocks.pokegear[0x5fe] == std::byte{7});
    CHECK(blocks.pokegear[0x60c] == std::byte{0});
    CHECK_EQ(std::count(blocks.pokegear.begin(), blocks.pokegear.end(), std::byte{0xff}), 74);
    CHECK(blocks.pokegear[0x657] == std::byte{0}); // trailing struct padding
}
