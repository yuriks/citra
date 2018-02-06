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
    static Result<HostFile> Open(std::string path);
    HostFile(FileUtil::IOFile&& file, std::string debug_path);
    HostFile(FileUtil::IOFile&& file);
    ~HostFile();

    HostFile(HostFile&&) = default;
    HostFile& operator=(HostFile&&) = default;

    void DebugFmt(fmt::Writer& w) const override;

    Result<size_t> Read(size_t length, u8* buffer) override;
    Result<size_t> Write(size_t length, const u8* buffer) override;
    Result<> Seek(u64 offset_from_beginning) override;
    Result<u64> Tell() override;
    Result<u64> GetSize() const override;
    Result<> SetSize(u64 size) override;
    Result<> Close() override;
    Result<> Flush() override;

    const FileUtil::IOFile& GetHostFile() const {
        return file;
    }

private:
    std::string debug_path;
    FileUtil::IOFile file;
};

} // namespace Vfs
