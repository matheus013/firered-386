#include "test.hpp"
#include "hg/crc_block_init.hpp"

#include <algorithm>
#include <limits>
#include <stdexcept>

namespace {
template<class F> bool rejects(F&& action) {
    try { action(); return false; } catch (const std::runtime_error&) { return true; }
}
}

TEST_CASE("crc block init: zero blocks get real CRCs and preserve trailer tails") {
    for (const auto& [id, crc] : {std::pair{23u,0xd71bu}, {26u,0x213fu}}) {
        hg::SaveContext context;
        auto block = context.view(id);
        std::fill(block.begin(), block.end(), std::byte{0xa5});
        const auto result = hg::initialize_soulsilver_usa_crc_block(context, id);
        CHECK_EQ(result.updates.size(), 1u);
        CHECK_EQ(result.updates[0].id, id); CHECK_EQ(result.updates[0].crc, crc);
        CHECK(std::all_of(context.raw_view(id).begin(), context.raw_view(id).end(),
            [](auto byte) { return byte == std::byte{0}; }));
        CHECK(block[block.size()-2] == std::byte{0xa5});
        CHECK(block[block.size()-1] == std::byte{0xa5});
    }
}

TEST_CASE("crc block init: rankings fills all names and performs 157 ordered updates") {
    hg::SaveContext context;
    const auto result = hg::initialize_soulsilver_usa_crc_block(context, 24);
    CHECK_EQ(result.updates.size(), 157u);
    CHECK_EQ(result.updates.back().crc, 0x6c5au);
    const auto payload = context.raw_view(24);
    for (unsigned entry = 0; entry < 156; ++entry) {
        for (unsigned byte = 0; byte < 24; ++byte)
            CHECK(payload[entry*24+byte] == (byte < 8 ? std::byte{0} : std::byte{0xff}));
        CHECK_EQ(result.updates[entry].id, 24u);
    }
    CHECK_EQ(result.updates[155].crc, result.updates[156].crc);
    CHECK(result.updates.front().crc != result.updates.back().crc);
}

TEST_CASE("crc block init: Mystery Gift preserves existing payload instead of clearing it") {
    hg::SaveContext context;
    std::fill(context.view(27).begin(), context.view(27).end(), std::byte{0xa5});
    const auto result = hg::initialize_soulsilver_usa_crc_block(context, 27);
    CHECK_EQ(result.updates.size(), 1u);
    CHECK(std::all_of(context.raw_view(27).begin(), context.raw_view(27).end(),
        [](auto byte) { return byte == std::byte{0xa5}; }));
    CHECK(context.view(27).back() == std::byte{0xa5});
    CHECK_EQ(result.updates[0].crc, context.update_crc(27));
    hg::SaveContext zero_context;
    const auto zero = hg::initialize_soulsilver_usa_crc_block(zero_context, 27);
    CHECK_EQ(zero.updates[0].crc, 0x28e2u);
}

TEST_CASE("crc block init: pinned English greeting updates twice and Wii notifications enable") {
    hg::SaveContext context;
    const auto easy = hg::initialize_soulsilver_usa_crc_block(context, 30);
    CHECK_EQ(easy.updates.size(), 2u);
    CHECK_EQ(easy.updates[0].id, 30u); CHECK_EQ(easy.updates[1].id, 30u);
    CHECK_EQ(easy.updates[0].crc, easy.updates[1].crc);
    CHECK(context.raw_view(30)[0] == std::byte{2});
    for (std::size_t byte = 1; byte < 8; ++byte) CHECK(context.raw_view(30)[byte] == std::byte{0});
    const auto wii = hg::initialize_soulsilver_usa_crc_block(context, 31);
    CHECK_EQ(wii.updates.size(), 1u); CHECK_EQ(wii.updates[0].crc, 0xd5afu);
    const auto payload = context.raw_view(31);
    for (std::size_t byte = 0; byte < payload.size(); ++byte)
        CHECK(payload[byte] == (byte == 0x34 ? std::byte{1} : std::byte{0}));
}

TEST_CASE("crc block init: only the bound block mutates and unsupported IDs preserve context") {
    hg::SaveContext context;
    for (const auto id : {23u,24u,26u,27u,30u,31u}) {
        std::fill(context.storage().begin(), context.storage().end(), std::byte{0x3c});
        const auto& layout = context.layout(id);
        const auto result = hg::initialize_soulsilver_usa_crc_block(context, id);
        CHECK(!result.updates.empty());
        for (std::size_t at = 0; at < context.storage().size(); ++at)
            if (at < layout.offset || at >= layout.offset + layout.stored_size)
                CHECK(context.storage()[at] == std::byte{0x3c});
    }
    std::fill(context.storage().begin(), context.storage().end(), std::byte{0x5a});
    for (const auto id : {0u,22u,25u,28u,29u,32u,41u,42u,std::numeric_limits<unsigned>::max()}) {
        CHECK(rejects([&] { (void)hg::initialize_soulsilver_usa_crc_block(context, id); }));
        CHECK(std::all_of(context.storage().begin(), context.storage().end(),
            [](auto byte) { return byte == std::byte{0x5a}; }));
    }
}
