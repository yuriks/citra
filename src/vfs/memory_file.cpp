// Copyright 2017 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include <algorithm>
#include <fmt/format.h>
#include "vfs/memory_file.h"
#include "vfs/vfs.h"

namespace Vfs {

MemoryFile::MemoryFile(std::vector<u8> data) : data(std::move(data)) {}

void MemoryFile::DebugFmt(fmt::Writer& w) const {
    w.write("MemoryFile{{size={}}}", data.size());
}

Result<size_t> MemoryFile::Read(u64 offset, size_t length, u8* buffer) {
    if (offset >= data.size()) {
        // Attempt to read past EOF
        return 0;
    }
    size_t read_size = std::min(length, data.size() - offset);
    std::copy_n(data.begin() + offset, read_size, buffer);
    return read_size;
}

Result<size_t> MemoryFile::Write(u64 offset, size_t length, const u8* buffer) {
    if (offset >= data.size()) {
        // Attempt to write past EOF
        return 0;
    }
    size_t write_size = std::min(length, data.size() - offset);
    std::copy_n(buffer, write_size, data.begin() + offset);
    return write_size;
}

Result<u64> MemoryFile::GetSize() {
    return data.size();
}

Result<> MemoryFile::SetSize(u64 size) {
    data.resize(size);
    return Ok;
}

Result<> MemoryFile::Close() {
    return Ok;
}

Result<> MemoryFile::Flush() {
    return Ok;
}

} // namespace Vfs
