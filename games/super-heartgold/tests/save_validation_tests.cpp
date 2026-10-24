#include "test.hpp"
#include "hg/save_validation.hpp"

#include <algorithm>
#include <limits>
#include <string_view>
#include <utility>

namespace {
using Bytes = std::vector<std::byte>;
void put16(Bytes& bytes, std::size_t at, std::uint16_t value) {
    bytes[at] = std::byte(value & 255);
    bytes[at + 1] = std::byte(value >> 8);
}
void put32(Bytes& bytes, std::size_t at, std::uint32_t value) {
    put16(bytes, at, static_cast<std::uint16_t>(value));
    put16(bytes, at + 2, static_cast<std::uint16_t>(value >> 16));
}
// Independent table implementation for fixture CRCs; production uses bitwise.
std::uint16_t fixture_crc(std::span<const std::byte> bytes) {
    std::array<std::uint16_t, 256> table{};
    for (unsigned i = 0; i < table.size(); ++i) {
        unsigned value = i << 8;
        for (unsigned bit = 0; bit < 8; ++bit)
            value = ((value << 1) ^ ((value & 0x8000) ? 0x1021 : 0)) & 0xffff;
        table[i] = static_cast<std::uint16_t>(value);
    }
    std::uint16_t crc = 0xffff;
    for (auto byte : bytes)
        crc = static_cast<std::uint16_t>((crc << 8) ^ table[(crc >> 8) ^ std::to_integer<unsigned>(byte)]);
    return crc;
}
std::size_t footer_at(unsigned bank, unsigned chunk) {
    const auto spec = hg::PrimarySaveChunks[chunk];
    return bank * hg::SaveImage::BankSize + spec.offset + spec.size - hg::SaveImage::FooterSize;
}
void write_chunk(Bytes& bytes, unsigned bank, unsigned chunk, std::uint32_t count) {
    const auto spec = hg::PrimarySaveChunks[chunk];
    const auto start = bank * hg::SaveImage::BankSize + spec.offset;
    const auto end = footer_at(bank, chunk);
    for (auto offset = start; offset < end; ++offset) bytes[offset] = std::byte((offset * 13 + bank * 7) % 251);
    put32(bytes, end, count);
    put32(bytes, end + 4, static_cast<std::uint32_t>(spec.size));
    put32(bytes, end + 8, 0x20060623);
    put16(bytes, end + 12, static_cast<std::uint16_t>(chunk));
    put16(bytes, end + 14, fixture_crc(std::span<const std::byte>(bytes).subspan(start, end - start)));
}
Bytes two_banks(std::uint32_t first = 10, std::uint32_t second = 11) {
    Bytes bytes(hg::SaveImage::Size, std::byte{0xff});
    for (unsigned chunk = 0; chunk < 2; ++chunk) {
        write_chunk(bytes, 0, chunk, first);
        write_chunk(bytes, 1, chunk, second);
    }
    return bytes;
}
hg::PrimarySaveValidation validate(const Bytes& bytes) {
    return hg::validate_primary_save(hg::SaveImage(bytes));
}
}

TEST_CASE("save_validation: CRC matches vectors verified against real SDK execution") {
    CHECK_EQ(hg::save_crc16({}), 0xffffu);
    const std::string_view digits = "123456789";
    CHECK_EQ(hg::save_crc16(std::as_bytes(std::span(digits.data(), digits.size()))), 0x29b1u);
    const std::array zero{std::byte{0}}, ff{std::byte{0xff}};
    CHECK_EQ(hg::save_crc16(zero), 0xe1f0u);
    CHECK_EQ(hg::save_crc16(ff), 0xff00u);
    Bytes bytes(256);
    for (std::size_t i = 0; i < bytes.size(); ++i) bytes[i] = std::byte(i);
    CHECK_EQ(hg::save_crc16(bytes), 0x3fbdu);
}

TEST_CASE("save_validation: selects newer complete bank and preserves bytes") {
    const auto bytes = two_banks();
    const hg::SaveImage image(bytes);
    const auto result = hg::validate_primary_save(image);
    CHECK(result.reference_selection.status == hg::SaveSelectionStatus::Good);
    CHECK(result.readable_primary_bank == 1u);
    for (const auto& bank : result.chunks) for (const auto& chunk : bank) CHECK(chunk.valid());
    const auto unchanged = image.bank_range(0, 0, hg::SaveImage::BankSize);
    CHECK(std::equal(unchanged.begin(), unchanged.end(), bytes.begin()));
}

TEST_CASE("save_validation: rejects every footer identity mismatch and payload corruption") {
    using Error = hg::SaveChunkError;
    for (unsigned bank = 0; bank < 2; ++bank) for (unsigned chunk = 0; chunk < 2; ++chunk) {
        const auto footer = footer_at(bank, chunk);
        for (const auto& [relative, error] : std::array{
                 std::pair{4u, Error::Size}, std::pair{8u, Error::Magic},
                 std::pair{12u, Error::Slot}, std::pair{14u, Error::Crc}}) {
            auto bytes = two_banks();
            bytes[footer + relative] ^= std::byte{1};
            CHECK(validate(bytes).chunks[bank][chunk].error == error);
        }
        auto bytes = two_banks();
        const auto start = bank * hg::SaveImage::BankSize + hg::PrimarySaveChunks[chunk].offset;
        bytes[start] ^= std::byte{1};
        CHECK(validate(bytes).chunks[bank][chunk].error == Error::Crc);
        bytes[start] ^= std::byte{1};
        bytes[footer - 1] ^= std::byte{0x80};
        CHECK(validate(bytes).chunks[bank][chunk].error == Error::Crc);
    }
}

TEST_CASE("save_validation: interrupted write recovers complete older bank") {
    auto bytes = two_banks();
    // Footer counters are not themselves CRC-covered in this original format.
    put32(bytes, footer_at(1, 1), 10);
    const auto result = validate(bytes);
    CHECK(result.reference_selection.status == hg::SaveSelectionStatus::Recovered);
    CHECK(result.readable_primary_bank == 0u);
    bytes[hg::SaveImage::BankSize] ^= std::byte{1};
    CHECK(validate(bytes).readable_primary_bank == 0u);
}

TEST_CASE("save_validation: rejects original zero dummy count edge on counter wrap") {
    auto bytes = two_banks(0, std::numeric_limits<std::uint32_t>::max());
    bytes[hg::PrimarySaveChunks[1].offset] ^= std::byte{1};
    const auto result = validate(bytes);
    CHECK(result.reference_selection.status == hg::SaveSelectionStatus::Good);
    CHECK(result.reference_selection.bank == 0u);
    CHECK(!result.reference_selection.selected_chunks_valid);
    CHECK(!result.readable_primary_bank);
}

TEST_CASE("save_validation: blank and malformed images do not become readable game saves") {
    for (auto value : {std::byte{0}, std::byte{0xff}, std::byte{0x42}}) {
        CHECK(!validate(Bytes(hg::SaveImage::Size, value)).readable_primary_bank);
    }
    auto bytes = two_banks();
    // Extra records are outside this primary-only validator's scope.
    bytes[0x30000] ^= std::byte{1};
    CHECK(validate(bytes).readable_primary_bank == 1u);
    // Valid but mismatched sole chunks must not trigger the original assertion.
    bytes = Bytes(hg::SaveImage::Size, std::byte{0xff});
    write_chunk(bytes, 0, 0, 1);
    write_chunk(bytes, 0, 1, 2);
    CHECK(validate(bytes).reference_selection.status == hg::SaveSelectionStatus::Corrupt);
    CHECK(!validate(bytes).readable_primary_bank);
}
