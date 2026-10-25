#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <span>

namespace hg {

struct SaveBlockLayout {
    unsigned id;
    unsigned slot; // Primary chunk group: normal=0, PC storage=1, not save bank.
    std::size_t raw_size;
    std::size_t aligned_size;
    std::size_t stored_size; // Aligned payload followed by a four-byte trailer.
    std::size_t offset; // Relative to the dynamic region, not SaveData/flash.
};

// Authorial storage and per-block CRC service, not a DS SaveData ABI, flash
// image, complete New Game initializer or working-copy registry. Construction
// clears all bytes but does not run block initializers or generate CRCs.
class SaveContext {
public:
    static constexpr std::size_t BlockCount = 42;
    static constexpr std::size_t DynamicRegionSize = 0x23000;

    SaveContext() = default;
    SaveContext(const SaveContext&) = delete;
    SaveContext& operator=(const SaveContext&) = delete;
    SaveContext(SaveContext&&) = delete;
    SaveContext& operator=(SaveContext&&) = delete;

    static std::span<const SaveBlockLayout, BlockCount> layouts() noexcept;
    const SaveBlockLayout& layout(std::size_t id) const;

    // Includes alignment padding and the trailer. Views remain valid until this
    // context is destroyed; no operation resizes or relocates its storage.
    std::span<std::byte> view(std::size_t id);
    std::span<const std::byte> view(std::size_t id) const;
    std::span<std::byte> raw_view(std::size_t id);
    std::span<const std::byte> raw_view(std::size_t id) const;
    std::span<std::byte, DynamicRegionSize> storage() noexcept;
    std::span<const std::byte, DynamicRegionSize> storage() const noexcept;

    // CRC-16 over the aligned payload, stored LE in trailer bytes 0..1. Bytes
    // 2..3 remain untouched. Does not change any array-header CRC or bank footer.
    // Invalid block IDs throw std::runtime_error before any mutation.
    std::uint16_t update_crc(std::size_t id);

private:
    std::array<std::byte, DynamicRegionSize> bytes_{};
};

} // namespace hg
