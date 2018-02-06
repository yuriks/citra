// Copyright 2017 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include "vfs/host_file.h"

namespace Vfs {

Result<HostFile> HostFile::Open(std::string path) {
    FileUtil::IOFile file;
    if (!file.Open(path, "rb")) {
        return Error::UnknownError;
    }
    return HostFile(std::move(file), std::move(path));
}

HostFile::HostFile(FileUtil::IOFile&& file, std::string debug_path)
    : debug_path(std::move(debug_path)), file(std::move(file)) {}

HostFile::HostFile(FileUtil::IOFile&& file) : HostFile(std::move(file), "") {}

HostFile::~HostFile() = default;

void HostFile::DebugFmt(fmt::Writer& w) const {
    w.write("HostFile{{path={}}}", debug_path);
}

Result<size_t> HostFile::Read(size_t length, u8* buffer) {
    size_t read = file.ReadBytes(buffer, length);

    if (read == 0 && file.IsEof()) {
        return Error::EndOfFile;
    } else if (read == -1) {
        return Error::UnknownError;
    } else {
        return read;
    }
}

Result<size_t> HostFile::Write(size_t length, const u8* buffer) {
    return Error::UnsupportedOperation;
}

Result<> HostFile::Seek(u64 offset_from_beginning) {
    file.Clear();
    return file.Seek(offset_from_beginning, SEEK_SET) ? Ok : Error::UnknownError;
}

Result<u64> HostFile::Tell() {
    u64 offset = file.Tell();
    return offset != -1 ? Result<u64>(offset) : Error::UnknownError;
}

Result<u64> HostFile::GetSize() const {
    return file.GetSize();
}

Result<> HostFile::SetSize(u64 size) {
    return Error::UnsupportedOperation;
}

Result<> HostFile::Close() {
    file.Clear();
    return file.Close() ? Ok : Error::UnknownError;
}

Result<> HostFile::Flush() {
    return Ok;
}

} // namespace Vfs
