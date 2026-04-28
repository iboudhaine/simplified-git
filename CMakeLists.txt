cmake_minimum_required(VERSION 3.16)
project(SimplifiedGit CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Core library: domain logic, no Qt dependency.
add_library(sgit_core STATIC
    src/File.cpp
    src/Repository.cpp
    src/Git.cpp
)
target_include_directories(sgit_core PUBLIC include)

# CLI executable.
add_executable(sgit src/main_cli.cpp)
target_link_libraries(sgit PRIVATE sgit_core)

# Tests.
option(BUILD_TESTS "Build unit tests" ON)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# GUI executable: built only if Qt is found.
option(BUILD_GUI "Build the Qt GUI (sgit-gui)" ON)
if(BUILD_GUI)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_AUTORCC ON)

    find_package(Qt6 QUIET COMPONENTS Widgets)
    if(NOT Qt6_FOUND)
        find_package(Qt5 QUIET COMPONENTS Widgets)
    endif()

    if(Qt6_FOUND OR Qt5_FOUND)
        add_executable(sgit-gui
            gui/main_gui.cpp
            gui/mainwindow.cpp
            gui/mainwindow.h
            gui/mainwindow.ui
        )
        target_link_libraries(sgit-gui PRIVATE sgit_core)
        if(Qt6_FOUND)
            target_link_libraries(sgit-gui PRIVATE Qt6::Widgets)
            message(STATUS "GUI: building with Qt6")
        else()
            target_link_libraries(sgit-gui PRIVATE Qt5::Widgets)
            message(STATUS "GUI: building with Qt5")
        endif()
    else()
        message(STATUS "GUI: Qt not found, sgit-gui will be skipped (install qt6-base-dev or qtbase5-dev)")
    endif()
endif()
