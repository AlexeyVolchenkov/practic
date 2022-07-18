cmake_minimum_required(VERSION 3.12)

include (platform/nk)

# Setting compilation flags
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions")

add_executable (Publisher "src/main.cpp" "src/publisher.cpp" )
add_dependencies (Publisher publisher_edl_files)
target_link_libraries(Publisher 
                      ${mosquitto_CXX_LIB}
                      ${vfs_CLIENT_LIB}
)

# We do not need default VFS entity here, which comes from ${vfs_CLIENT_LIB}
set_target_properties (Publisher PROPERTIES ${vfs_ENTITY}_REPLACEMENT "")
