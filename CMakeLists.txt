cmake_minimum_required(VERSION 4.0)
project(CPPSmartFileOrganizer)

set(CMAKE_CXX_STANDARD 17)

add_executable(CPPSmartFileOrganizer main.cpp
        src/FileEntry.cpp
        include/FileEntry.hpp)
