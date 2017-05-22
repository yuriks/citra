// Copyright 2017 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include <algorithm>
#include "vfs/subrange_file.h"
#include "vfs/vfs.h"

namespace Vfs {

SubrangeFile::SubrangeFile(std::shared_ptr<File> base_file, u64 offset, u64 size)
    : base_file(base_file), file_offset(offset), file_size(size) {}

void SubrangeFile::DebugFmt(fmt::Writer& w) const {
    w.write("SubrangeFile{{offset={}, size={}, base_file={}}}", file_offset, file_size, *base_file);
}

ResultVal<size_t> SubrangeFile::Read(u64 offset, size_t length, u8* buffer) {
    if (offset >= file_size) {
        // Attempt to read past EOF
        return MakeResult<size_t>(0);
    }
    return base_file->Read(offset + file_offset, std::min(length, file_size - offset), buffer);
}

ResultVal<size_t> SubrangeFile::Write(u64 offset, size_t length, const u8* buffer) {
    if (offset >= file_size) {
        // Attempt to write past EOF
        return MakeResult<size_t>(0);
    }
    return base_file->Write(offset + file_offset, std::min(length, file_size - offset), buffer);
}

ResultVal<u64> SubrangeFile::GetSize() {
    CASCADE_RESULT(u64 base_size, base_file->GetSize());
    if (file_offset >= base_size) {
        return MakeResult<u64>(0);
    } else {
        return MakeResult<u64>(std::min(file_size, base_size - file_offset));
    }
}

ResultCode SubrangeFile::SetSize(u64 size) {
    return ERR_UNSUPPORTED_OPERATION;
}

ResultCode SubrangeFile::Close() {
    return base_file->Close();
}

ResultCode SubrangeFile::Flush() {
    return base_file->Flush();
}

} // namespace Vfs
