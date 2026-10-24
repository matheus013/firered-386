#include "hg/nitro_font.hpp"

#include <stdexcept>

namespace hg {
namespace {
void require(bool condition, const char* reason) {
    if (!condition) throw std::runtime_error(reason);
}
unsigned byte(std::span<const std::byte> bytes, std::size_t at) {
    require(at < bytes.size(), "Truncated font data");
    return std::to_integer<unsigned>(bytes[at]);
}
std::uint32_t word(std::span<const std::byte> bytes, std::size_t at) {
    return byte(bytes, at) | (byte(bytes, at+1) << 8) |
        (byte(bytes, at+2) << 16) | (byte(bytes, at+3) << 24);
}
}

NitroFont::NitroFont(std::span<const std::byte> bytes, bool fixed_width)
    : use_fixed_width_(fixed_width) {
    require(bytes.size() >= 16 && bytes.size() <= 8*1024*1024, "Invalid font resource size");
    glyph_offset_ = word(bytes, 0);
    width_offset_ = word(bytes, 4);
    count_ = word(bytes, 8);
    fixed_width_ = byte(bytes, 12);
    height_ = byte(bytes, 13);
    tiles_wide_ = byte(bytes, 14);
    tiles_high_ = byte(bytes, 15);
    require(count_ > 0 && count_ <= 65535, "Unsupported font glyph count");
    require(tiles_wide_ >= 1 && tiles_wide_ <= 2 && tiles_high_ >= 1 && tiles_high_ <= 2,
        "Unsupported font glyph shape");
    require(height_ > 0 && height_ <= tiles_high_*8, "Invalid font height");
    require(glyph_offset_ >= 16 && glyph_offset_ <= bytes.size(), "Invalid font glyph offset");
    const std::size_t glyph_bytes = tiles_wide_ * tiles_high_ * 16;
    require(count_ <= (bytes.size() - glyph_offset_) / glyph_bytes, "Truncated font glyphs");
    if (fixed_width) {
        require(fixed_width_ > 0 && fixed_width_ <= tiles_wide_*8, "Invalid fixed font width");
    } else {
        require(width_offset_ >= glyph_offset_ + count_*glyph_bytes && width_offset_ <= bytes.size() &&
                count_ <= bytes.size() - width_offset_, "Invalid font width table");
        for (unsigned id = 0; id < count_; ++id)
            require(byte(bytes, width_offset_ + id) <= tiles_wide_*8, "Invalid variable glyph width");
    }
    bytes_.assign(bytes.begin(), bytes.end());
}

FontGlyph NitroFont::glyph(unsigned glyph_id) const {
    require(!bytes_.empty(), "Font has no owned data");
    require(glyph_id != 0, "Zero is not a font glyph ID");
    // Original replacement glyph, when the font actually contains it. Small
    // synthetic/custom fonts reject missing glyphs rather than indexing past end.
    if (glyph_id > count_) {
        require(count_ >= 428, "Glyph outside font");
        glyph_id = 428;
    }
    const auto index = glyph_id - 1;
    const auto width = use_fixed_width_ ? fixed_width_ : byte(bytes_, width_offset_ + index);
    FontGlyph result{width, height_, std::vector<std::uint8_t>(width * height_)};
    const auto glyph_start = glyph_offset_ + index * tiles_wide_ * tiles_high_ * 16;
    for (unsigned y = 0; y < result.height; ++y) for (unsigned x = 0; x < result.width; ++x) {
        const auto tile = (y/8)*tiles_wide_ + x/8;
        const auto row = glyph_start + tile*16 + (y%8)*2;
        const auto bits = byte(bytes_, row) | (byte(bytes_, row+1) << 8);
        result.roles[y*width+x] = static_cast<std::uint8_t>((bits >> (14 - (x%8)*2)) & 3);
    }
    return result;
}
} // namespace hg
