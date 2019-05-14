cmake_minimum_required(VERSION 3.13)
project(dropbox)

set(CMAKE_CXX_STANDARD 14)

add_executable(box client-side/source/box_main.cpp client-side/source/Client.cpp utils/include/Instruction.h utils/source/Instruction.cpp client-side/include/Client.h client-side/source/box_main.cpp client-side/source/Box.cpp client-side/include/Box.h client-side/source/SystemDAO.cpp client-side/include/SystemDAO.h utils/source/FileManager.cpp utils/include/FileManager.h utils/source/File.cpp utils/include/File.h utils/source/Utility.cpp utils/include/Utility.h utils/include/masterInclude.h)
add_executable(warehouse server-side/source/warehouse_main.cpp utils/include/Instruction.h utils/source/Instruction.cpp server-side/source/Server.cpp server-side/include/Server.h client-side/source/Client.cpp client-side/include/Client.h utils/source/File.cpp utils/include/File.h server-side/source/warehouse_main.cpp utils/source/FileManager.cpp utils/include/FileManager.h server-side/source/Warehouse.cpp server-side/include/Warehouse.h utils/source/Utility.cpp utils/include/Utility.h server-side/source/SystemDAOWarehouse.cpp server-side/include/SystemDAOWarehouse.h utils/include/masterInclude.h)
# create a new library
add_library(TEST_FILES client-side/source/box_main.cpp utils/include/Instruction.h utils/source/Instruction.cpp client-side/source/Client.cpp client-side/include/Client.h client-side/source/box_main.cpp client-side/source/Box.cpp client-side/include/Box.h client-side/source/SystemDAO.cpp client-side/include/SystemDAO.h utils/source/FileManager.cpp utils/include/FileManager.h utils/source/File.cpp utils/include/File.h utils/source/Utility.cpp utils/include/Utility.h utils/include/masterInclude.h)

add_executable(tests tests/tests_user_interface.cpp utils/include/masterInclude.h)
# link libraries
target_link_libraries(tests TEST_FILES)

set(CMAKE_CXX_FLAGS -pthread)

#find_package(Catch2 REQUIRED)
#target_link_libraries(tests Catch2::Catch2)

