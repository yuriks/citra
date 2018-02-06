// Copyright 2014 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#pragma once

#include <cstddef>
#include <string>
#include "common/common_types.h"
#include "common/printable.h"
#include "core/hle/result.h"
#include "vfs/vfs.h"

namespace Vfs {

class File : public Common::VirtualPrintable {
public:
    virtual ~File() = default;

    /**
     * Reads data from the file.
     *
     * @param offset position (in bytes) to read data from
     * @param length number of bytes to read
     * @param buffer buffer to receive the read data
     * @return number of bytes actually read
     */
    virtual Result<size_t> Read(u64 offset, size_t length, u8* buffer) = 0;

    /**
     * Writes data to the file.
     *
     * @param offset position (in bytes) to write data to
     * @param length number of bytes to write
     * @param buffer buffer to write data from
     * @return number of bytes actually written
     */
    virtual Result<size_t> Write(u64 offset, size_t length, const u8* buffer) = 0;

    /** Returns the size of the file in bytes. */
    virtual Result<u64> GetSize() = 0;

    /**
     * Changes the size of the file. If smaller than the current size, the file will be truncated.
     * If larger, the new space will be filled with zeros.
     *
     * @param size new size of the file, in bytes
     */
    virtual Result<> SetSize(u64 size) = 0;

    /** Closes the file. */
    virtual Result<> Close() = 0;

    /** Flushes any pending writes. */
    virtual Result<> Flush() = 0;
};

/// Abstract implementation of File, which stubs out functions unsupported for read-only files.
class ReadOnlyFile : public File {
public:
    Result<size_t> Write(u64 offset, size_t length, const u8* buffer) override {
        return Error::UnsupportedOperation;
    }

    Result<> SetSize(u64 size) override {
        return Error::UnsupportedOperation;
    }

    Result<> Flush() override {
        return Ok;
    }
};

class StreamFile : public Common::VirtualPrintable {
public:
    virtual ~StreamFile() = default;

    /**
     * Reads data from the file and advances the cursor by the amount read.
     *
     * @param length number of bytes to read
     * @param buffer buffer to receive the read data
     * @return number of bytes actually read, or ERR_END_OF_FILE if at EOF
     */
    virtual Result<size_t> Read(size_t length, u8* buffer) = 0;

    /**
     * Writes data to the file and advances the cursor by the amount written.
     *
     * @param length number of bytes to write
     * @param buffer buffer to write data from
     * @return number of bytes actually written
     */
    virtual Result<size_t> Write(size_t length, const u8* buffer) = 0;

    /**
     * Moves the read/write cursor to a different position in the file.
     *
     * @param offset_from_beginning offset inside the file to move the cursor to
     * @return ERR_END_OF_FILE if position is outside current file bounds
     */
    virtual Result<> Seek(u64 offset_from_beginning) = 0;

    /** Returns the position of the read/write cursor in bytes from the beginning of the file. */
    virtual Result<u64> Tell() = 0;

    /** Returns the size of the file in bytes. */
    virtual Result<u64> GetSize() const = 0;

    /**
     * Changes the size of the file. If smaller than the current size, the file will be truncated.
     * If larger, the new space will be filled with zeros.
     *
     * @param size new size of the file, in bytes
     */
    virtual Result<> SetSize(u64 size) = 0;

    /** Closes the file. */
    virtual Result<> Close() = 0;

    /** Flushes any pending writes. */
    virtual Result<> Flush() = 0;
};

} // namespace Vfs
