#pragma once

#include <cstddef>
#include <cstdint>
#include <span>
#include <vector>

namespace hg {
struct RgbaImage {
    unsigned width{};
    unsigned height{};
    std::vector<std::uint8_t> pixels; // RGBA byte order, top-left origin.
};

// Uncompressed containers only. The returned view borrows the archive's lifetime.
// Malformed ranges and unsupported formats throw std::runtime_error.
std::span<const std::byte> narc_member(std::span<const std::byte> archive, unsigned member);

// Text backgrounds: 8x8 tiles, 4/8 bpp, maps up to 256x256.
// No affine maps or extended palettes. Palette index zero has transparent alpha.
RgbaImage decode_text_background(std::span<const std::byte> ncgr,
    std::span<const std::byte> nclr, std::span<const std::byte> nscr);

// Overlay at the top-left, clipping to the destination. Only alpha 0/255 is supported.
void composite_layer(RgbaImage& destination, const RgbaImage& layer);
} // namespace hg
