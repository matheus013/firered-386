#pragma once

#include <array>
#include <cstddef>

namespace hg {

// Twenty-six raw DS-layout block initializers, before NewGame_InitSaveData's
// money/position/record/flag edits. This is not a complete save or playable state.
// Defaults are verified against the pinned SoulSilver USA ARM9 image only.
struct PartialNewGameBlocks {
    std::array<std::byte, 0x2c> player_data;
    std::array<std::byte, 0x44c> vars_flags;
    std::array<std::byte, 0x80> local_field_data;
    std::array<std::byte, 0x79c> bag;
    std::array<std::byte, 0x880> pal_pad;
    std::array<std::byte, 0x1400> map_objects;
    std::array<std::byte, 0x20> link_battle_ruleset;
    std::array<std::byte, 0x170> seal_case;
    std::array<std::byte, 0x3ec> chatot;
    std::array<std::byte, 0xe> special_ribbons;
    std::array<std::byte, 0xf8> gts;
    std::array<std::byte, 0x28> pokeathlon_friendship_records;
    std::array<std::byte, 0x8> follow_mon;
    std::array<std::byte, 0x340> pokedex;
    std::array<std::byte, 0x620> trainer_card;
    std::array<std::byte, 0x8> plaza_survey;
    std::array<std::byte, 0x1294> photo_album;
    std::array<std::byte, 0xb80> pokeathlon;
    std::array<std::byte, 0x80> apricorn_box;
    std::array<std::byte, 0x134> pokewalker;
    std::array<std::byte, 0xf00> trainer_house;
    std::array<std::byte, 0x5b0> party;
    std::array<std::byte, 0x1e0> daycare;
    std::array<std::byte, 0x688> migrated_pokemon;
    std::array<std::byte, 0x834> fashion_data;
    std::array<std::byte, 0x658> pokegear;
};

[[nodiscard]] PartialNewGameBlocks initialize_soulsilver_usa_partial_blocks();

} // namespace hg
