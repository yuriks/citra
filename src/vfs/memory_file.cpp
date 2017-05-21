// Copyright 2017 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include <algorithm>
#include <fmt/format.h>
#include "vfs/memory_file.h"
#include "vfs/vfs.h"

namespace Vfs {

MemoryFile::MemoryFile(std::vector<u8> data) : data(std::move(data)) {}

std::string MemoryFile::DebugStr() const {
    return fmt::format("MemoryFile{{size={}}}", data.size());
}

ResultVal<size_t> MemoryFile::Read(u64 offset, size_t length, u8* buffer) const {
    if (offset >= data.size()) {
        // Attempt to read past EOF
        return MakeResult<size_t>(0);
    }
    size_t read_size = std::min(length, data.size() - offset);
    std::copy_n(data.begin() + offset, read_size, buffer);
    return MakeResult<size_t>(read_size);
}

ResultVal<size_t> MemoryFile::Write(u64 offset, size_t length, const u8* buffer) {
    if (offset >= data.size()) {
        // Attempt to write past EOF
        return MakeResult<size_t>(0);
    }
    size_t write_size = std::min(length, data.size() - offset);
    std::copy_n(buffer, write_size, data.begin() + offset);
    return MakeResult<size_t>(write_size);
}

ResultVal<u64> MemoryFile::GetSize() const {
    return MakeResult<u64>(data.size());
}

ResultCode MemoryFile::SetSize(u64 size) {
    data.resize(size);
    return RESULT_SUCCESS;
}

ResultCode MemoryFile::Flush() {
    return RESULT_SUCCESS;
}

} // namespace Vfs
