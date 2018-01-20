add_library(vfs STATIC
    file.h
    host_file.cpp
    host_file.h
    memory_file.cpp
    memory_file.h
    random_access_adapter.cpp
    random_access_adapter.h
    subrange_file.cpp
    subrange_file.h
    vfs.h
)

create_target_directory_groups(vfs)

target_link_libraries(vfs PUBLIC common core)
