cmake_minimum_required(VERSION 3.16)
project(SmartFileOrganizer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Help CMake find Qt
list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew/Cellar/qt/6.9.3")

# Build core library first
add_subdirectory(core)

# Build CLI
option(BUILD_CLI "Build command-line interface" ON)
if(BUILD_CLI)
    add_subdirectory(cli)
endif()

# Build Qt UI (optional - disable if Qt not installed)
option(BUILD_UI "Build Qt user interface" ON)
if(BUILD_UI)
    find_package(Qt6 REQUIRED COMPONENTS Quick Widgets)
    add_subdirectory(ui)
endif()