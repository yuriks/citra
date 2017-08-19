// Copyright 2017 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#pragma once

#include <memory>
#include <string>
#include "common/common_types.h"
#include "common/printable.h"
#include "core/hle/result.h"
#include "vfs/directory.h"
#include "vfs/file.h"

namespace Vfs {

class Mode;

template <typename Path>
class Filesystem : NonCopyable, public Common::VirtualPrintable {
public:
    Filesystem() = default;
    virtual ~Filesystem() = default;

    /// Returns a descriptive string for the filesystem, for debugging purposes.
    virtual std::string DebugStr() const = 0;

    /**
     * Open a file specified by its path, using the specified mode
     * @param path Path relative to the archive
     * @param mode Mode to open the file with
     * @return Opened file, or error code
     */
    virtual ResultVal<std::unique_ptr<File>> OpenFile(const Path& path, const Mode& mode) const = 0;

    /**
     * Delete a file specified by its path
     * @param path Path relative to the archive
     * @return Result of the operation
     */
    virtual ResultCode DeleteFile(const Path& path) const = 0;

    /**
     * Rename a File specified by its path
     * @param src_path Source path relative to the archive
     * @param dest_path Destination path relative to the archive
     * @return Result of the operation
     */
    virtual ResultCode RenameFile(const Path& src_path, const Path& dest_path) const = 0;
    /**
     * Delete a directory specified by its path
     * @param path Path relative to the archive
     * @return Result of the operation
     */
    virtual ResultCode DeleteDirectory(const Path& path) const = 0;

    /**
     * Delete a directory specified by its path and anything under it
     * @param path Path relative to the archive
     * @return Result of the operation
     */
    virtual ResultCode DeleteDirectoryRecursively(const Path& path) const = 0;

    /**
     * Create a file specified by its path
     * @param path Path relative to the Archive
     * @param size The size of the new file, filled with zeroes
     * @return Result of the operation
     */
    virtual ResultCode CreateFile(const Path& path, u64 size) const = 0;

    /**
     * Create a directory specified by its path
     * @param path Path relative to the archive
     * @return Result of the operation
     */
    virtual ResultCode CreateDirectory(const Path& path) const = 0;

    /**
     * Rename a Directory specified by its path
     * @param src_path Source path relative to the archive
     * @param dest_path Destination path relative to the archive
     * @return Result of the operation
     */
    virtual ResultCode RenameDirectory(const Path& src_path, const Path& dest_path) const = 0;

    /**
     * Open a directory specified by its path
     * @param path Path relative to the archive
     * @return Opened directory, or error code
     */
    virtual ResultVal<std::unique_ptr<DirectoryIterator<Path>>> OpenDirectory(
        const Path& path) const = 0;

    /**
     * Get the free space
     * @return The number of free bytes in the archive
     */
    virtual u64 GetFreeBytes() const = 0;
};

/**
 * Abstract implementation of Filesystem, which stubs out functions unsupported for read-only
 * filesystems.
 */
template <typename Path>
class ReadOnlyFilesystem : public Filesystem<Path> {
public:
    ResultCode DeleteFile(const Path& path) const override {
        return ERR_UNSUPPORTED_OPERATION;
    }

    ResultCode RenameFile(const Path& src_path, const Path& dest_path) const override {
        return ERR_UNSUPPORTED_OPERATION;
    }

    ResultCode DeleteDirectory(const Path& path) const override {
        return ERR_UNSUPPORTED_OPERATION;
    }

    ResultCode DeleteDirectoryRecursively(const Path& path) const override {
        return ERR_UNSUPPORTED_OPERATION;
    }

    ResultCode CreateFile(const Path& path, u64 size) const override {
        return ERR_UNSUPPORTED_OPERATION;
    }

    ResultCode CreateDirectory(const Path& path) const override {
        return ERR_UNSUPPORTED_OPERATION;
    }

    ResultCode RenameDirectory(const Path& src_path, const Path& dest_path) const override {
        return ERR_UNSUPPORTED_OPERATION;
    }
};

} // namespace Vfs