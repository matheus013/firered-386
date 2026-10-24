#include "test.hpp"
#include "hg/save_image.hpp"

#include <algorithm>
#include <chrono>
#include <fstream>
#include <limits>
#include <utility>

namespace {
template<class F> bool throws(F&& action) {
    try { action(); return false; } catch (const std::exception&) { return true; }
}
struct FileFixture {
    std::filesystem::path directory = std::filesystem::temp_directory_path() /
        ("hg-save-read-" + std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));
    std::filesystem::path path = directory / "synthetic.sav";
    FileFixture() { std::filesystem::create_directory(directory); }
    ~FileFixture() { std::error_code error; std::filesystem::remove_all(directory, error); }
    void write(const std::vector<std::byte>& bytes) {
        std::ofstream output(path, std::ios::binary);
        output.write(reinterpret_cast<const char*>(bytes.data()), static_cast<std::streamsize>(bytes.size()));
    }
};
}

TEST_CASE("save_image: snapshot owns bytes and isolates banks") {
    std::vector<std::byte> bytes(hg::SaveImage::Size, std::byte{0xff});
    bytes[0] = std::byte{0x12};
    bytes[hg::SaveImage::BankSize] = std::byte{0x34};
    hg::SaveImage image(bytes);
    bytes[0] = std::byte{0};
    CHECK(image.bank_range(0, 0, 1)[0] == std::byte{0x12});
    CHECK(image.bank_range(1, 0, 1)[0] == std::byte{0x34});
    CHECK(image.bank_range(1, hg::SaveImage::BankSize, 0).empty());
    CHECK(throws([&] { image.bank_range(2, 0, 0); }));
    CHECK(throws([&] { image.bank_range(0, hg::SaveImage::BankSize, 1); }));
    CHECK(throws([&] { image.bank_range(0, std::numeric_limits<std::size_t>::max(), 1); }));
    CHECK(throws([&] { image.bank_range(0, 1, std::numeric_limits<std::size_t>::max()); }));
}

TEST_CASE("save_image: footer uses little endian fields without host struct layout") {
    std::vector<std::byte> bytes(hg::SaveImage::Size);
    const unsigned values[]{0x78,0x56,0x34,0x12, 0x44,0x33,0x22,0x11,
                            0x23,0x06,0x06,0x20, 0x01,0x00,0xef,0xbe};
    const auto offset = hg::SaveImage::BankSize - hg::SaveImage::FooterSize;
    for (std::size_t i = 0; i < 16; ++i) bytes[hg::SaveImage::BankSize + offset + i] = std::byte(values[i]);
    hg::SaveImage image(bytes);
    const auto footer = image.footer(1, offset);
    CHECK_EQ(footer.count, 0x12345678u);
    CHECK_EQ(footer.size, 0x11223344u);
    CHECK_EQ(footer.magic, 0x20060623u);
    CHECK_EQ(footer.slot, 1u);
    CHECK_EQ(footer.crc, 0xbeefu);
    CHECK(throws([&] { image.footer(1, offset + 1); }));
}

TEST_CASE("save_image: moved snapshots retain ownership without unsafe source reads") {
    hg::SaveImage source(std::vector<std::byte>(hg::SaveImage::Size, std::byte{0x5a}));
    const hg::SaveImage destination(std::move(source));
    CHECK(destination.bank_range(0, 0, 1)[0] == std::byte{0x5a});
    // A moved-from vector is valid but its size is unspecified; either it still
    // owns a complete snapshot or the bounds guard must reject the operation.
    try { CHECK(source.bank_range(1, 0, 1)[0] == std::byte{0x5a}); }
    catch (const std::logic_error&) {}
}

TEST_CASE("save_image: reject truncated padded and emulator container files") {
    FileFixture fixture;
    CHECK(throws([&] { auto image = hg::SaveImage::load(fixture.path); }));
    CHECK(!std::filesystem::exists(fixture.path));
    for (const auto size : {std::size_t{0}, hg::SaveImage::Size - 1, hg::SaveImage::Size + 1}) {
        std::vector<std::byte> bytes(size);
        CHECK(throws([&] { hg::SaveImage image(bytes); }));
        fixture.write(bytes);
        CHECK(throws([&] { auto image = hg::SaveImage::load(fixture.path); }));
        CHECK_EQ(std::filesystem::file_size(fixture.path), size);
    }
}

TEST_CASE("save_image: loading preserves original content and modification time") {
    FileFixture fixture;
    std::vector<std::byte> bytes(hg::SaveImage::Size);
    for (std::size_t i = 0; i < bytes.size(); ++i) bytes[i] = std::byte(i % 251);
    fixture.write(bytes);
    const auto modified = std::filesystem::last_write_time(fixture.path);
    const auto image = hg::SaveImage::load(fixture.path);
    std::ifstream input(fixture.path, std::ios::binary);
    std::vector<std::byte> after(bytes.size());
    input.read(reinterpret_cast<char*>(after.data()), static_cast<std::streamsize>(after.size()));
    CHECK(after == bytes);
    CHECK(std::filesystem::last_write_time(fixture.path) == modified);
    const auto bank = image.bank_range(1, 0, hg::SaveImage::BankSize);
    CHECK(std::equal(bank.begin(), bank.end(), bytes.begin() + hg::SaveImage::BankSize));
}
