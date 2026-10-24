#pragma once
#include <cstddef>
#include <cstdint>
#include <span>
#include <vector>

namespace hg {
struct RgbaImage {
    unsigned width{}, height{};
    std::vector<std::uint8_t> pixels; // RGBA byte order, top-left origin
};

// Uncompressed resource containers only. Returned span borrows the archive.
std::span<const std::byte> narc_member(std::span<const std::byte> archive, unsigned member);
// Text BG: 8x8 tiles, 4/8 bpp, maps up to 256x256; no affine/extended palettes.
RgbaImage decode_text_background(std::span<const std::byte> ncgr,
    std::span<const std::byte> nclr, std::span<const std::byte> nscr);
void composite_layer(RgbaImage& destination, const RgbaImage& layer);
} // namespace hg
