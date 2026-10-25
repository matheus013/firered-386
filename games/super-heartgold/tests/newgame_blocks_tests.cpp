#include "test.hpp"
#include "hg/newgame_blocks.hpp"

#include <algorithm>
#include <span>

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
