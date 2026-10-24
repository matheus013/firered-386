#include "hg/nitro_compression.hpp"

#include <cstdint>
#include <stdexcept>

namespace hg {
namespace {

class Input {
public:
    explicit Input(std::span<const std::byte> bytes) : bytes_(bytes) {}

    std::uint32_t next() {
        if (position_ == bytes_.size())
            throw std::runtime_error("Truncated Nitro LZ stream");
        return std::to_integer<std::uint32_t>(bytes_[position_++]);
    }

private:
    std::span<const std::byte> bytes_;
    std::size_t position_{};
};

struct Reference {
    std::size_t length;
    std::size_t distance;
};

Reference read_reference(Input& input, bool extended) {
    const auto first = input.next();
    const auto kind = first >> 4;
    std::uint32_t distance_high;
    std::size_t length;

    if (!extended || kind >= 2) {
        length = kind + (extended ? 1u : 3u);
        distance_high = first & 0x0fu;
    } else if (kind == 0) {
        const auto second = input.next();
        length = ((first & 0x0fu) << 4) + (second >> 4) + 0x11u;
        distance_high = second & 0x0fu;
    } else {
        const auto second = input.next();
        const auto third = input.next();
        length = ((first & 0x0fu) << 12) + (second << 4) + (third >> 4) + 0x111u;
        distance_high = third & 0x0fu;
    }

    const auto distance_low = input.next();
    return {length, (distance_high << 8) + distance_low + 1u};
}

void append_reference(std::vector<std::byte>& output, Reference reference,
    std::size_t declared_size) {
    if (reference.distance > output.size())
        throw std::runtime_error("Nitro LZ reference precedes output");
    if (reference.length > declared_size - output.size())
        throw std::runtime_error("Nitro LZ reference exceeds declared output");

    // Recompute the source for every byte: new bytes may be the source of later
    // bytes in this same token (distance one is a valid repeated-byte run).
    for (std::size_t i = 0; i < reference.length; ++i) {
        const auto value = output[output.size() - reference.distance];
        output.push_back(value);
    }
}

} // namespace

std::vector<std::byte> decompress_nitro_lz(std::span<const std::byte> compressed,
    std::size_t max_output_bytes) {
    Input input(compressed);
    const auto type = input.next();
    if (type != 0x10u && type != 0x11u)
        throw std::runtime_error("Unsupported Nitro LZ compression type");

    const auto low = input.next();
    const auto middle = input.next();
    const auto high = input.next();
    const std::size_t declared_size = low | (middle << 8) | (high << 16);
    if (declared_size == 0)
        throw std::runtime_error("Nitro LZ requires a nonzero 24-bit output size");
    if (declared_size > max_output_bytes)
        throw std::runtime_error("Nitro LZ output exceeds configured limit");

    std::vector<std::byte> output;
    output.reserve(declared_size);
    while (output.size() < declared_size) {
        const auto flags = input.next();
        for (unsigned mask = 0x80u; mask != 0 && output.size() < declared_size; mask >>= 1) {
            if ((flags & mask) != 0)
                append_reference(output, read_reference(input, type == 0x11u), declared_size);
            else
                output.push_back(static_cast<std::byte>(input.next()));
        }
    }
    return output;
}

} // namespace hg
