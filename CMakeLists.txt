project(irc-server)
cmake_minimum_required(VERSION 3.5)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "lib")

set(CMAKE_CXX_FLAGS "-g -std=gnu++14 -Wall -Werror -fsanitize=address,undefined")

include_directories(".")

add_subdirectory(base)
add_subdirectory(network)

add_executable(main main.cpp server.cpp) 

target_link_libraries(main base network)
