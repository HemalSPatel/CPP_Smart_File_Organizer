# Qt user interface executable

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable(SmartFileOrganizerUI main.cpp)

# Link against core library and Qt
target_link_libraries(SmartFileOrganizerUI PRIVATE
        core
        Qt6::Quick
        Qt6::Widgets
)