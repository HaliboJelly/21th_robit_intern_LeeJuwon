cmake_minimum_required(VERSION 3.19)

project(kakaotalk LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS ui)

find_package(Qt6 COMPONENTS Widgets QUIET)
find_package(Qt6 REQUIRED COMPONENTS Widgets Network)
if(Qt6_FOUND)
    set(QT_LIB Qt6::Widgets Qt6::Network)
else()
    find_package(Qt5 REQUIRED COMPONENTS Widgets)
    set(QT_LIB Qt5::Widgets Qt5::Network)
endif()

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/mainwindow.cpp
    src/keypad.cpp
    include/mainwindow.h
    include/keypad.h
    ui/mainwindow.ui
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
    include
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    Qt6::Widgets
    Qt6::Network
)
