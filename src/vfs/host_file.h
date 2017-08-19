// Copyright 2017 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#pragma once

#include <cstdint>
#include <cstdio>
#include <string>
#include "common/common_types.h"
#include "common/file_util.h"
#include "vfs/file.h"

namespace Vfs {

class HostFile : public StreamFile {
public:
    static ResultVal<HostFile> Open(std::string path, OpenMode mode);
    HostFile(FileUtil::IOFile&& file, OpenMode mode, std::string debug_path);
    HostFile(FileUtil::IOFile&& file, OpenMode mode);
    ~HostFile();

    HostFile(HostFile&&) = default;
    HostFile& operator=(HostFile&&) = default;

    std::string DebugStr() const override;
    ResultVal<size_t> Read(size_t length, u8* buffer) override;
    ResultVal<size_t> Write(size_t length, const u8* buffer) override;
    ResultCode Seek(u64 offset_from_beginning) override;
    ResultVal<u64> Tell() override;
    ResultVal<u64> GetSize() const override;
    ResultCode SetSize(u64 size) override;
    ResultCode Flush() override;

    const FileUtil::IOFile& GetHostFile() const {
        return file;
    }

    OpenMode GetMode() const {
        return mode;
    }

private:
    std::string debug_path;
    FileUtil::IOFile file;
    OpenMode mode;
};

} // namespace Vfs
