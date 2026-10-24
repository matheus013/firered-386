#include "hg/resource_store.hpp"
#include "host_files.hpp"
extern "C" {
#include "filesystem.h"
}
#include <algorithm>
#include <array>
#include <chrono>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace {
void require(bool condition, const char* why) { if (!condition) throw std::runtime_error(why); }
std::uint32_t le32(const std::vector<std::byte>& b, std::size_t p) {
    require(p <= b.size() && b.size()-p >= 4, "Truncated fixture/archive");
    return std::to_integer<std::uint32_t>(b[p]) | (std::to_integer<std::uint32_t>(b[p+1])<<8) |
        (std::to_integer<std::uint32_t>(b[p+2])<<16) | (std::to_integer<std::uint32_t>(b[p+3])<<24);
}
void put(std::vector<std::byte>& bytes, std::size_t p, std::uint32_t value, int count=4) {
    for (int i=0; i<count; ++i) bytes[p+static_cast<std::size_t>(i)] = static_cast<std::byte>((value>>(8*i))&255);
}
std::vector<std::byte> fixture() {
    std::vector<std::byte> b(76);
    std::memcpy(b.data(), "NARC",4); put(b,4,0x0100fffe); put(b,8,76); put(b,12,16,2); put(b,14,3,2);
    std::memcpy(b.data()+16,"BTAF",4); put(b,20,28); put(b,24,2,2);
    put(b,28,0); put(b,32,5); put(b,36,5); put(b,40,12);
    std::memcpy(b.data()+44,"BTNF",4); put(b,48,12);
    std::memcpy(b.data()+56,"GMIF",4); put(b,60,20);
    for (std::size_t i=64;i<b.size();++i) b[i]=static_cast<std::byte>(i-63);
    return b;
}
void verify(const std::filesystem::path& root, unsigned id, const std::string& path) {
    require(id<=static_cast<unsigned>(NARC_application_voltorb_flip),"NARC ID outside pinned table");
    require(path==sNarcFileList[id],"NARC ID/path differs from pinned table");
    hg_host_mount(root);
    auto file=hg::ResourceStore(root).open(path);
    require(file.size() <= 128*1024*1024, "Archive too large for probe");
    std::vector<std::byte> archive(static_cast<std::size_t>(file.size())); file.read(0,archive);
    require(archive.size()>=16 && std::memcmp(archive.data(),"NARC",4)==0, "Not NARC");
    const auto fat=le32(archive,12)&65535;
    const auto names=fat+le32(archive,fat+4);
    const auto image=names+le32(archive,names+4)+8;
    auto* narc=NARC_New(static_cast<NarcId>(id), HG_HOST_TEST_HEAP);
    require(narc!=nullptr,"NARC_New failed");
    const auto count=NARC_GetFileCount(narc);
    require(count==(le32(archive,fat+8)&65535),"Member count differs");
    std::size_t checked_bytes=0;
    for (unsigned i=0; i<count; ++i) {
        const auto start=le32(archive,fat+12+8*i), end=le32(archive,fat+16+8*i);
        require(start<=end && image<=archive.size() && end<=archive.size()-image,"Invalid member bounds");
        require(NARC_GetMemberSize(narc,i)==end-start,"Member size differs");
        std::vector<std::byte> expected(archive.begin()+image+start,archive.begin()+image+end), actual(expected.size());
        if (expected.empty()) continue;
        NARC_ReadWholeMember(narc,i,actual.data());
        require(actual==expected,"Original C whole member differs");
        auto* allocated=NARC_AllocAndReadWholeMember(narc,i,HG_HOST_TEST_HEAP);
        require(std::memcmp(allocated,expected.data(),expected.size())==0,"Allocated member differs"); Heap_Free(allocated);
        const auto slice=std::min<std::size_t>(4,expected.size());
        NARC_ReadFromMember(narc,i,static_cast<u32>(expected.size()-slice),static_cast<u32>(slice),actual.data());
        require(std::equal(actual.begin(),actual.begin()+slice,expected.end()-slice),"Partial read differs");
        u32 absolute{};
        NARC_GetMemberImageStartOffset(narc,i,&absolute);
        require(absolute==image+start,"Absolute member position differs");
        NARC_ReadFromAbsolutePos(narc,absolute,static_cast<u32>(slice),actual.data());
        require(std::equal(actual.begin(),actual.begin()+slice,expected.begin()),"Absolute read differs");
        NARC_ReadFromAbsolutePos(narc,absolute,0,actual.data());
        NARC_ReadFile(narc,static_cast<u32>(expected.size()),actual.data());
        require(actual==expected,"Cursor read differs");
        require(GetNarcMemberSizeByIdPair(static_cast<NarcId>(id),static_cast<s32>(i))==expected.size(),"Id-pair size differs");
        ReadWholeNarcMemberByIdPair(actual.data(),static_cast<NarcId>(id),static_cast<s32>(i));
        require(actual==expected,"Id-pair read differs");
        ReadFromNarcMemberByIdPair(actual.data(),static_cast<NarcId>(id),static_cast<s32>(i),static_cast<u32>(expected.size()-slice),static_cast<u32>(slice));
        require(std::equal(actual.begin(),actual.begin()+slice,expected.end()-slice),"Id-pair slice differs");
        for (const bool at_end : {false,true}) {
            auto* whole=at_end ? AllocAtEndAndReadWholeNarcMemberByIdPair(static_cast<NarcId>(id),static_cast<s32>(i),HG_HOST_TEST_HEAP)
                : AllocAndReadWholeNarcMemberByIdPair(static_cast<NarcId>(id),static_cast<s32>(i),HG_HOST_TEST_HEAP);
            require(std::memcmp(whole,expected.data(),expected.size())==0,"Allocated id-pair member differs"); Heap_Free(whole);
            auto* part=at_end ? AllocAtEndAndReadFromNarcMemberByIdPair(static_cast<NarcId>(id),static_cast<s32>(i),HG_HOST_TEST_HEAP,0,static_cast<u32>(slice))
                : AllocAndReadFromNarcMemberByIdPair(static_cast<NarcId>(id),static_cast<s32>(i),HG_HOST_TEST_HEAP,0,static_cast<u32>(slice));
            require(std::memcmp(part,expected.data(),slice)==0,"Allocated id-pair slice differs"); Heap_Free(part);
        }
        checked_bytes+=expected.size();
    }
    NARC_Delete(narc);
    require(hg_host_open_files()==0,"Open resource leak");
    require(hg_host_allocations()==0,"Heap allocation leak");
    std::cout<<"PASS original C filesystem: archive="<<path<<" members="<<count<<" bytes="<<checked_bytes<<" handles=0 allocations=0\n";
}
}

int main(int argc, char** argv) {
    static_assert(sizeof(void*)==8);
    std::filesystem::path temporary;
    try {
        if (argc==4) { verify(argv[1],static_cast<unsigned>(std::stoul(argv[2])),argv[3]); return 0; }
        require(argc==1,"Usage: hg_reference_filesystem [resource_root narc_id relative_archive]");
        temporary=std::filesystem::temp_directory_path() / ("hg-narc-"+std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));
        std::filesystem::create_directories(temporary/"a/0/0");
        const auto bytes=fixture();
        { std::ofstream out(temporary/"a/0/0/0",std::ios::binary); out.write(reinterpret_cast<const char*>(bytes.data()),static_cast<std::streamsize>(bytes.size())); }
        verify(temporary,0,"a/0/0/0");
        std::filesystem::remove_all(temporary);
        return 0;
    } catch (const std::exception& e) {
        if (!temporary.empty()) { std::error_code ignored; std::filesystem::remove_all(temporary,ignored); }
        std::cerr<<e.what()<<'\n'; return 1;
    }
}
