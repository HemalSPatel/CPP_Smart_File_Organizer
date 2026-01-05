# Command-line interface executable

add_executable(SmartFileOrganizerCLI main.cpp)

# Link against the core library
target_link_libraries(SmartFileOrganizerCLI PRIVATE core)