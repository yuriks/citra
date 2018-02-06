// Copyright 2017 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include <array>
#include <initializer_list>
#include <catch.hpp>
#include "common/file_util.h"
#include "vfs/memory_file.h"

namespace Vfs {

TEST_CASE("MemoryFile", "[vfs]") {
    MemoryFile file({0, 1, 2, 3, 4});

    SECTION("Read past EOF reads nothing") {
        std::array<u8, 2> buffer{9, 9};

        auto result = file.Read(6, 2, buffer.data());
        REQUIRE(result.Unwrap() == 0);
    }
    SECTION("Read with smaller buffer does not overflow") {
        std::array<u8, 5> buffer;
        buffer.fill(9);

        auto result = file.Read(1, 3, buffer.data());
        REQUIRE(result.Unwrap() == 3);
        REQUIRE(buffer == (std::array<u8, 5>{1, 2, 3, 9, 9}));
    }
    SECTION("Read with larger buffer does not overread") {
        std::array<u8, 7> buffer;
        buffer.fill(9);

        auto result = file.Read(1, 7, buffer.data());
        REQUIRE(result.Unwrap() == 4);
        REQUIRE(buffer == (std::array<u8, 7>{1, 2, 3, 4, 9, 9, 9}));
    }
    SECTION("Write past EOF reads nothing") {
        std::array<u8, 2> buffer{9, 9};

        auto result = file.Write(6, 2, buffer.data());
        REQUIRE(result.Unwrap() == 0);
        REQUIRE(file.GetData() == (std::vector<u8>{0, 1, 2, 3, 4}));
    }
    SECTION("Write with larger buffer does not overflow") {
        const std::array<u8, 7> buffer{10, 11, 12, 13, 14, 15, 16};
        auto result = file.Write(1, 7, buffer.data());
        REQUIRE(result.Unwrap() == 4);
        REQUIRE(file.GetData() == (std::vector<u8>{0, 10, 11, 12, 13}));
    }
    SECTION("Write with smaller buffer does not overread") {
        const std::array<u8, 3> buffer{10, 11, 12};
        auto result = file.Write(1, 3, buffer.data());
        REQUIRE(result.Unwrap() == 3);
        REQUIRE(file.GetData() == (std::vector<u8>{0, 10, 11, 12, 4}));
    }
    SECTION("GetSize works") {
        auto result = file.GetSize();
        REQUIRE(result.Unwrap() == 5);
    }
    SECTION("Resize to smaller truncates") {
        auto result = file.SetSize(3);
        REQUIRE(result.Succeeded());
        REQUIRE(file.GetSize().Unwrap() == 3);
    }
    SECTION("Resize to larger fills with zeros") {
        auto result = file.SetSize(7);
        REQUIRE(result.Succeeded());
        REQUIRE(file.GetData() == (std::vector<u8>{0, 1, 2, 3, 4, 0, 0}));
    }
    SECTION("Flush succeeds") {
        auto result = file.Flush();
        REQUIRE(result.Succeeded());
    }
}

} // namespace Vfs
