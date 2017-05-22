// Copyright 2017 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#pragma once

#include <limits>
#include "vfs/file.h"

namespace Vfs {

class SubrangeFile : public File {
public:
    static constexpr u64 UNBOUNDED_SIZE = std::numeric_limits<u64>::max();

    SubrangeFile(std::shared_ptr<File> base_file, u64 offset, u64 size);

    std::string DebugStr() const override;
    ResultVal<size_t> Read(u64 offset, size_t length, u8* buffer) const override;
    ResultVal<size_t> Write(u64 offset, size_t length, const u8* buffer) override;
    ResultVal<u64> GetSize() const override;
    ResultCode SetSize(u64 size) override;
    ResultCode Flush() override;

private:
    std::shared_ptr<File> base_file;
    u64 file_offset;
    u64 file_size;
};

} // namespace Vfs
