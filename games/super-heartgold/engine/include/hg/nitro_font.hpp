#pragma once

#include <cstddef>
#include <cstdint>
#include <span>
#include <vector>

namespace hg {
struct FontGlyph {
    unsigned width{};
    unsigned height{};
    // Row-major roles: 0 transparent, 1 foreground, 2 shadow, 3 background.
    std::vector<std::uint8_t> roles;
};

// Owns one custom HG/SS font NARC member, not an NFTR or an entire archive.
class NitroFont {
public:
    explicit NitroFont(std::span<const std::byte> bytes, bool fixed_width = false);
    unsigned glyph_count() const noexcept { return count_; }
    FontGlyph glyph(unsigned glyph_id) const; // Original one-based glyph IDs.

private:
    std::vector<std::byte> bytes_;
    std::size_t glyph_offset_{};
    std::size_t width_offset_{};
    unsigned count_{};
    unsigned tiles_wide_{};
    unsigned tiles_high_{};
    unsigned fixed_width_{};
    unsigned height_{};
    bool use_fixed_width_{};
};
} // namespace hg
