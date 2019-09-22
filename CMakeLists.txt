cmake_minimum_required(VERSION 3.1)

project(ProtobufGUI)

set(CMAKE_CXX_STANDARD 17)

include(${CMAKE_SOURCE_DIR}/conan.cmake)
conan_cmake_run(
    CONANFILE conanfile.txt
    BASIC_SETUP
    BUILD missing)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
find_package(Protobuf REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(SRC     src/main.cpp
            src/MainWindow.cpp
            src/ProtoManager.cpp
            src/ProtobufModel.cpp
            src/prototreeitem.cpp
            src/fielddelegate.cpp
            src/numericprotoitem.cpp
            src/stringprotoitem.cpp
            src/floatprotoitem.cpp
            src/boolprotoitem.cpp
            src/enumprotoitem.cpp
            src/repeatedprotoitem.cpp)

set(HEADERS src/MainWindow.h
            src/ProtoManager.h
            src/ProtobufModel.h
            src/prototreeitem.h
            src/fielddelegate.h
            src/numericprotoitem.h
            src/stringprotoitem.h
            src/floatprotoitem.h
            src/boolprotoitem.h
            src/enumprotoitem.h
            src/repeatedprotoitem.h)

set(RSC rsc/rsc.qrc)
qt5_wrap_ui(UI ui/mainwindow.ui)


add_executable(${PROJECT_NAME} ${SRC} ${HEADERS} ${RSC} ${UI})
include_directories(${PROJECT_NAME} ${Protobuf_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Widgets ${Protobuf_PROTOC_LIBRARIES} ${Protobuf_LIBRARIES})
