#include "test.hpp"
#include "hg/save_context.hpp"

#include <algorithm>
#include <limits>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

namespace {
static_assert(!std::is_copy_constructible_v<hg::SaveContext>);
static_assert(!std::is_move_constructible_v<hg::SaveContext>);
static_assert(!std::is_copy_assignable_v<hg::SaveContext>);
static_assert(!std::is_move_assignable_v<hg::SaveContext>);
template<class F> bool rejects(F&& action) {
    try { action(); return false; } catch (const std::runtime_error&) { return true; }
}
}

TEST_CASE("save context: fixed DS layouts bind 42 non-overlapping stored blocks") {
    hg::SaveContext context;
    CHECK_EQ(hg::SaveContext::layouts().size(), 42u);
    std::size_t previous_end = 0;
    for (const auto& block : hg::SaveContext::layouts()) {
        CHECK_EQ(block.slot, block.id == 41 ? 1u : 0u);
        CHECK_EQ(block.aligned_size, (block.raw_size + 3) & ~std::size_t{3});
        CHECK_EQ(block.stored_size, block.aligned_size + 4);
        CHECK(block.offset >= previous_end);
        CHECK(block.offset + block.stored_size <= context.storage().size());
        CHECK(context.view(block.id).data() == context.storage().data() + block.offset);
        CHECK_EQ(context.view(block.id).size(), block.stored_size);
        CHECK_EQ(context.raw_view(block.id).size(), block.raw_size);
        previous_end = block.offset + block.stored_size;
    }
    CHECK_EQ(context.layout(20).raw_size, 14u);
    CHECK_EQ(context.layout(20).aligned_size, 16u);
    CHECK_EQ(context.layout(40).offset + context.layout(40).stored_size, 0xf618u);
    CHECK_EQ(context.layout(41).offset, 0xf700u);
    CHECK_EQ(context.layout(41).raw_size, 0x122fcu);
    CHECK_EQ(previous_end, 0x21a00u);
}

TEST_CASE("save context: construction is deterministic zero without generating CRCs") {
    hg::SaveContext context;
    CHECK_EQ(context.storage().size(), 0x23000u);
    CHECK(std::all_of(context.storage().begin(), context.storage().end(),
        [](auto byte) { return byte == std::byte{0}; }));
    const auto& constant = std::as_const(context);
    CHECK(constant.storage().data() == context.storage().data());
    CHECK(constant.view(27).data() == context.view(27).data());
    CHECK(constant.raw_view(27).data() == context.raw_view(27).data());
    CHECK_EQ(constant.raw_view(27).size(), 0x1680u);
}

TEST_CASE("save context: update writes only the two CRC bytes and preserves trailer tail") {
    hg::SaveContext context;
    std::fill(context.storage().begin(), context.storage().end(), std::byte{0x5a});
    std::fill(context.raw_view(23).begin(), context.raw_view(23).end(), std::byte{0});
    const auto before = std::vector(context.storage().begin(), context.storage().end());
    const auto crc_at = context.layout(23).offset + context.layout(23).aligned_size;
    CHECK_EQ(context.update_crc(23), 0xd71bu);
    CHECK(context.storage()[crc_at] == std::byte{0x1b});
    CHECK(context.storage()[crc_at + 1] == std::byte{0xd7});
    for (std::size_t at = 0; at < before.size(); ++at)
        if (at != crc_at && at != crc_at + 1) CHECK(context.storage()[at] == before[at]);
    CHECK_EQ(context.update_crc(23), 0xd71bu);
}

TEST_CASE("save context: alignment padding participates in CRC but is never cleared") {
    hg::SaveContext context;
    auto block = context.view(20);
    CHECK_EQ(context.update_crc(20), 0x6a0au);
    block[14] = std::byte{0xa5}; block[15] = std::byte{0x5a};
    block[18] = std::byte{0x12}; block[19] = std::byte{0x34};
    CHECK_EQ(context.update_crc(20), 0x733eu);
    CHECK(block[14] == std::byte{0xa5}); CHECK(block[15] == std::byte{0x5a});
    CHECK(block[16] == std::byte{0x3e}); CHECK(block[17] == std::byte{0x73});
    CHECK(block[18] == std::byte{0x12}); CHECK(block[19] == std::byte{0x34});
    CHECK_EQ(context.raw_view(20).size(), 14u);
}

TEST_CASE("save context: every binding stays stable across all CRC updates and raw writes") {
    hg::SaveContext context;
    std::array<std::byte*, hg::SaveContext::BlockCount> addresses{};
    for (std::size_t id = 0; id < addresses.size(); ++id) addresses[id] = context.view(id).data();
    const auto base = context.storage().data();
    // These are bank footer/gap bytes, not any block's own CRC trailer.
    context.storage()[0xf618] = std::byte{0x77};
    context.storage()[0xf6ff] = std::byte{0x88};
    context.storage()[0x21a00] = std::byte{0x99};
    for (std::size_t id = 0; id < addresses.size(); ++id) {
        context.raw_view(id).front() = std::byte(id);
        context.update_crc(id);
    }
    CHECK(context.storage().data() == base);
    for (std::size_t id = 0; id < addresses.size(); ++id) {
        CHECK(context.view(id).data() == addresses[id]);
        CHECK(std::as_const(context).raw_view(id).data() == addresses[id]);
        CHECK(addresses[id][0] == std::byte(id));
    }
    CHECK(context.storage()[0xf618] == std::byte{0x77});
    CHECK(context.storage()[0xf6ff] == std::byte{0x88});
    CHECK(context.storage()[0x21a00] == std::byte{0x99});
}

TEST_CASE("save context: invalid IDs reject before mutation for mutable and const views") {
    hg::SaveContext context;
    context.raw_view(0)[0] = std::byte{0xa5};
    const auto before = std::vector(context.storage().begin(), context.storage().end());
    for (const auto id : {std::size_t{42}, std::size_t{43}, std::size_t{1000000},
             std::numeric_limits<std::size_t>::max()}) {
        CHECK(rejects([&] { context.layout(id); }));
        CHECK(rejects([&] { context.view(id); }));
        CHECK(rejects([&] { context.raw_view(id); }));
        CHECK(rejects([&] { std::as_const(context).view(id); }));
        CHECK(rejects([&] { std::as_const(context).raw_view(id); }));
        CHECK(rejects([&] { context.update_crc(id); }));
        CHECK(std::equal(before.begin(), before.end(), context.storage().begin()));
    }
}

TEST_CASE("save context: independent owners do not share payloads or CRC trailers") {
    hg::SaveContext first, second;
    CHECK(first.view(30).data() != second.view(30).data());
    first.raw_view(30)[0] = std::byte{1};
    first.update_crc(30);
    CHECK(std::all_of(second.storage().begin(), second.storage().end(),
        [](auto byte) { return byte == std::byte{0}; }));
    CHECK(first.raw_view(30)[0] == std::byte{1});
    CHECK(second.raw_view(30)[0] == std::byte{0});
}
