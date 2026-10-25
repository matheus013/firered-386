// Private compatibility probe; links to a separately supplied libntr checkout.
#include "hg/system_info_init.hpp"
#include <chrono>
#include <cstdint>
#include <cstdio>
extern "C" std::int64_t hg_libntr_seconds(unsigned,unsigned,unsigned,unsigned,unsigned,unsigned,unsigned);
int main() {
    using namespace std::chrono;
    unsigned count=0;
    for (auto day=sys_days{year{2000}/January/1}; day<sys_days{year{2100}/January/1}; day+=days{1}) {
        const year_month_day date{day};
        const hg::RtcSnapshot snapshot{unsigned(int(date.year())-2000),unsigned(date.month()),
            unsigned(date.day()),unsigned(weekday{day}.c_encoding()),23,59,59};
        const auto block=hg::initialize_system_info(snapshot);
        std::uint64_t expected=0;
        for(unsigned i=0;i<8;++i) expected|=std::uint64_t(std::to_integer<unsigned>(block[52+i]))<<(8*i);
        const auto actual=hg_libntr_seconds(snapshot.year_since_2000,snapshot.month,snapshot.day,
            snapshot.weekday,snapshot.hour,snapshot.minute,snapshot.second);
        if(actual<0 || std::uint64_t(actual)!=expected) return 1;
        ++count;
    }
    std::printf("libntr RTC compatibility passed: %u calendar dates\n",count);
}
