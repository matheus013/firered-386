#pragma once

#include <cstddef>
#include <cstdint>
#include <span>
#include <vector>

namespace hg {

struct MessageLimits {
    std::size_t max_messages = 65535;
    std::size_t max_encoded_units = 65536;
    std::size_t max_decoded_units = 131072;
};

struct DecodedMessage {
    // Original glyph/control IDs, not Unicode. Includes the final 0xFFFF.
    std::vector<std::uint16_t> code_units;
    bool packed_trainer_name{};
};

// A bank is one message NARC member, not the outer NARC. All functions borrow
// only during the call; returned vectors own their contents. Malformed ranges,
// missing terminators and configured limits throw std::runtime_error.
std::size_t message_count(std::span<const std::byte> bank, MessageLimits limits = {});

// Exact decrypted entry, preserving all control words, arguments, packed-name
// marker/words and the outer 0xFFFF terminator. No guessed character mapping.
std::vector<std::uint16_t> decrypt_message_units(std::span<const std::byte> bank,
    std::size_t message_id, MessageLimits limits = {});

// Ordinary entries remain unchanged. A leading 0xF100 expands 9-bit IDs from
// 15-bit packed words; packed EOS 0x1FF becomes 0xFFFF and the marker is removed.
// Does not interpret control commands, expand placeholders or render fonts.
DecodedMessage decode_message_data(std::span<const std::byte> bank,
    std::size_t message_id, MessageLimits limits = {});

} // namespace hg
