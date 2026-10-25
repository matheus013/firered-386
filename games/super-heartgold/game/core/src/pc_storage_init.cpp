#include "hg/pc_storage_init.hpp"
#include "hg/message_data.hpp"
#include "hg/pokemon_crypto.hpp"

#include <array>
#include <cstdint>
#include <stdexcept>
#include <vector>

namespace hg {

PreclearedPcStorageBlock initialize_precleared_pc_storage(std::span<const std::byte> bank) {
    if (bank.size() > 16 * 1024 * 1024)
        throw std::runtime_error("PC name bank exceeds diagnostic limit");
    if (message_count(bank) < 24)
        throw std::runtime_error("PC name bank has fewer than 24 messages");
    std::array<std::vector<std::uint16_t>, 18> names;
    // Validate all input before spending time encrypting the 540 empty records.
    for (std::size_t box = 0; box < names.size(); ++box) {
        // Original ReadMsgDataIntoU16Array copies raw decrypted units, without
        // the packed-name expansion used by String APIs.
        names[box] = decrypt_message_units(bank, box + 6);
        if (names[box].empty() || names[box].size() > 20 || names[box].front() == 0xf100)
            throw std::runtime_error("PC box name exceeds supported array format");
    }
    PreclearedPcStorageBlock result{};
    for (std::size_t box = 0; box < names.size(); ++box) {
        for (std::size_t mon = 0; mon < 30; ++mon)
            xor_pokemon_segment(std::span(result.bytes).subspan(box * 0x1000 + mon * 136 + 8, 128), 0);
        for (std::size_t unit = 0; unit < names[box].size(); ++unit) {
            const auto offset = 0x12008 + box * 40 + unit * 2;
            result.bytes[offset] = static_cast<std::byte>(names[box][unit] & 0xff);
            result.bytes[offset + 1] = static_cast<std::byte>(names[box][unit] >> 8);
        }
        result.bytes[0x122d8 + box] = static_cast<std::byte>(box % 16);
    }
    // curBox is zero by the original initializer. Modified flag, per-box
    // padding, unused name tails and final filler are zero due to pre-clear.
    return result;
}

} // namespace hg
