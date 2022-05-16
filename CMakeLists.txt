cmake_minimum_required(VERSION 2.8.3)

project(map_editor)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

file(GLOB QT_RESOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} resources/*.qrc)
file(GLOB_RECURSE QT_MOC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FOLLOW_SYMLINKS include/*.h)

qt5_add_resources(QT_RESOURCES_CPP ${QT_RESOURCES})
qt5_wrap_cpp(QT_MOC_H ${QT_MOC})

file(GLOB_RECURSE QT_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FOLLOW_SYMLINKS src/*.cpp)

add_executable(${PROJECT_NAME} ${QT_SOURCES} ${QT_RESOURCES_CPP} ${QT_MOC_H})

target_include_directories (${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/)
target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::Core Qt5::Gui ${catkin_LIBRARIES})