#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <span>
#include <vector>

namespace hg {

// Immutable snapshot of raw HG/SS flash. Loading is not save validation.
// Emulator-specific headers/trailers must be converted explicitly elsewhere.
class SaveImage {
public:
    static constexpr std::size_t Size = 0x80000;
    static constexpr std::size_t BankSize = 0x40000;
    static constexpr std::size_t FooterSize = 16;

    struct Footer {
        std::uint32_t count;
        std::uint32_t size;
        std::uint32_t magic;
        std::uint16_t slot;
        std::uint16_t crc;
    };

    static SaveImage load(const std::filesystem::path& path);
    explicit SaveImage(std::span<const std::byte> bytes);
    std::span<const std::byte> bank_range(unsigned bank, std::size_t offset,
                                          std::size_t length) const;
    Footer footer(unsigned bank, std::size_t offset) const;

private:
    std::vector<std::byte> bytes_;
};

} // namespace hg
