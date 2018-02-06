// Copyright 2017 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#pragma once

#include "common/result.h"

namespace Vfs {

enum class Error {
    UnknownError = 0,
    UnsupportedOperation,
    EndOfFile,
};

template <typename T = Common::OkType>
using Result = Common::Result<T, Error>;

static const Result<> Ok = Common::OkType{};

} // namespace Vfs
