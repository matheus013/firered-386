#include "test.hpp"
#include "hg/resource_store.hpp"
#include <array>
#include <chrono>
#include <filesystem>
#include <fstream>

namespace {
struct Fixture {
    std::filesystem::path root = std::filesystem::temp_directory_path() /
        ("hg-resource-" + std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));
    Fixture() { std::filesystem::create_directories(root / "data"); std::ofstream(root / "data/test", std::ios::binary) << "abcde"; }
    ~Fixture() { std::error_code ec; std::filesystem::remove_all(root, ec); }
};
template<class F> bool throws(F&& fn) { try { fn(); return false; } catch (const std::exception&) { return true; } }
}

TEST_CASE("resource: read ranges and repeat after rejected range") {
    Fixture fixture;
    auto file = hg::ResourceStore(fixture.root).open("data/test");
    CHECK_EQ(file.size(), 5u);
    std::array<std::byte, 2> bytes{};
    file.read(1, bytes);
    CHECK(bytes[0] == std::byte{'b'} && bytes[1] == std::byte{'c'});
    CHECK(throws([&] { file.read(4, bytes); }));
    CHECK(throws([&] { file.read(UINT64_MAX, bytes); }));
    file.read(5, {});
    file.read(3, bytes);
    CHECK(bytes[0] == std::byte{'d'} && bytes[1] == std::byte{'e'});
}

TEST_CASE("resource: reject traversal absolute missing and directory paths") {
    Fixture fixture;
    hg::ResourceStore store(fixture.root);
    for (const auto* name : {"../test", "/data/test", "C:/data/test", "data\\test", "data/./test", "missing", "data"})
        CHECK(throws([&] { auto unused = store.open(name); }));
    const std::string embedded_nul("data/test\0suffix", 16);
    CHECK(throws([&] { auto unused = store.open(embedded_nul); }));
}

TEST_CASE("resource: reject symlink escaping mounted directory when supported") {
    Fixture fixture;
    Fixture outside;
    std::error_code error;
    std::filesystem::create_symlink(outside.root / "data/test", fixture.root / "escape", error);
    if (!error) {
        hg::ResourceStore store(fixture.root);
        CHECK(throws([&] { auto unused = store.open("escape"); }));
    }
}
