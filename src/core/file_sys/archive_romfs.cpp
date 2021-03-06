// Copyright 2014 Citra Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#include "common/common_types.h"

#include "core/file_sys/archive_romfs.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
// FileSys namespace

namespace FileSys {

Archive_RomFS::Archive_RomFS(const Loader::AppLoader& app_loader) {
    // Load the RomFS from the app
    if (Loader::ResultStatus::Success != app_loader.ReadRomFS(raw_data)) {
        WARN_LOG(FILESYS, "Unable to read RomFS!");
    }
}

Archive_RomFS::~Archive_RomFS() {
}

/**
 * Read data from the archive
 * @param offset Offset in bytes to start reading data from
 * @param length Length in bytes of data to read from archive
 * @param buffer Buffer to read data into
 * @return Number of bytes read
 */
size_t Archive_RomFS::Read(const u64 offset, const u32 length, u8* buffer) const {
    DEBUG_LOG(FILESYS, "called offset=%d, length=%d", offset, length);
    memcpy(buffer, &raw_data[(u32)offset], length);
    return length;
}

/**
 * Write data to the archive
 * @param offset Offset in bytes to start writing data to
 * @param length Length in bytes of data to write to archive
 * @param buffer Buffer to write data from
 * @param flush  The flush parameters (0 == do not flush)
 * @return Number of bytes written
 */
size_t Archive_RomFS::Write(const u64 offset, const u32 length, const u32 flush, u8* buffer) {
    ERROR_LOG(FILESYS, "Attempted to write to ROMFS.");
    return 0;
}

/**
 * Get the size of the archive in bytes
 * @return Size of the archive in bytes
 */
size_t Archive_RomFS::GetSize() const {
    return sizeof(u8) * raw_data.size();
}

/**
 * Set the size of the archive in bytes
 */
void Archive_RomFS::SetSize(const u64 size) {
    ERROR_LOG(FILESYS, "Attempted to set the size of ROMFS");
}

} // namespace FileSys
