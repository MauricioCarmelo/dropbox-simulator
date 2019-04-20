cmake_minimum_required(VERSION 3.13)
project(dropbox)

set(CMAKE_CXX_STANDARD 14)

add_executable(box client-side/box_main.cpp client-side/Client.cpp client-side/Client.h client-side/box_main.cpp client-side/Box.cpp client-side/Box.h client-side/SystemDAO.cpp client-side/SystemDAO.h client-side/FileManager.cpp client-side/FileManager.h)
add_executable(warehouse server-side/Warehouse.cpp)