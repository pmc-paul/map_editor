cmake_minimum_required(VERSION 2.8.3)
project(map_loader)

find_package(catkin REQUIRED
    COMPONENTS
        roscpp
        visualization_msgs
)

find_package(yaml-cpp)

catkin_package(
    INCLUDE_DIRS
        include
    CATKIN_DEPENDS
        roscpp
        visualization_msgs
)


include_directories(
    include 
    ${catkin_INCLUDE_DIRS}
    ${YAML_CPP_INCLUDE_DIRS}
)

add_executable(load_waypoints src/main.cpp)
add_dependencies(load_waypoints ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(load_waypoints ${YAML_CPP_LIBRARIES} ${catkin_LIBRARIES})