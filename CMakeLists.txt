cmake_minimum_required(VERSION 3.13)
project(dropbox)

set(CMAKE_CXX_STANDARD 14)

add_executable(box client-side/source/box_main.cpp client-side/source/Client.cpp client-side/include/Client.h client-side/source/box_main.cpp client-side/source/Box.cpp client-side/include/Box.h client-side/source/SystemDAO.cpp client-side/include/SystemDAO.h client-side/source/FileManager.cpp client-side/include/FileManager.h client-side/source/File.cpp client-side/include/File.h)
add_executable(warehouse server-side/Warehouse.cpp server-side/Server.cpp server-side/Server.h client-side/source/Client.cpp client-side/include/Client.h client-side/source/File.cpp client-side/include/File.h)