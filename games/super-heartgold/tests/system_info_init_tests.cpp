#include "test.hpp"
#include "hg/system_info_init.hpp"
#include <algorithm>
#include <stdexcept>

namespace {
std::uint64_t word(const hg::InitialSystemInfo& bytes, unsigned offset, unsigned size = 4) {
    std::uint64_t result{};
    for (unsigned i=0; i<size; ++i) result |= std::uint64_t(std::to_integer<unsigned>(bytes[offset+i])) << (8*i);
    return result;
}
}
TEST_CASE("system_info_init: Nitro epoch and owner fields are explicit without host identity") {
    const auto bytes = hg::initialize_system_info({0,1,1,6,0,0,0});
    CHECK_EQ(bytes.size(),92u); CHECK_EQ(word(bytes,16),1u);
    CHECK_EQ(word(bytes,48),0u); CHECK_EQ(word(bytes,52,8),0u);
    CHECK(std::all_of(bytes.begin(),bytes.begin()+16,[](auto value) { return value == std::byte{0}; }));
    CHECK(std::all_of(bytes.begin()+60,bytes.end(),[](auto value) { return value == std::byte{0}; }));
}
TEST_CASE("system_info_init: leap day and unsigned seconds match the Nitro calendar") {
    const auto leap = hg::initialize_system_info({0,2,29,2,12,34,56});
    CHECK_EQ(word(leap,48),59u); CHECK_EQ(word(leap,52,8),5142896u);
    const auto last = hg::initialize_system_info({99,12,31,4,23,59,59});
    CHECK_EQ(word(last,48),36524u); CHECK_EQ(word(last,52,8),3155759999ull);
    const auto copied_weekday = hg::initialize_system_info({0,1,1,0,0,0,0});
    CHECK_EQ(word(copied_weekday,32),0u); // Stored verbatim, not recomputed from date.
}
TEST_CASE("system_info_init: invalid dates times and wide month values reject") {
    for (const auto snapshot : {hg::RtcSnapshot{100,1,1,0,0,0,0}, {1,2,29,0,0,0,0},
             {0,257,1,0,0,0,0}, {0,0,1,0,0,0,0}, {0,4,31,0,0,0,0},
             {0,1,1,7,0,0,0}, {0,1,1,0,24,0,0}, {0,1,1,0,0,60,0}, {0,1,1,0,0,0,60}}) {
        bool rejected{};
        try { (void)hg::initialize_system_info(snapshot); } catch (const std::runtime_error&) { rejected=true; }
        CHECK(rejected);
    }
}
