// Copyright 2017 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include <array>
#include <initializer_list>
#include <memory>
#include <catch.hpp>
#include "common/file_util.h"
#include "vfs/memory_file.h"
#include "vfs/subrange_file.h"

namespace Vfs {

TEST_CASE("SubrangeFile", "[vfs]") {
    auto mem_file = std::make_shared<MemoryFile>(std::vector<u8>{0, 1, 2, 3, 4, 5, 6});

    SECTION("Read with smaller buffer does not overflow") {
        SubrangeFile file(mem_file, 1, 5);

        std::vector<u8> buffer{9, 9, 9, 9, 9};
        auto result = file.Read(1, 3, buffer.data());

        REQUIRE(result.Unwrap() == 3);
        REQUIRE(buffer == (std::vector<u8>{2, 3, 4, 9, 9}));
    }
    SECTION("Read past window is clamped") {
        SubrangeFile file(mem_file, 1, 5);

        std::vector<u8> buffer{9, 9, 9, 9, 9};
        auto result = file.Read(4, 3, buffer.data());

        REQUIRE(result.Unwrap() == 1);
        REQUIRE(buffer == (std::vector<u8>{5, 9, 9, 9, 9}));
    }
    SECTION("Read outside window reads nothing") {
        SubrangeFile file(mem_file, 1, 5);

        std::vector<u8> buffer{9, 9, 9, 9, 9};
        auto result = file.Read(6, 3, buffer.data());

        REQUIRE(result.Unwrap() == 0);
        REQUIRE(buffer == (std::vector<u8>{9, 9, 9, 9, 9}));
    }
    SECTION("Read past underlying file is truncated by it") {
        SubrangeFile file(mem_file, 5, 5);

        std::vector<u8> buffer{9, 9, 9, 9, 9};
        auto result = file.Read(0, 5, buffer.data());

        REQUIRE(result.Unwrap() == 2);
        REQUIRE(buffer == (std::vector<u8>{5, 6, 9, 9, 9}));
    }
    SECTION("Write with smaller buffer does not overread") {
        SubrangeFile file(mem_file, 1, 5);

        std::vector<u8> buffer{10, 11, 12, 13, 14};
        auto result = file.Write(1, 3, buffer.data());

        REQUIRE(result.Unwrap() == 3);
        REQUIRE(mem_file->GetData() == (std::vector<u8>{0, 1, 10, 11, 12, 5, 6}));
    }
    SECTION("Write past window is clamped") {
        SubrangeFile file(mem_file, 1, 5);

        std::vector<u8> buffer{10, 11};
        auto result = file.Write(4, 2, buffer.data());

        REQUIRE(result.Unwrap() == 1);
        REQUIRE(mem_file->GetData() == (std::vector<u8>{0, 1, 2, 3, 4, 10, 6}));
    }
    SECTION("Write outside window writes nothing") {
        SubrangeFile file(mem_file, 1, 5);

        std::vector<u8> buffer{10, 11, 12, 13, 14};
        auto result = file.Write(6, 3, buffer.data());

        REQUIRE(result.Unwrap() == 0);
        REQUIRE(mem_file->GetData() == (std::vector<u8>{0, 1, 2, 3, 4, 5, 6}));
    }
    SECTION("Write past underlying file is truncated by it") {
        SubrangeFile file(mem_file, 5, 5);

        std::vector<u8> buffer{10, 11, 12, 13, 14};
        auto result = file.Write(0, 5, buffer.data());

        REQUIRE(result.Unwrap() == 2);
        REQUIRE(mem_file->GetData() == (std::vector<u8>{0, 1, 2, 3, 4, 10, 11}));
    }
    SECTION("GetSize clamps size to window if larger") {
        SubrangeFile file(mem_file, 1, 5);
        auto result = file.GetSize();
        REQUIRE(result.Unwrap() == 5);
    }
    SECTION("Flush succeeds") {
        SubrangeFile file(mem_file, 0, 5);
        auto result = file.Flush();
        REQUIRE(result.Succeeded());
    }
}

} // namespace Vfs
