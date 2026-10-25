#pragma once
#include <array>
#include <cstddef>
#include <cstdint>

namespace hg {
struct RtcSnapshot {
    std::uint32_t year_since_2000, month, day, weekday; // Sunday=0; copied as supplied.
    std::uint32_t hour, minute, second;
};
// Shared calendar validation for save blocks consuming an RTC snapshot.
void validate_rtc_snapshot(RtcSnapshot snapshot);
using InitialSystemInfo = std::array<std::byte, 0x5c>;
// Raw SysInfo_Init only, with a validated, consistent-in-time RTC sample.
// Owner MAC, birthday and offset remain zero; InitFromSystem is a later service.
// Throws on invalid date/time ranges before producing a block.
[[nodiscard]] InitialSystemInfo initialize_system_info(RtcSnapshot snapshot);
} // namespace hg
