#include "hg/nitro_graphics.hpp"

#include <algorithm>
#include <cstring>
#include <stdexcept>
#include <string_view>

namespace hg {
namespace {
using Bytes = std::span<const std::byte>;

constexpr unsigned tile_side = 8;
constexpr unsigned rgba_channels = 4;
constexpr unsigned tile_index_mask = 0x03ff;
constexpr unsigned horizontal_flip_mask = 0x0400;
constexpr unsigned vertical_flip_mask = 0x0800;
constexpr std::size_t container_header_size = 16;
constexpr std::size_t block_header_size = 8;

void require(bool condition, const char* message)
{
    if (!condition) throw std::runtime_error(message);
}

Bytes checked_slice(Bytes bytes, std::size_t offset, std::size_t size)
{
    require(offset <= bytes.size() && size <= bytes.size() - offset,
        "Truncated resource range");
    return bytes.subspan(offset, size);
}

unsigned read_u16(Bytes bytes, std::size_t offset)
{
    const auto value = checked_slice(bytes, offset, 2);
    return std::to_integer<unsigned>(value[0])
        | (std::to_integer<unsigned>(value[1]) << 8);
}

std::uint32_t read_u32(Bytes bytes, std::size_t offset)
{
    return read_u16(bytes, offset)
        | (static_cast<std::uint32_t>(read_u16(bytes, offset + 2)) << 16);
}

bool matches_signature(Bytes bytes, std::size_t offset, std::string_view signature)
{
    const auto value = checked_slice(bytes, offset, signature.size());
    return std::memcmp(value.data(), signature.data(), signature.size()) == 0;
}

Bytes find_block(Bytes bytes, std::string_view container, std::string_view wanted)
{
    require(matches_signature(bytes, 0, container),
        "Unsupported resource signature (compression not supported)");
    const auto byte_order_marker = container == "NARC" ? 0xfffeu : 0xfeffu;
    const auto version = read_u16(bytes, 6);
    require(read_u16(bytes, 4) == byte_order_marker && (version == 0x100 || version == 0x101),
        "Unsupported resource byte order/version");
    require(read_u32(bytes, 8) == bytes.size() && read_u16(bytes, 12) == container_header_size,
        "Invalid resource header size");
    const auto block_count = read_u16(bytes, 14);
    require(block_count > 0 && block_count < 256, "Invalid resource block count");

    Bytes found;
    std::size_t cursor = container_header_size;
    for (unsigned index = 0; index < block_count; ++index) {
        const auto size = read_u32(bytes, cursor + 4);
        require(size >= block_header_size, "Invalid resource block size");
        const auto candidate = checked_slice(bytes, cursor, size);
        if (matches_signature(candidate, 0, wanted)) {
            require(found.empty(), "Duplicate resource block");
            found = candidate;
        }
        cursor += size;
    }
    require(cursor == bytes.size() && !found.empty(), "Missing resource block or trailing bytes");
    return found;
}

struct CharacterData {
    Bytes tiles;
    bool four_bit;

