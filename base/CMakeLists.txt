project(irc-server-base)
cmake_minimum_required(VERSION 3.5)

add_library(base fd_stream_buffer.cpp error.cpp)
