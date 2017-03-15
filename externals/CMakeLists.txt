# Xbyak
if (ARCHITECTURE_x86_64)
    add_library(xbyak INTERFACE)
    target_include_directories(xbyak INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/xbyak/xbyak)
    if (NOT MSVC)
        target_compile_options(xbyak INTERFACE -fno-operator-names)
    endif()
endif()

add_subdirectory(cryptopp)

# while spdlog has a cmakelists file, it pollutes our cmake if we include it directly
# thankfully its header only so we can just include the directory
add_library(spdlog INTERFACE)
target_include_directories(spdlog INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/spdlog/include)