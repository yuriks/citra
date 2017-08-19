// Copyright 2017 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include <sstream>
#include "common/string_util.h"
#include "vfs/path.h"

namespace Vfs {

boost::optional<std::string> Path::AsString() const {
    struct visitor : boost::static_visitor<boost::optional<std::string>> {
        boost::optional<std::string> operator()(boost::blank) const {
            return boost::none;
        }

        boost::optional<std::string> operator()(const std::string& data) const {
            return data;
        }

        boost::optional<std::string> operator()(const std::u16string& data) const {
            return Common::UTF16ToUTF8(data);
        }

        boost::optional<std::string> operator()(const std::vector<u8>& data) const {
            return boost::none;
        }
    };
    return boost::apply_visitor(visitor(), data);
}

boost::optional<std::u16string> Path::AsU16String() const {
    struct visitor : boost::static_visitor<boost::optional<std::u16string>> {
        boost::optional<std::u16string> operator()(boost::blank) const {
            return boost::none;
        }

        boost::optional<std::u16string> operator()(const std::string& data) const {
            return Common::UTF8ToUTF16(data);
        }

        boost::optional<std::u16string> operator()(const std::u16string& data) const {
            return data;
        }

        boost::optional<std::u16string> operator()(const std::vector<u8>& data) const {
            return boost::none;
        }
    };
    return boost::apply_visitor(visitor(), data);
}

std::string Path::DebugStr() const {
    struct visitor : boost::static_visitor<std::string> {
        std::string operator()(boost::blank) const {
            return "[blank]";
        }

        std::string operator()(const std::string& data) const {
            // TOOD(yuriks): Use fmt
            return "[string: " + data + "]";
        }

        std::string operator()(const std::u16string& data) const {
            // TOOD(yuriks): Use fmt
            return "[u16string: " + Common::UTF16ToUTF8(data) + "]";
        }

        std::string operator()(const std::vector<u8>& data) const {
            // TODO(yuriks): Use fmt
            std::stringstream res;
            res << "[binary: ";
            for (unsigned byte : data)
                res << std::hex << std::setw(2) << std::setfill('0') << byte;
            res << ']';
            return res.str();
        }
    };
    return boost::apply_visitor(visitor(), data);
}

} // namespace Vfs
