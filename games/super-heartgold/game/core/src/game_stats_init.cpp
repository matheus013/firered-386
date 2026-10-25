#include "hg/game_stats_init.hpp"
#include "hg/pokemon_crypto.hpp"

#include <span>

namespace hg {
InitialGameStats initialize_game_stats(std::uint32_t vblank_count) {
    InitialGameStats bytes{};
    const auto key = static_cast<std::uint16_t>(vblank_count | (vblank_count << 8));
    // Original two unencrypted words precede the secure region. Its last
    // word is the zero dummy field; the checksum and key are outside it.
    xor_pokemon_segment(std::span(bytes).subspan(8, 436), static_cast<std::uint32_t>(key) << 16);
    bytes[446] = std::byte(key & 255);
    bytes[447] = std::byte(key >> 8);
    return bytes;
}
} // namespace hg
