cmake_minimum_required(VERSION 3.21)
project(SleepGuard VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Bluetooth Network)

qt_add_executable(SleepGuard
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/sleepengine.cpp
    src/sleepengine.h
    src/sleepdata.cpp
    src/sleepdata.h
    src/devicecontroller.cpp
    src/devicecontroller.h
)

target_link_libraries(SleepGuard PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Bluetooth
    Qt6::Network
)

if (ANDROID)
    set_property(TARGET SleepGuard PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
                 "${CMAKE_CURRENT_SOURCE_DIR}/android")
endif()
