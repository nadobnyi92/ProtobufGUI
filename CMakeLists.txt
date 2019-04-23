cmake_minimum_required(VERSION 3.1)

project(ProtobufGUI)

find_package(Qt5 COMPONENTS Core Quick REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(SRC     src/main.cpp
            src/MainWindow.cpp)

set(HEADERS src/MainWindow.h)

set(QML rsc/qml.qrc)

add_executable(${PROJECT_NAME} ${SRC} ${HEADERS} ${QML})
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Quick)
