cmake_minimum_required(VERSION 3.1)

project(ProtobufGUI)

message("here ----> ${CMAKE_SOURCE_DIR}")

include(${CMAKE_SOURCE_DIR}/conan.cmake)
conan_cmake_run(
    CONANFILE conanfile.txt
    BASIC_SETUP
    BUILD missing)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

find_package(Qt5 COMPONENTS Core Quick REQUIRED)
find_package(Protobuf REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(SRC     src/main.cpp
            src/MainWindow.cpp
            src/WindowController.cpp
            src/ProtobufModel.cpp
            src/prototreeitem.cpp)

set(HEADERS src/MainWindow.h
            src/WindowController.h
            src/ProtobufModel.h
            src/prototreeitem.h)

set(QML rsc/qml.qrc)

#link_directories()
message("here ----> ${Protobuf_PROTOC_LIBRARY}")

add_executable(${PROJECT_NAME} ${SRC} ${HEADERS} ${QML})
include_directories(${PROJECT_NAME} ${Protobuf_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Quick ${Protobuf_PROTOC_LIBRARIES} ${Protobuf_LIBRARIES})
