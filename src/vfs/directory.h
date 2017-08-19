// Copyright 2017 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#pragma once

#include <iosfwd>
#include <string>
#include <boost/optional.hpp>
#include "common/common_types.h"
#include "common/printable.h"

namespace Vfs {

template <typename Path>
struct DirectoryEntry {
    enum Flags {
        FlagDirectory = 1 << 0,
        FlagHidden = 1 << 1,
        FlagArchive = 1 << 2,
        FlagReadOnly = 1 << 3,
    };

    /// File/directory path fragment
    Path filename;
    /// File size (for files only)
    u64 file_size;
    Flags flags;
};

template <typename Path>
class DirectoryIterator : NonCopyable, public Common::VirtualPrintable {
public:
    DirectoryIterator() = default;
    virtual ~DirectoryIterator() = default;

    /// Returns a descriptive string for the filesystem, for debugging purposes.
    virtual std::string DebugStr() const = 0;

    /**
     * Gets the next directory entry in the sequence.
     * @return The next directory entry, or none if iteration has finished.
     */
    virtual boost::optional<DirectoryEntry<Path>> GetEntry() = 0;
};

} // namespace Vfs