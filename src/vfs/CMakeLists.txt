add_library(vfs STATIC
    file.h
    vfs.h
)

create_target_directory_groups(vfs)

target_link_libraries(vfs PUBLIC common core)
