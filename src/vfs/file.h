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
    virtual ResultVal<size_t> Read(u64 offset, size_t length, u8* buffer) = 0;

    /**
     * Writes data to the file.
     *
     * @param offset position (in bytes) to write data to
     * @param length number of bytes to write
     * @param buffer buffer to write data from
     * @return number of bytes actually written
     */
    virtual ResultVal<size_t> Write(u64 offset, size_t length, const u8* buffer) = 0;

    /** Returns the size of the file in bytes. */
    virtual ResultVal<u64> GetSize() = 0;

    /**
     * Changes the size of the file. If smaller than the current size, the file will be truncated.
     * If larger, the new space will be filled with zeros.
     *
     * @param size new size of the file, in bytes
     */
    virtual ResultCode SetSize(u64 size) = 0;

    /** Closes the file. */
    virtual ResultCode Close() = 0;

    /** Flushes any pending writes. */
    virtual ResultCode Flush() = 0;
};

/// Abstract implementation of File, which stubs out functions unsupported for read-only files.
class ReadOnlyFile : public File {
public:
    ResultVal<size_t> Write(u64 offset, size_t length, const u8* buffer) override {
        return ERR_UNSUPPORTED_OPERATION;
    }

    ResultCode SetSize(u64 size) override {
        return ERR_UNSUPPORTED_OPERATION;
    }

    ResultCode Flush() override {
        return RESULT_SUCCESS;
    }
};

} // namespace Vfs
