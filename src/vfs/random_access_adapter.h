// Copyright 2017 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#pragma once

#include <cstdint>
#include <string>
#include "common/common_types.h"
#include "common/file_util.h"
#include "vfs/file.h"

namespace Vfs {

class RandomAccessAdapter : public File {
public:
    RandomAccessAdapter(std::unique_ptr<StreamFile> base_file) : base_file(std::move(base_file)) {}

    void DebugFmt(fmt::Writer& w) const override;

    ResultVal<size_t> Read(u64 offset, size_t length, u8* buffer) override;
    ResultVal<size_t> Write(u64 offset, size_t length, const u8* buffer) override;
    ResultVal<u64> GetSize() override;
    ResultCode SetSize(u64 size) override;
    ResultCode Close() override;
    ResultCode Flush() override;

    const StreamFile& GetBaseFile() const {
        return *base_file;
    }

private:
    std::unique_ptr<StreamFile> base_file;
};

} // namespace Vfs
