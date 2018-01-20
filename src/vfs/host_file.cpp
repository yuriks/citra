// Copyright 2017 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include "vfs/host_file.h"

namespace Vfs {

ResultVal<HostFile> HostFile::Open(std::string path) {
    FileUtil::IOFile file;
    if (!file.Open(path, "rb")) {
        return ERR_UNKNOWN_ERROR;
    }
    return MakeResult<HostFile>(std::move(file), std::move(path));
}

HostFile::HostFile(FileUtil::IOFile&& file, std::string debug_path)
    : debug_path(std::move(debug_path)), file(std::move(file)) {}

HostFile::HostFile(FileUtil::IOFile&& file) : HostFile(std::move(file), "") {}

HostFile::~HostFile() = default;

void HostFile::DebugFmt(fmt::Writer& w) const {
    w.write("HostFile{{path={}}}", debug_path);
}

ResultVal<size_t> HostFile::Read(size_t length, u8* buffer) {
    size_t read = file.ReadBytes(buffer, length);

    if (read == 0 && file.IsEof()) {
        return ERR_END_OF_FILE;
    } else if (read == -1) {
        return ERR_UNKNOWN_ERROR;
    } else {
        return MakeResult<size_t>(read);
    }
}

ResultVal<size_t> HostFile::Write(size_t length, const u8* buffer) {
    return ERR_UNSUPPORTED_OPERATION;
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
    return ERR_UNSUPPORTED_OPERATION;
}

ResultCode HostFile::Close() {
    file.Clear();
    return file.Close() ? RESULT_SUCCESS : ERR_UNKNOWN_ERROR;
}

ResultCode HostFile::Flush() {
    return RESULT_SUCCESS;
}

} // namespace Vfs
