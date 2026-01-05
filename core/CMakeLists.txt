# Core library - contains all the file organization logic

add_library(core STATIC
        src/FileOrganizer.cpp
        src/OperationExecutor.cpp
)

# Make headers available to targets that link against core
target_include_directories(core
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)