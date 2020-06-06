aux_source_directory(. SRCS)
aux_source_directory(./base SRCS_BASE)

add_executable(WebServer_self ${SRCS} ${SRCS_BASE})

find_package(Threads REQUIRED)
target_link_libraries(WebServer_self Threads::Threads)