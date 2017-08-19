// Copyright 2017 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#pragma once

#include <iosfwd>
#include <string>
#include <vector>
#include <boost/optional.hpp>
#include <boost/variant.hpp>
#include "common/common_types.h"
#include "common/printable.h"

namespace Vfs {

class Path : public Common::Printable {
public:
    Path() = default;
    Path(std::string utf8_path) : data(std::move(utf8_path)) {}
    Path(std::u16string utf16_path) : data(std::move(utf16_path)) {}
    Path(std::vector<u8> binary_data) : data(std::move(binary_data)) {}

    boost::variant<boost::blank, std::string, std::u16string, std::vector<u8>> data;

    /// Returns a UTF-8 encoding of UTF-8 or UTF-16 strings, or none otherwise.
    boost::optional<std::string> AsString() const;
    /// Returns a UTF-16 encoding of UTF-8 or UTF-16 strings, or none otherwise.
    boost::optional<std::u16string> AsU16String() const;

    /// Returns a string representation of the path for debugging purposes.
    std::string DebugStr() const;
};

} // namespace Vfs
