#include "host_files.hpp"
#include "hg/resource_store.hpp"
#include "nitro/fs/file.h"
#include "heap.h"

#include <cstdlib>
#include <cstdio>
#include <map>
#include <memory>
#include <set>
#include <stdexcept>

namespace {
struct Handle { hg::ResourceFile file; std::uint64_t cursor{}; };
std::unique_ptr<hg::ResourceStore> store;
std::map<std::uint64_t, Handle> handles;
std::set<void*> allocations;
std::uint64_t next_token = 1;
[[noreturn]] void fail(const char* message) {
    std::fprintf(stderr, "Host filesystem probe failed: %s\n", message);
    std::abort();
}
}

void hg_host_mount(const std::filesystem::path& root) {
    if (!handles.empty() || !allocations.empty()) throw std::logic_error("Host resources still in use");
    store = std::make_unique<hg::ResourceStore>(root);
}
std::size_t hg_host_open_files() { return handles.size(); }
std::size_t hg_host_allocations() { return allocations.size(); }

extern "C" {
void FS_InitFile(FSFile* file) { file->token = 0; }
BOOL FS_OpenFile(FSFile* file, const char* path) {
    try {
        if (!store || file->token) fail("Invalid open state");
        const auto token = next_token++;
        handles.emplace(token, Handle{store->open(path), 0});
        file->token = token;
        return TRUE;
    } catch (const std::exception& e) { fail(e.what()); }
}
BOOL FS_CloseFile(FSFile* file) {
    if (handles.erase(file->token) != 1) fail("Invalid close");
    file->token = 0;
    return TRUE;
}
BOOL FS_SeekFile(FSFile* file, s32 offset, FSSeekFileMode mode) {
    auto it = handles.find(file->token);
    if (it == handles.end()) fail("Invalid seek handle");
    auto& h = it->second;
    std::int64_t base{};
    if (mode == FS_SEEK_CUR) base = static_cast<std::int64_t>(h.cursor);
    else if (mode == FS_SEEK_END) base = static_cast<std::int64_t>(h.file.size());
    else if (mode != FS_SEEK_SET) fail("Invalid seek mode");
    const auto target = base + offset;
    if (target < 0 || static_cast<std::uint64_t>(target) > h.file.size()) fail("Seek outside file");
    h.cursor = static_cast<std::uint64_t>(target);
    return TRUE;
}
s32 FS_ReadFile(FSFile* file, void* destination, s32 size) {
    try {
        auto it = handles.find(file->token);
        if (it == handles.end() || size < 0) fail("Invalid read");
        auto& h = it->second;
        h.file.read(h.cursor, {static_cast<std::byte*>(destination), static_cast<std::size_t>(size)});
        h.cursor += static_cast<std::uint32_t>(size);
        return size;
    } catch (const std::exception& e) { fail(e.what()); }
}
void* Heap_Alloc(enum HeapID, u32 size) {
    auto* pointer = std::malloc(size);
    if (!pointer) fail("Allocation failed");
    allocations.insert(pointer);
    return pointer;
}
void* Heap_AllocAtEnd(enum HeapID heap, u32 size) { return Heap_Alloc(heap, size); }
void Heap_Free(void* pointer) {
    if (!pointer || allocations.erase(pointer) != 1) fail("Invalid allocation release");
    std::free(pointer);
}
}
