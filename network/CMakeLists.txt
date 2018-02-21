project(irc-server-network)
cmake_minimum_required(VERSION 3.5)

add_library(network poll_array.cpp server_poll_socket.cpp)
