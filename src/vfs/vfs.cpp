// Copyright 2017 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include <iostream>
#include "common/string_util.h"
#include "vfs/directory.h"
#include "vfs/file.h"
#include "vfs/filesystem.h"
#include "vfs/path.h"
#include "vfs/vfs.h"

namespace Vfs {

void format_arg(fmt::BasicFormatter<char>& f, const char*& format, const OpenMode& mode) {
    auto& writer = f.writer();
    if ((mode & OpenMode::READ) != OpenMode::NONE)
        writer << 'r';
    if ((mode & OpenMode::WRITE) != OpenMode::NONE)
        writer << 'w';
    if ((mode & OpenMode::TRUNCATE) != OpenMode::NONE)
        writer << 'c';
    if ((mode & OpenMode::APPEND) != OpenMode::NONE)
        writer << 'a';
}

} // namespace Vfs
