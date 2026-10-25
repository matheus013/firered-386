#include "hg/pokemon_crypto.hpp"

#include <stdexcept>

namespace hg {
namespace {
void validate_segment(std::size_t size) {
    if (size % 2 != 0 || size > PokemonCryptoMaxBytes)
        throw std::runtime_error("Pokemon segment requires an even size of at most 128KiB");
}
} // namespace

void xor_pokemon_segment(std::span<std::byte> segment, std::uint32_t seed) {
    validate_segment(segment.size());
    for (std::size_t at = 0; at < segment.size(); at += 2) {
        seed = seed * 1103515245u + 24691u;
        segment[at] ^= std::byte((seed >> 16) & 255u);
        segment[at + 1] ^= std::byte(seed >> 24);
    }
}

std::uint16_t pokemon_checksum(std::span<const std::byte> segment) {
    validate_segment(segment.size());
    std::uint32_t sum = 0;
    for (std::size_t at = 0; at < segment.size(); at += 2) {
        sum += std::to_integer<std::uint32_t>(segment[at]) |
            (std::to_integer<std::uint32_t>(segment[at + 1]) << 8);
    }
    return static_cast<std::uint16_t>(sum);
}
} // namespace hg
