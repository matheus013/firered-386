#include "hg/message_data.hpp"

#include <stdexcept>
#include <utility>

namespace hg {
namespace {
constexpr std::uint16_t end_of_message = 0xffff;
constexpr std::uint16_t packed_name_marker = 0xf100;
constexpr std::uint16_t end_of_packed_name = 0x1ff;

void require(bool condition, const char* reason) {
    if (!condition) throw std::runtime_error(reason);
}

std::uint16_t read_u16(std::span<const std::byte> bytes, std::size_t offset) {
    require(offset <= bytes.size() && bytes.size() - offset >= 2,
        "Truncated message bank");
    return static_cast<std::uint16_t>(std::to_integer<unsigned>(bytes[offset])
        | (std::to_integer<unsigned>(bytes[offset + 1]) << 8));
}

std::uint32_t read_u32(std::span<const std::byte> bytes, std::size_t offset) {
    return read_u16(bytes, offset) | (static_cast<std::uint32_t>(read_u16(bytes, offset + 2)) << 16);
}

std::vector<std::uint16_t> unpack_name(std::span<const std::uint16_t> words,
    std::size_t limit) {
    std::vector<std::uint16_t> result;
    std::uint32_t pending{};
    unsigned available{};
    std::size_t cursor = 1; // Skip the 0xF100 marker.
    while (true) {
        while (available < 9) {
            require(cursor < words.size(), "Truncated packed message");
            // The last outer 0xFFFF also supplies packed end bits. Every
            // preceding packed word is generated with its spare bit clear.
            const auto word = words[cursor];
            require(cursor + 1 == words.size() || (word & 0x8000u) == 0,
                "Invalid spare bit in packed message");
            pending |= static_cast<std::uint32_t>(word & 0x7fffu) << available;
            available += 15;
            ++cursor;
        }
        const auto code = static_cast<std::uint16_t>(pending & 0x1ffu);
        pending >>= 9;
        available -= 9;
        require(result.size() < limit, "Decoded message exceeds configured limit");
        result.push_back(code == end_of_packed_name ? end_of_message : code);
        if (code == end_of_packed_name) return result;
    }
}
} // namespace

std::size_t message_count(std::span<const std::byte> bank, MessageLimits limits) {
    require(bank.size() >= 4, "Truncated message header");
    const auto count = read_u16(bank, 0);
    require(count <= limits.max_messages, "Message count exceeds configured limit");
    require(count <= (bank.size() - 4) / 8, "Truncated message allocation table");
    return count;
}

std::vector<std::uint16_t> decrypt_message_units(std::span<const std::byte> bank,
    std::size_t message_id, MessageLimits limits) {
    const auto count = message_count(bank, limits);
    require(message_id < count, "Message ID outside bank");
    const auto entry = 4 + message_id * 8;
    const auto index = static_cast<std::uint32_t>(message_id + 1);
    const auto table_seed = (static_cast<std::uint32_t>(read_u16(bank, 2)) * 765u * index) & 0xffffu;
    const auto table_mask = table_seed | (table_seed << 16);
    const std::size_t offset = read_u32(bank, entry) ^ table_mask;
    const std::size_t length = read_u32(bank, entry + 4) ^ table_mask;
    require(length != 0 && length <= limits.max_encoded_units,
        "Encoded message length exceeds configured limit or is zero");
    require(offset >= 4 + count * 8 && offset % 2 == 0,
        "Invalid message payload offset");
    require(offset <= bank.size() && length <= (bank.size() - offset) / 2,
        "Message payload outside bank");

    std::vector<std::uint16_t> result;
    result.reserve(length);
    auto seed = static_cast<std::uint16_t>(index * 596947u);
    for (std::size_t i = 0; i < length; ++i) {
        result.push_back(static_cast<std::uint16_t>(read_u16(bank, offset + i * 2) ^ seed));
        seed = static_cast<std::uint16_t>(seed + 18749u);
    }
    require(result.back() == end_of_message, "Message lacks final terminator");
    return result;
}

DecodedMessage decode_message_data(std::span<const std::byte> bank,
    std::size_t message_id, MessageLimits limits) {
    auto units = decrypt_message_units(bank, message_id, limits);
    if (units.front() == packed_name_marker)
        return {unpack_name(units, limits.max_decoded_units), true};
    require(units.size() <= limits.max_decoded_units,
        "Decoded message exceeds configured limit");
    return {std::move(units), false};
}

} // namespace hg
