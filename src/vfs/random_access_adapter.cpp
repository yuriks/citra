// Copyright 2017 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include "vfs/random_access_adapter.h"

namespace Vfs {

void RandomAccessAdapter::DebugFmt(fmt::Writer& w) const {
    w.write("RandomAccessAdapter{{{}}}", *base_file);
}

Result<size_t> RandomAccessAdapter::Read(u64 offset, size_t length, u8* buffer) {
    CASCADE_COMMON_RESULT(auto result, base_file->Seek(offset));
    return base_file->Read(length, buffer);
}

Result<size_t> RandomAccessAdapter::Write(u64 offset, size_t length, const u8* buffer) {
    CASCADE_COMMON_RESULT(auto result, base_file->Seek(offset));
    return base_file->Write(length, buffer);
}

Result<u64> RandomAccessAdapter::GetSize() {
    return base_file->GetSize();
}

Result<> RandomAccessAdapter::SetSize(u64 size) {
    return base_file->SetSize(size);
}

Result<> RandomAccessAdapter::Close() {
    return base_file->Close();
}

Result<> RandomAccessAdapter::Flush() {
    return base_file->Flush();
}

} // namespace Vfs
