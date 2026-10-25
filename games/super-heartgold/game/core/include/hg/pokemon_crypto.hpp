#pragma once

#include <cstddef>
#include <cstdint>
#include <span>

namespace hg {

inline constexpr std::size_t PokemonCryptoMaxBytes = 128 * 1024;

// HG/SS segment encoding, not a security primitive. Each call starts its own
// LCRNG from seed. The same operation encrypts/decrypts an even-length sequence
// of little-endian u16 words, without depending on alignment or host byte order.
// Odd or oversized segments throw std::runtime_error before any mutation.
void xor_pokemon_segment(std::span<std::byte> segment, std::uint32_t seed);

// Sum of little-endian u16 words modulo 65536. Accepts an empty segment (zero).
// The same even-length/128KiB bounds apply. Does not interpret a Pokemon layout.
std::uint16_t pokemon_checksum(std::span<const std::byte> segment);

} // namespace hg
