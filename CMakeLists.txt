cmake_minimum_required(VERSION 3.1)

project(ProtobufGUI)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

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
            src/mainwindow.cpp
            src/prototypedialog.cpp
            src/protomanager.cpp
            src/protobufmodel.cpp
            src/fielddelegate.cpp
            src/prototreeerror.cpp
            src/protoview.cpp
            src/protoerrordialog.cpp
            src/tree_items/prototreeitem.cpp
            src/tree_items/numericprotoitem.cpp
            src/tree_items/stringprotoitem.cpp
            src/tree_items/floatprotoitem.cpp
            src/tree_items/boolprotoitem.cpp
            src/tree_items/enumprotoitem.cpp
            src/tree_items/repeatedprotoitem.cpp
            src/tree_items/messageprotoitem.cpp
            src/tree_items/rootprotoitem.cpp
            src/tree_items/bytestprotoitem.cpp
            src/tree_items/protofieldwrapper.cpp)

set(HEADERS src/mainwindow.h
            src/prototypedialog.h
            src/protomanager.h
            src/protobufmodel.h
            src/fielddelegate.h
            src/prototreeerror.h
            src/protoview.h
            src/protoerrordialog.h
            src/tree_items/prototreeitem.h
            src/tree_items/numericprotoitem.h
            src/tree_items/stringprotoitem.h
            src/tree_items/floatprotoitem.h
            src/tree_items/boolprotoitem.h
            src/tree_items/enumprotoitem.h
            src/tree_items/repeatedprotoitem.h
            src/tree_items/messageprotoitem.h
            src/tree_items/rootprotoitem.h
            src/tree_items/bytestprotoitem.h
            src/tree_items/protofieldwrapper.h)

set(RSC rsc/rsc.qrc)
qt5_wrap_ui(UI ui/mainwindow.ui
               ui/prototypedialog.ui
               ui/protoerrordialog.ui
               ui/protoview.ui)


add_executable(${PROJECT_NAME} ${SRC} ${HEADERS} ${RSC} ${UI})
include_directories(${PROJECT_NAME} ${Protobuf_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Widgets ${Protobuf_PROTOC_LIBRARIES} ${Protobuf_LIBRARIES})
