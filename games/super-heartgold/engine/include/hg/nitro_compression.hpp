#pragma once

#include <cstddef>
#include <span>
#include <vector>

namespace hg {

inline constexpr std::size_t default_lz_output_limit = 16u * 1024u * 1024u;

// Nintendo LZ10/LZ11 with a nonzero 24-bit output-size header. Supports all
// three LZ11 token-length forms and overlapping references. Bytes after the
// declared output are ignored (containers may include alignment padding).
// Malformed input, unsupported types and output beyond the caller's limit
// throw std::runtime_error. No partially decoded result escapes on failure.
std::vector<std::byte> decompress_nitro_lz(std::span<const std::byte> compressed,
    std::size_t max_output_bytes = default_lz_output_limit);

} // namespace hg
