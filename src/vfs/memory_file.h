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

    void DebugFmt(fmt::Writer& w) const override;

    Result<size_t> Read(u64 offset, size_t length, u8* buffer) override;
    Result<size_t> Write(u64 offset, size_t length, const u8* buffer) override;
    Result<u64> GetSize() override;
    Result<> SetSize(u64 size) override;
    Result<> Close() override;
    Result<> Flush() override;

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
