cmake_minimum_required(VERSION 3.13)
project(dropbox)

set(CMAKE_CXX_STANDARD 14)

add_executable(box client-side/main.cpp client-side/Box.cpp)
add_executable(warehouse server-side/main2.cpp server-side/Warehouse.cpp)