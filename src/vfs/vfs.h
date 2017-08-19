// Copyright 2017 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#pragma once

#include "core/hle/result.h"

namespace Vfs {

constexpr ResultCode ERR_UNSUPPORTED_OPERATION = UnimplementedFunction(ErrorModule::Citra);
constexpr ResultCode ERR_UNKNOWN_ERROR(1, ErrorModule::Citra, ErrorSummary::InvalidState,
                                       ErrorLevel::Permanent);
constexpr ResultCode ERR_INVALID_OPEN_MODE(2, ErrorModule::Citra, ErrorSummary::Canceled,
                                           ErrorLevel::Usage);
constexpr ResultCode ERR_END_OF_FILE(3, ErrorModule::Citra, ErrorSummary::WouldBlock,
                                     ErrorLevel::Status);

enum class OpenMode {
    NONE = 0,
    READ = 1 << 0,
    WRITE = 1 << 1,
    /// Mutually exclusive with WRITE
    APPEND = 1 << 2,
    CREATE = 1 << 3,
    TRUNCATE = 1 << 4,
};

constexpr OpenMode operator|(OpenMode lhs, OpenMode rhs) {
    return static_cast<OpenMode>(static_cast<int>(lhs) | static_cast<int>(rhs));
}

constexpr OpenMode operator &(OpenMode lhs, OpenMode rhs) {
    return static_cast<OpenMode>(static_cast<int>(lhs) & static_cast<int>(rhs));
}

constexpr OpenMode& operator|=(OpenMode& lhs, OpenMode rhs) {
    return lhs = lhs | rhs;
}

void format_arg(fmt::BasicFormatter<char>& f, const char*& format, const OpenMode& mode);

} // namespace Vfs
