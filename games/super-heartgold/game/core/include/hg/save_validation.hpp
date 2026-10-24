#pragma once

#include "hg/save_image.hpp"
#include "hg/save_selection.hpp"

#include <array>

namespace hg {
struct SaveChunkSpec {
    std::size_t offset;
    std::size_t size;
};

// Fixed DS disk sizes, established from the pinned size functions. Never use
// sizeof a native gameplay struct to calculate these offsets.
inline constexpr std::array<SaveChunkSpec, 2> PrimarySaveChunks{{
    {0, 0xf628}, {0xf700, 0x12310}
}};

enum class SaveChunkError { None, Size, Magic, Slot, Crc };
struct SaveChunkValidation {
    SaveImage::Footer footer{};
    SaveChunkError error{SaveChunkError::None};
    std::uint16_t calculated_crc{};
    bool valid() const noexcept { return error == SaveChunkError::None; }
};

struct PrimarySaveValidation {
    // Indexed [bank][chunk: normal=0, PC=1].
    std::array<std::array<SaveChunkValidation, 2>, 2> chunks;
    SaveSelection reference_selection;
    // Present only if the reference selection points to two individually valid
    // chunks with matching counters. Extra records/game state are NOT checked.
    std::optional<unsigned> readable_primary_bank;
};

std::uint16_t save_crc16(std::span<const std::byte> bytes) noexcept;
PrimarySaveValidation validate_primary_save(const SaveImage& image);
} // namespace hg
