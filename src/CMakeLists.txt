# Enable modules to include each other's files
include_directories(.)
# Include fmtlib so it can be used across the application for logging
include_directories(../externals/fmt)

add_subdirectory(common)
add_subdirectory(core)
add_subdirectory(video_core)
add_subdirectory(audio_core)
add_subdirectory(input_common)
add_subdirectory(tests)
if (ENABLE_SDL2)
    add_subdirectory(citra)
endif()
if (ENABLE_QT)
    add_subdirectory(citra_qt)
endif()
