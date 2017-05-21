// Copyright 2017 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#pragma once

#include "core/hle/result.h"

namespace Vfs {

constexpr ResultCode ERR_UNSUPPORTED_OPERATION = UnimplementedFunction(ErrorModule::Citra);

} // namespace Vfs
