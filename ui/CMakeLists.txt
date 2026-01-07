set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Quick QuickControls2 QuickDialogs2)

qt_add_executable(SmartFileOrganizerUI main.cpp)

qt_add_qml_module(SmartFileOrganizerUI
        URI SmartFileOrganizer
        VERSION 1.0
        QML_FILES
        qml/Main.qml
)

target_link_libraries(SmartFileOrganizerUI PRIVATE
        core
        Qt6::Quick
        Qt6::QuickControls2
        Qt6::QuickDialogs2
)