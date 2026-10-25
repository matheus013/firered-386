#include "hg/system_info_init.hpp"
#include <chrono>
#include <stdexcept>

namespace hg {
InitialSystemInfo initialize_system_info(RtcSnapshot snapshot) {
    if (snapshot.year_since_2000 > 99 || snapshot.month < 1 || snapshot.month > 12 ||
        snapshot.day < 1 || snapshot.day > 31 || snapshot.weekday > 6 ||
        snapshot.hour > 23 || snapshot.minute > 59 || snapshot.second > 59)
        throw std::runtime_error("RTC snapshot is outside the supported DS calendar");
    using namespace std::chrono;
    const year_month_day date{year{2000 + static_cast<int>(snapshot.year_since_2000)},
        month{snapshot.month}, day{snapshot.day}};
    if (!date.ok()) throw std::runtime_error("RTC snapshot contains an invalid calendar date");
    const auto days = static_cast<std::uint32_t>((sys_days{date} - sys_days{year{2000}/January/1}).count());
    const std::uint64_t seconds = static_cast<std::uint64_t>(days) * 86400u +
        snapshot.hour * 3600u + snapshot.minute * 60u + snapshot.second;
    InitialSystemInfo bytes{};
    const auto write = [&](unsigned offset, std::uint64_t value, unsigned size = 4) {
        for (unsigned i = 0; i < size; ++i) bytes[offset+i] = std::byte((value >> (8*i)) & 255);
    };
    write(16,1);
    write(20,snapshot.year_since_2000); write(24,snapshot.month);
    write(28,snapshot.day); write(32,snapshot.weekday);
    write(36,snapshot.hour); write(40,snapshot.minute); write(44,snapshot.second);
    write(48,days); write(52,seconds,8);
    return bytes;
}
} // namespace hg
