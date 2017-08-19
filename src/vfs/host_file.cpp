// Copyright 2017 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include "vfs/host_file.h"

namespace Vfs {

ResultVal<HostFile> HostFile::Open(std::string path, OpenMode mode) {
    const char* fopen_mode;
    switch (mode) {
    case OpenMode::READ:
        fopen_mode = "rb";
        break;
    case OpenMode::WRITE:
    case OpenMode::READ | OpenMode::WRITE:
        fopen_mode = "r+b";
        break;
    case OpenMode::WRITE | OpenMode::TRUNCATE:
        fopen_mode = "wb";
        break;
    case OpenMode::READ | OpenMode::WRITE | OpenMode::TRUNCATE:
        fopen_mode = "w+b";
        break;
    case OpenMode::APPEND:
        fopen_mode = "ab";
        break;
    case OpenMode::READ | OpenMode::APPEND:
        fopen_mode = "a+b";
        break;
    default: {
        std::string mode_str = fmt::format("{}", mode);
        LOG_ERROR(Service_FS, "Unsupported open mode: %s", mode_str.c_str());
        return ERR_INVALID_OPEN_MODE;
    }
    }

    FileUtil::IOFile file;
    if (!file.Open(path, fopen_mode)) {
        return ERR_UNKNOWN_ERROR;
    }
    return MakeResult<HostFile>(std::move(file), mode, std::move(path));
}

HostFile::HostFile(FileUtil::IOFile&& file, OpenMode mode, std::string debug_path)
    : debug_path(std::move(debug_path)), file(std::move(file)), mode(mode) {}

HostFile::HostFile(FileUtil::IOFile&& file, OpenMode mode) : HostFile(std::move(file), mode, "") {}

HostFile::~HostFile() = default;

std::string HostFile::DebugStr() const {
    return fmt::format("HostFile{{path={}, mode={}}}", debug_path, mode);
}

ResultVal<size_t> HostFile::Read(size_t length, u8* buffer) {
    if ((mode & OpenMode::READ) == OpenMode::NONE) {
        return ERR_INVALID_OPEN_MODE;
    }

    size_t read = file.ReadBytes(buffer, length);
    return read != -1 ? MakeResult<size_t>(read) : ERR_UNKNOWN_ERROR;
}

ResultVal<size_t> HostFile::Write(size_t length, const u8* buffer) {
    if ((mode & (OpenMode::WRITE | OpenMode::APPEND)) == OpenMode::NONE) {
        return ERR_INVALID_OPEN_MODE;
    }

    size_t written = file.WriteBytes(buffer, length);
    return written != -1 ? MakeResult<size_t>(written) : ERR_UNKNOWN_ERROR;
}

ResultCode HostFile::Seek(u64 offset_from_beginning) {
    file.Clear();
    return file.Seek(offset_from_beginning, SEEK_SET) ? RESULT_SUCCESS : ERR_UNKNOWN_ERROR;
}

ResultVal<u64> HostFile::Tell() {
    u64 offset = file.Tell();
    return offset != -1 ? MakeResult<u64>(offset) : ERR_UNKNOWN_ERROR;
}

ResultVal<u64> HostFile::GetSize() const {
    return MakeResult<u64>(file.GetSize());
}

ResultCode HostFile::SetSize(u64 size) {
    file.Clear();
    return file.Resize(size) ? RESULT_SUCCESS : ERR_UNKNOWN_ERROR;
}

ResultCode HostFile::Flush() {
    file.Clear();
    return file.Flush() ? RESULT_SUCCESS : ERR_UNKNOWN_ERROR;
}

} // namespace Vfs
