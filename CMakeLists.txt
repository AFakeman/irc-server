project(irc-server)
cmake_minimum_required(VERSION 3.5)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin")

set(CMAKE_CXX_FLAGS "-g -std=c++14 -Wall -Werror")

add_executable(main main.cpp poll_array.cpp poll_server_socket.cpp server.cpp base/error.cpp) 
