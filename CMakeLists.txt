cmake_minimum_required(VERSION 3.13)
project(dropbox)

set(CMAKE_CXX_STANDARD 14)

add_executable(box client-side/Box.cpp client-side/Client.cpp client-side/Client.h)
add_executable(warehouse server-side/Warehouse.cpp)