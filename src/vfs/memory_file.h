// Copyright 2017 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#pragma once

#include <limits>
#include "vfs/file.h"

namespace Vfs {

class MemoryFile : public File {
public:
    explicit MemoryFile(std::vector<u8> data);

    std::string DebugStr() const override;
    ResultVal<size_t> Read(u64 offset, size_t length, u8* buffer) const override;
    ResultVal<size_t> Write(u64 offset, size_t length, const u8* buffer) override;
    ResultVal<u64> GetSize() const override;
    ResultCode SetSize(u64 size) override;
    ResultCode Flush() override;

    const std::vector<u8>& GetData() const {
        return data;
    }

    std::vector<u8>& GetData() {
        return data;
    }

private:
    std::vector<u8> data;
};

} // namespace Vfs
