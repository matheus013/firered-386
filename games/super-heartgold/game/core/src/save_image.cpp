#include "hg/save_image.hpp"

#include <fstream>
#include <stdexcept>

namespace {
std::uint16_t read16(std::span<const std::byte> data, std::size_t offset) {
    return static_cast<std::uint16_t>(std::to_integer<unsigned>(data[offset]) |
        (std::to_integer<unsigned>(data[offset + 1]) << 8));
}
std::uint32_t read32(std::span<const std::byte> data, std::size_t offset) {
    return static_cast<std::uint32_t>(read16(data, offset)) |
        (static_cast<std::uint32_t>(read16(data, offset + 2)) << 16);
}
}

namespace hg {
SaveImage::SaveImage(std::span<const std::byte> bytes) {
    if (bytes.size() != Size)
        throw std::invalid_argument("Expected a raw 512 KiB HG/SS flash image");
    bytes_.assign(bytes.begin(), bytes.end());
}

SaveImage SaveImage::load(const std::filesystem::path& path) {
    // No write mode, file creation, resizing or save repair in this interface.
    std::ifstream stream(path, std::ios::binary | std::ios::ate);
    if (!stream) throw std::runtime_error("Cannot open save image for reading");
    if (stream.tellg() != static_cast<std::streamoff>(Size))
        throw std::invalid_argument("Expected a raw 512 KiB HG/SS flash image");
    std::vector<std::byte> bytes(Size);
    stream.seekg(0);
    stream.read(reinterpret_cast<char*>(bytes.data()), static_cast<std::streamsize>(Size));
    if (!stream) throw std::runtime_error("Cannot read complete save image");
    // Detect growth after the size check, too; never silently strip a trailer.
    if (stream.peek() != std::char_traits<char>::eof())
        throw std::runtime_error("Save image size changed while reading");
    if (stream.bad()) throw std::runtime_error("Save image read failed");
    return SaveImage(bytes);
}

std::span<const std::byte> SaveImage::bank_range(unsigned bank, std::size_t offset,
                                               std::size_t length) const {
    if (bytes_.size() != Size) throw std::logic_error("Save image has no snapshot");
    if (bank > 1 || offset > BankSize || length > BankSize - offset)
        throw std::out_of_range("Save read exceeds flash bank");
    return std::span<const std::byte>(bytes_).subspan(bank * BankSize + offset, length);
}

SaveImage::Footer SaveImage::footer(unsigned bank, std::size_t offset) const {
    const auto data = bank_range(bank, offset, FooterSize);
    return {read32(data, 0), read32(data, 4), read32(data, 8),
            read16(data, 12), read16(data, 14)};
}
} // namespace hg
