#include "test.hpp"
#include "hg/random.hpp"

#include <array>

TEST_CASE("random: LCRNG known seeds advance before returning the high word") {
    struct Example { std::uint32_t seed; std::array<std::uint16_t,4> words; };
    const std::array examples{
        Example{0, {0x0000, 0xe97e, 0x5271, 0x31b0}},
        Example{1, {0x41c6, 0xac21, 0xd2ee, 0x1fb7}},
        Example{0xffffffff, {0xbe3a, 0x26db, 0xd1f3, 0x43aa}},
        Example{0x12345678, {0x0b71, 0x84ea, 0xd98a, 0xf4e0}},
    };
    for (const auto& example : examples) {
        hg::Lcrng random(example.seed);
        CHECK_EQ(random.state(), example.seed);
        for (const auto expected : example.words) CHECK_EQ(random.next16(), expected);
        random.reseed(example.seed);
        for (const auto expected : example.words) CHECK_EQ(random.next16(), expected);
    }
    hg::Lcrng zero(0);
    zero.next16();
    CHECK_EQ(zero.state(), 24691u);
}

TEST_CASE("random: MT19937 reproduces the standard 5489 sequence and seed zero") {
    hg::Mt19937 standard(5489);
    for (const auto expected : std::array<std::uint32_t,6>{3499211612u,581869302u,
            3890346734u,3586334585u,545404204u,4161255391u})
        CHECK_EQ(standard.next32(), expected);
    hg::Mt19937 zero(0);
    for (const auto expected : std::array<std::uint32_t,3>{2357136044u,2546248239u,3071714933u})
        CHECK_EQ(zero.next32(), expected);
}

TEST_CASE("random: independent instances survive interleaving and repeated MT twists") {
    hg::Mt19937 first(0x12345678), second(0xffffffff);
    hg::Mt19937 first_reference(0x12345678), second_reference(0xffffffff);
    hg::Lcrng lc_first(1), lc_second(0x80000000);
    hg::Lcrng lc_first_reference(1), lc_second_reference(0x80000000);
    for (unsigned i = 0; i < 2500; ++i) {
        CHECK_EQ(first.next32(), first_reference.next32());
        CHECK_EQ(lc_second.next16(), lc_second_reference.next16());
        CHECK_EQ(second.next32(), second_reference.next32());
        CHECK_EQ(lc_first.next16(), lc_first_reference.next16());
    }
    CHECK_EQ(lc_first.state(), lc_first_reference.state());
    CHECK_EQ(lc_second.state(), lc_second_reference.state());
}

TEST_CASE("random: copy snapshots and reseeding discard prior stream position") {
    hg::Mt19937 original(0xdeadbeef);
    hg::Lcrng lc_original(0xdeadbeef);
    for (unsigned i = 0; i < 701; ++i) { original.next32(); lc_original.next16(); }
    auto snapshot = original;
    auto lc_snapshot = lc_original;
    for (unsigned i = 0; i < 1300; ++i) {
        CHECK_EQ(original.next32(), snapshot.next32());
        CHECK_EQ(lc_original.next16(), lc_snapshot.next16());
    }
    original.reseed(5489);
    lc_original.reseed(0);
    hg::Mt19937 reset_reference(5489);
    hg::Lcrng lc_reset_reference(0);
    for (unsigned i = 0; i < 1300; ++i) {
        CHECK_EQ(original.next32(), reset_reference.next32());
        CHECK_EQ(lc_original.next16(), lc_reset_reference.next16());
    }
}