    unsigned bytes_per_tile() const { return four_bit ? 32u : 64u; }
};

CharacterData read_characters(Bytes resource)
{
    const auto characters = find_block(resource, "RGCN", "RAHC");
    const auto format = read_u32(characters, 12);
    require(format == 3 || format == 4, "Unsupported character color format");
    require(read_u32(characters, 20) == 0, "Bitmap/scanned characters are not supported");
    const CharacterData result{
        checked_slice(characters, block_header_size + static_cast<std::size_t>(read_u32(characters, 28)),
            read_u32(characters, 24)),
        format == 3};
    require(!result.tiles.empty() && result.tiles.size() % result.bytes_per_tile() == 0,
        "Invalid character data length");
    return result;
}

Bytes read_colors(Bytes resource)
{
    const auto palette = find_block(resource, "RLCN", "TTLP");
    const auto format = read_u32(palette, 8);
    require(format == 3 || format == 4, "Unsupported palette format");
    require(read_u32(palette, 12) == 0, "Extended palette resources are not supported");
    const auto colors = checked_slice(palette,
        block_header_size + static_cast<std::size_t>(read_u32(palette, 20)), read_u32(palette, 16));
    require(!colors.empty() && colors.size() % 2 == 0 && colors.size() <= 512,
        "Invalid standard palette size");
    return colors;
}

unsigned read_pixel_index(const CharacterData& characters, unsigned entry, unsigned x, unsigned y)
{
    const auto tile_index = entry & tile_index_mask;
    require(tile_index < characters.tiles.size() / characters.bytes_per_tile(),
        "Tile index outside character data");
    const auto tile_x = x % tile_side;
    const auto tile_y = y % tile_side;
    const auto flipped_x = entry & horizontal_flip_mask ? tile_side - 1 - tile_x : tile_x;
    const auto flipped_y = entry & vertical_flip_mask ? tile_side - 1 - tile_y : tile_y;
    const auto pixel = flipped_y * tile_side + flipped_x;
    const auto byte_offset = tile_index * characters.bytes_per_tile()
        + (characters.four_bit ? pixel / 2 : pixel);
    const auto packed = std::to_integer<unsigned>(characters.tiles[byte_offset]);
    return characters.four_bit ? (packed >> ((pixel % 2) * 4)) & 15u : packed;
}

std::uint8_t expand_five_bits(unsigned value)
{
    return static_cast<std::uint8_t>((value << 3) | (value >> 2));
}

void write_color(RgbaImage& image, std::size_t offset, unsigned color, bool transparent)
{
    image.pixels[offset] = expand_five_bits(color & 31);
    image.pixels[offset + 1] = expand_five_bits((color >> 5) & 31);
    image.pixels[offset + 2] = expand_five_bits((color >> 10) & 31);
    image.pixels[offset + 3] = transparent ? 0 : 255;
}

void validate_image(const RgbaImage& image)
{
    require(image.width <= 4096 && image.height <= 4096, "Composition dimensions exceed limit");
    require(image.pixels.size() == static_cast<std::size_t>(image.width) * image.height * rgba_channels,
        "Invalid RGBA image buffer");
}
} // namespace

Bytes narc_member(Bytes archive, unsigned member)
{
    const auto allocations = find_block(archive, "NARC", "BTAF");
    const auto image = find_block(archive, "NARC", "GMIF");
    const auto member_count = read_u16(allocations, 8);
    require(allocations.size() >= 12 && member_count <= (allocations.size() - 12) / 8,
        "Invalid archive allocation table");
    require(member < member_count, "NARC member outside archive");
    const auto begin = read_u32(allocations, 12 + 8 * member);
    const auto end = read_u32(allocations, 16 + 8 * member);
    require(begin <= end, "Reversed NARC member range");
    return checked_slice(image, block_header_size + static_cast<std::size_t>(begin), end - begin);
}

RgbaImage decode_text_background(Bytes ncgr, Bytes nclr, Bytes nscr)
{
    const auto characters = read_characters(ncgr);
    const auto colors = read_colors(nclr);
    const auto screen = find_block(nscr, "RCSN", "NRCS");
    require(read_u16(screen, 12) == (characters.four_bit ? 0u : 1u) && read_u16(screen, 14) == 0,
        "Unsupported screen color/affine format");
    const auto width = read_u16(screen, 8);
    const auto height = read_u16(screen, 10);
    require(width > 0 && height > 0 && width <= 256 && height <= 256
        && width % tile_side == 0 && height % tile_side == 0, "Unsupported screen dimensions");
    const auto tilemap = checked_slice(screen, 20, read_u32(screen, 16));
    require(tilemap.size() == static_cast<std::size_t>(width) * height / 32, "Invalid tilemap length");

    RgbaImage result{width, height,
        std::vector<std::uint8_t>(static_cast<std::size_t>(width) * height * rgba_channels)};
    for (unsigned y = 0; y < height; ++y) {
        for (unsigned x = 0; x < width; ++x) {
            const auto map_index = (y / tile_side) * (width / tile_side) + x / tile_side;
            const auto entry = read_u16(tilemap, map_index * 2);
            const auto pixel_index = read_pixel_index(characters, entry, x, y);
            const auto palette_index = pixel_index + (characters.four_bit ? (entry >> 12) * 16 : 0);
            const auto color = read_u16(colors, palette_index * 2);
            const auto offset = (static_cast<std::size_t>(y) * width + x) * rgba_channels;
            write_color(result, offset, color, pixel_index == 0);
        }
    }
    return result;
}

void composite_layer(RgbaImage& destination, const RgbaImage& layer)
{
    validate_image(destination);
    validate_image(layer);
    const auto width = std::min(destination.width, layer.width);
    const auto height = std::min(destination.height, layer.height);
    for (unsigned y = 0; y < height; ++y) {
        for (unsigned x = 0; x < width; ++x) {
            const auto source = (static_cast<std::size_t>(y) * layer.width + x) * rgba_channels;
            const auto target = (static_cast<std::size_t>(y) * destination.width + x) * rgba_channels;
            const auto alpha = layer.pixels[source + 3];
            require(alpha == 0 || alpha == 255, "Partial layer alpha not supported");
            if (alpha != 0) {
                std::copy_n(layer.pixels.begin() + source, rgba_channels, destination.pixels.begin() + target);
            }
        }
    }
}
} // namespace hg
