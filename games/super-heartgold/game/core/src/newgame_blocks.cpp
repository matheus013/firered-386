#include "hg/newgame_blocks.hpp"

namespace hg {

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
    return blocks;
}

} // namespace hg
