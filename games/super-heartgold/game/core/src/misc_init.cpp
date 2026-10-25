#include "hg/misc_init.hpp"
#include <algorithm>

namespace hg {
InitialMisc initialize_misc(RtcSnapshot snapshot) {
    validate_rtc_snapshot(snapshot);
    InitialMisc bytes{};
    const std::array fields{snapshot.year_since_2000, snapshot.month, snapshot.day,
        snapshot.weekday, snapshot.hour, snapshot.minute, snapshot.second};
    for (std::size_t field = 0; field < fields.size(); ++field)
        for (unsigned octet = 0; octet < 4; ++octet)
            bytes[0x230 + field * 4 + octet] = std::byte((fields[field] >> (octet * 8)) & 255);
    // The original fills eight BYTES, leaving the latter half of rivalName zero.
    std::fill_n(bytes.begin() + 0x270, 8, std::byte{0xff});
    std::fill_n(bytes.begin() + 0x280, 8, std::byte{0xff});
    bytes[0x2a0] = std::byte{4}; // Greeting message bank.
    bytes[0x2a4] = std::byte{1}; // EasyChat "regards", word index 0x501.
    bytes[0x2a5] = std::byte{5};
    std::fill(bytes.begin() + 0x2a6, bytes.begin() + 0x2d0, std::byte{0xff});
    return bytes;
}
} // namespace hg
