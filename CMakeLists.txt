cmake_minimum_required(VERSION 3.28)
project(stream_example)

set(CMAKE_CXX_STANDARD 23)

add_library(server_lib STATIC server.h)
set_target_properties(server_lib PROPERTIES LINKER_LANGUAGE CXX)

add_executable(stream_example main.cpp)

target_link_libraries(stream_example server_lib)

# testing
enable_testing()
add_subdirectory(tests)
