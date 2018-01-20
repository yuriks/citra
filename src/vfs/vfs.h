// Copyright 2017 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#pragma once

#include "core/hle/result.h"

namespace Vfs {

constexpr ResultCode ERR_UNSUPPORTED_OPERATION = UnimplementedFunction(ErrorModule::Citra);
constexpr ResultCode ERR_UNKNOWN_ERROR(1, ErrorModule::Citra, ErrorSummary::InvalidState,
                                       ErrorLevel::Permanent);
constexpr ResultCode ERR_END_OF_FILE(2, ErrorModule::Citra, ErrorSummary::WouldBlock,
                                     ErrorLevel::Status);

} // namespace Vfs
