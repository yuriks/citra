add_library(vfs STATIC
    directory.h
    file.h
    filesystem.h
    host_file.cpp
    host_file.h
    host_filesystem.cpp
    host_filesystem.h
    memory_file.cpp
    memory_file.h
    path.cpp
    path.h
    random_access_adapter.cpp
    random_access_adapter.h
    subrange_file.cpp
    subrange_file.h
    vfs.cpp
    vfs.h
)

create_target_directory_groups(vfs)

target_link_libraries(vfs PUBLIC common core)
target_link_libraries(vfs PUBLIC Boost::boost)
