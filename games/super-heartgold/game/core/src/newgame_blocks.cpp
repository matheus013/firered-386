#include "hg/newgame_blocks.hpp"
#include "hg/pokemon_crypto.hpp"

#include <algorithm>
#include <span>

namespace hg {
namespace {
// The local per-segment LCRNG is part of the Pokémon storage cipher, not the
// game's global RNG. Empty Pokémon have zero checksum/personality, so every
// independently encrypted region starts with seed zero and plaintext zero.
void initialize_empty_pokemon(std::span<std::byte> pokemon) {
    xor_pokemon_segment(pokemon.subspan(8, 128), 0); // BoxPokemon encrypted blocks
    xor_pokemon_segment(pokemon.subspan(136, 100), 0); // party stats/mail/capsule
}
}

PartialNewGameBlocks initialize_soulsilver_usa_partial_blocks() {
    PartialNewGameBlocks blocks{};

    // PLAYERDATA: Options is at +0; PlayerProfile starts at +4 after padding.
    // Profile language is +25 and version is +28 within that profile.
    blocks.player_data[0] = std::byte{1};  // medium text speed, other options zero
    blocks.player_data[29] = std::byte{2}; // GAME_LANGUAGE: English
    blocks.player_data[32] = std::byte{8}; // GAME_VERSION: SoulSilver

    // Save_LocalFieldData_Init sets only lastSpawn after clearing the whole
    // block and initializing PlayerSaveData to zero. u16 lastSpawn at +104.
    blocks.local_field_data[104] = std::byte{1}; // GetMomSpawnId() == 1

    // Sixteen 136-byte Pal Pad entries. An empty name is the DS text EOS
    // marker (u16 0xffff), not a string of zeroes; the other entry bytes clear.
    for (std::size_t entry = 0; entry < 16; ++entry) {
        blocks.pal_pad[entry * 136] = std::byte{0xff};
        blocks.pal_pad[entry * 136 + 1] = std::byte{0xff};
    }
    // Nine additional blocks have original all-zero initializers. Aggregate
    // initialization clears every byte, independent of host ABI.

    // Pokédex magic in little endian, followed by empty form-history markers.
    blocks.pokedex[0] = std::byte{0xfe};
    blocks.pokedex[1] = std::byte{0xca};
    blocks.pokedex[2] = std::byte{0xef};
    blocks.pokedex[3] = std::byte{0xbe};
    blocks.pokedex[67] = blocks.pokedex[131] = std::byte{0xff}; // Deoxys nibbles
    std::fill_n(blocks.pokedex.begin() + 264, 60, std::byte{0xff});
    std::fill_n(blocks.pokedex.begin() + 824, 7, std::byte{0xff});

    for (std::size_t badge = 0; badge < 8; ++badge)
        blocks.trainer_card[badge * 4] = std::byte{0x8c};
    std::fill_n(blocks.plaza_survey.begin(), 4, std::byte{0xff}); // no prior survey

    // Album header is four bytes; every 132-byte photo has two EOS-filled names
    // at +8 (8 u16 units) and +24 (12 units), a contiguous 40-byte range.
    for (std::size_t photo = 0; photo < 36; ++photo)
        std::fill_n(blocks.photo_album.begin() + 4 + photo * 132 + 8, 40, std::byte{0xff});

    // Ten solo/link event records, each with five unrecorded u16 scores.
    for (std::size_t event = 0; event < 10; ++event)
        for (std::size_t entry = 0; entry < 5; ++entry) {
            std::fill_n(blocks.pokeathlon.begin() + 0x2cc + event * 44 + entry * 8, 2, std::byte{0xff});
            std::fill_n(blocks.pokeathlon.begin() + 0x484 + event * 164 + entry * 8, 2, std::byte{0xff});
        }
    for (std::size_t record = 0; record < 3; ++record)
        std::fill_n(blocks.apricorn_box.begin() + 32 + record * 32 + 8, 16, std::byte{0xff});
    blocks.pokewalker[304] = std::byte{3}; // initial courses 0 and 1 unlocked

    // Ten 384-byte Trainer House sets: trainer name +8, then six 56-byte
    // Pokémon records after the 48-byte trainer; nickname occupies +36..55.
    for (std::size_t set = 0; set < 10; ++set) {
        const auto start = set * 384;
        std::fill_n(blocks.trainer_house.begin() + start + 8, 16, std::byte{0xff});
        for (std::size_t mon = 0; mon < 6; ++mon)
            std::fill_n(blocks.trainer_house.begin() + start + 48 + mon * 56 + 36, 20, std::byte{0xff});
    }

    blocks.party[0] = std::byte{6}; // capacity, current count remains zero
    for (std::size_t slot = 0; slot < 6; ++slot) {
        initialize_empty_pokemon(std::span(blocks.party).subspan(8 + slot * 236, 236));
        initialize_empty_pokemon(std::span(blocks.migrated_pokemon).subspan(slot * 236, 236));
    }
    // Two 236-byte DaycareMon records contain a 136-byte BoxPokemon followed
    // by clear mail/name/steps extras. Only their 128-byte box payload encrypts.
    for (std::size_t slot = 0; slot < 2; ++slot)
        xor_pokemon_segment(std::span(blocks.daycare).subspan(slot * 236 + 8, 128), 0);

    // Empty Fashion entries use magic 0x00001234. The case's 18 wallpaper
    // order bytes use 18 as the empty marker, not an unlocked wallpaper ID.
    const auto empty_fashion_entry = [&](std::size_t offset) {
        blocks.fashion_data[offset] = std::byte{0x34};
        blocks.fashion_data[offset + 1] = std::byte{0x12};
    };
    for (std::size_t entry = 0; entry < 11; ++entry) empty_fashion_entry(entry * 116);
    for (std::size_t entry = 0; entry < 5; ++entry) empty_fashion_entry(0x4fc + entry * 152);
    std::fill_n(blocks.fashion_data.begin() + 0x81c, 18, std::byte{18});

    blocks.pokegear[0] = std::byte{3};
    blocks.pokegear[2] = blocks.pokegear[3] = std::byte{128}; // radio cursor
    for (std::size_t contact = 0; contact < 75; ++contact)
        blocks.pokegear[0x4b8 + contact * 4] = std::byte{4}; // rematch startup bit
    blocks.pokegear[0x5fe] = std::byte{7}; // Kenji wait days, inactive
    // Contact zero is Mother; the remaining 74 contacts are absent (0xff).
    std::fill_n(blocks.pokegear.begin() + 0x60d, 74, std::byte{0xff});
    return blocks;
}

} // namespace hg
