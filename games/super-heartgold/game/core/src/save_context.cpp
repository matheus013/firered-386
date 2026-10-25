#include "hg/save_context.hpp"
#include "hg/save_validation.hpp"

#include <stdexcept>

namespace hg {
namespace {
// Fixed raw sizes proven by the pinned ARM9 size functions. Offsets follow the
// original aligned-size + trailer rule, including the primary-chunk gap.
constexpr std::array<SaveBlockLayout, SaveContext::BlockCount> Layouts{{
    {0, 0, 0x5c, 0x5c, 0x60, 0x0},
    {1, 0, 0x2c, 0x2c, 0x30, 0x60},
    {2, 0, 0x5b0, 0x5b0, 0x5b4, 0x90},
    {3, 0, 0x79c, 0x79c, 0x7a0, 0x644},
    {4, 0, 0x44c, 0x44c, 0x450, 0xde4},
    {5, 0, 0x80, 0x80, 0x84, 0x1234},
    {6, 0, 0x340, 0x340, 0x344, 0x12b8},
    {7, 0, 0x1e0, 0x1e0, 0x1e4, 0x15fc},
    {8, 0, 0x880, 0x880, 0x884, 0x17e0},
    {9, 0, 0x2e0, 0x2e0, 0x2e4, 0x2064},
    {10, 0, 0x1400, 0x1400, 0x1404, 0x2348},
    {11, 0, 0x20, 0x20, 0x24, 0x374c},
    {12, 0, 0x834, 0x834, 0x838, 0x3770},
    {13, 0, 0x460, 0x460, 0x464, 0x3fa8},
    {14, 0, 0x108, 0x108, 0x10c, 0x440c},
    {15, 0, 0x620, 0x620, 0x624, 0x4518},
    {16, 0, 0x1c0, 0x1c0, 0x1c4, 0x4b3c},
    {17, 0, 0x170, 0x170, 0x174, 0x4d00},
    {18, 0, 0x3ec, 0x3ec, 0x3f0, 0x4e74},
    {19, 0, 0x1628, 0x1628, 0x162c, 0x5264},
    {20, 0, 0xe, 0x10, 0x14, 0x6890},
    {21, 0, 0x68, 0x68, 0x6c, 0x68a4},
    {22, 0, 0xf8, 0xf8, 0xfc, 0x6910},
    {23, 0, 0xbc8, 0xbc8, 0xbcc, 0x6a0c},
    {24, 0, 0xea0, 0xea0, 0xea4, 0x75d8},
    {25, 0, 0x8c0, 0x8c0, 0x8c4, 0x847c},
    {26, 0, 0xff8, 0xff8, 0xffc, 0x8d40},
    {27, 0, 0x1680, 0x1680, 0x1684, 0x9d3c},
    {28, 0, 0x688, 0x688, 0x68c, 0xb3c0},
    {29, 0, 0x28, 0x28, 0x2c, 0xba4c},
    {30, 0, 0x8, 0x8, 0xc, 0xba78},
    {31, 0, 0x40, 0x40, 0x44, 0xba84},
    {32, 0, 0x8, 0x8, 0xc, 0xbac8},
    {33, 0, 0x8, 0x8, 0xc, 0xbad4},
    {34, 0, 0x658, 0x658, 0x65c, 0xbae0},
    {35, 0, 0x5fc, 0x5fc, 0x600, 0xc13c},
    {36, 0, 0x1294, 0x1294, 0x1298, 0xc73c},
    {37, 0, 0xb80, 0xb80, 0xb84, 0xd9d4},
    {38, 0, 0x80, 0x80, 0x84, 0xe558},
    {39, 0, 0x134, 0x134, 0x138, 0xe5dc},
    {40, 0, 0xf00, 0xf00, 0xf04, 0xe714},
    {41, 1, 0x122fc, 0x122fc, 0x12300, 0xf700},
}};

constexpr bool valid_layouts() {
    std::size_t next_offset = 0;
    for (std::size_t id = 0; id < Layouts.size(); ++id) {
        const auto& block = Layouts[id];
        if (block.id != id || block.slot != (id == 41 ? 1u : 0u) ||
            block.aligned_size != ((block.raw_size + 3) & ~std::size_t{3}) ||
            block.stored_size != block.aligned_size + 4 || block.offset != next_offset ||
            block.offset + block.stored_size > SaveContext::DynamicRegionSize)
            return false;
        next_offset += block.stored_size;
        if (id == 40) next_offset = (next_offset + 16 + 255) & ~std::size_t{255};
    }
    return Layouts[40].offset + Layouts[40].stored_size + 16 == PrimarySaveChunks[0].size &&
        Layouts[41].offset == PrimarySaveChunks[1].offset &&
        Layouts[41].stored_size + 16 == PrimarySaveChunks[1].size;
}
static_assert(valid_layouts());
} // namespace

std::span<const SaveBlockLayout, SaveContext::BlockCount> SaveContext::layouts() noexcept {
    return Layouts;
}

const SaveBlockLayout& SaveContext::layout(std::size_t id) const {
    if (id >= BlockCount) throw std::runtime_error("Save block ID is outside 0..41");
    return Layouts[id];
}

std::span<std::byte> SaveContext::view(std::size_t id) {
    const auto& block = layout(id);
    return std::span(bytes_).subspan(block.offset, block.stored_size);
}

std::span<const std::byte> SaveContext::view(std::size_t id) const {
    const auto& block = layout(id);
    return std::span(bytes_).subspan(block.offset, block.stored_size);
}

std::span<std::byte> SaveContext::raw_view(std::size_t id) {
    return view(id).first(layout(id).raw_size);
}

std::span<const std::byte> SaveContext::raw_view(std::size_t id) const {
    return view(id).first(layout(id).raw_size);
}

std::span<std::byte, SaveContext::DynamicRegionSize> SaveContext::storage() noexcept { return bytes_; }
std::span<const std::byte, SaveContext::DynamicRegionSize> SaveContext::storage() const noexcept { return bytes_; }

std::uint16_t SaveContext::update_crc(std::size_t id) {
    auto block = view(id);
    const auto payload_size = layout(id).aligned_size;
    const auto crc = save_crc16(block.first(payload_size));
    block[payload_size] = std::byte(crc & 255);
    block[payload_size + 1] = std::byte(crc >> 8);
    return crc;
}
} // namespace hg
