// Copyright 2014 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include <algorithm>
#include <cstdio>
#include <memory>
#include "common/common_types.h"
#include "common/file_util.h"
#include "common/logging/log.h"
#include "core/file_sys/disk_archive.h"
#include "core/file_sys/errors.h"
#include "vfs/random_access_adapter.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
// FileSys namespace

namespace FileSys {

ResultVal<size_t> DiskFile::Read(const u64 offset, const size_t length, u8* buffer) const {
    if (!mode.read_flag)
        return ERROR_INVALID_OPEN_FLAGS;

    file->Seek(offset, SEEK_SET);
    return MakeResult<size_t>(file->ReadBytes(buffer, length));
}

ResultVal<size_t> DiskFile::Write(const u64 offset, const size_t length, const bool flush,
                                  const u8* buffer) {
    if (!mode.write_flag)
        return ERROR_INVALID_OPEN_FLAGS;

    file->Seek(offset, SEEK_SET);
    size_t written = file->WriteBytes(buffer, length);
    if (flush)
        file->Flush();
    return MakeResult<size_t>(written);
}

u64 DiskFile::GetSize() const {
    return file->GetSize();
}

bool DiskFile::SetSize(const u64 size) const {
    file->Resize(size);
    file->Flush();
    return true;
}

bool DiskFile::Close() {
    return file->Close();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

VfsDiskFile::VfsDiskFile(std::unique_ptr<Vfs::HostFile> vfs_file)
    : vfs_file(std::make_shared<Vfs::RandomAccessAdapter>(std::move(vfs_file))) {}

ResultVal<size_t> VfsDiskFile::Read(u64 offset, size_t length, u8* buffer) const {
    auto result = vfs_file->Read(offset, length, buffer);
    if (result.Succeeded())
        return result;
    else if (result.Code() == Vfs::ERR_INVALID_OPEN_MODE)
        return ERROR_INVALID_OPEN_FLAGS;
    else {
        LOG_ERROR(Service_FS, "Unhandled vfs error: %08x", result.Code().raw);
        return result;
    }
}

ResultVal<size_t> VfsDiskFile::Write(u64 offset, size_t length, bool flush,
                                     const u8* buffer) {
    auto result = vfs_file->Write(offset, length, buffer);
    if (result.Succeeded()) {
        if (flush)
            vfs_file->Flush();
        return result;
    } else if (result.Code() == Vfs::ERR_INVALID_OPEN_MODE)
        return ERROR_INVALID_OPEN_FLAGS;
    else {
        LOG_ERROR(Service_FS, "Unhandled vfs error: %08x", result.Code().raw);
        return result;
    }
}

u64 VfsDiskFile::GetSize() const {
    auto result = vfs_file->GetSize();
    if (result.Succeeded())
        return result.Unwrap();
    else {
        LOG_ERROR(Service_FS, "Unhandled vfs error: %08x", result.Code().raw);
        return 0;
    }
}

bool VfsDiskFile::SetSize(u64 size) const {
    auto result = vfs_file->SetSize(size);
    if (result.IsSuccess()) {
        vfs_file->Flush();
        return true;
    } else {
        return false;
    }
}

bool VfsDiskFile::Close() {
    vfs_file = nullptr;
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

DiskDirectory::DiskDirectory(const std::string& path) : directory() {
    unsigned size = FileUtil::ScanDirectoryTree(path, directory);
    directory.size = size;
    directory.isDirectory = true;
    children_iterator = directory.children.begin();
}

u32 DiskDirectory::Read(const u32 count, Entry* entries) {
    u32 entries_read = 0;

    while (entries_read < count && children_iterator != directory.children.cend()) {
        const FileUtil::FSTEntry& file = *children_iterator;
        const std::string& filename = file.virtualName;
        Entry& entry = entries[entries_read];

        LOG_TRACE(Service_FS, "File %s: size=%llu dir=%d", filename.c_str(), file.size,
                  file.isDirectory);

        // TODO(Link Mauve): use a proper conversion to UTF-16.
        for (size_t j = 0; j < FILENAME_LENGTH; ++j) {
            entry.filename[j] = filename[j];
            if (!filename[j])
                break;
        }

        FileUtil::SplitFilename83(filename, entry.short_name, entry.extension);

        entry.is_directory = file.isDirectory;
        entry.is_hidden = (filename[0] == '.');
        entry.is_read_only = 0;
        entry.file_size = file.size;

        // We emulate a SD card where the archive bit has never been cleared, as it would be on
        // most user SD cards.
        // Some homebrews (blargSNES for instance) are known to mistakenly use the archive bit as a
        // file bit.
        entry.is_archive = !file.isDirectory;

        ++entries_read;
        ++children_iterator;
    }
    return entries_read;
}

} // namespace FileSys
