cmake_minimum_required(VERSION 3.26)
project(Notepad)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)
qt_standard_project_setup()

add_executable(Notepad 
        main.cpp
        Front.cpp
        Front.h
        Back.cpp
        Back.h
        header.h
)

target_link_libraries(Notepad
        Qt::Core
        Qt::Gui
        Qt::Widgets
)
