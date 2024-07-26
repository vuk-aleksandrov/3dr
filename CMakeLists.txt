cmake_minimum_required(VERSION 3.10)

set (CMAKE_CXX_STANDARD 23)

# set the project name
project(3dr)

# add the executable
add_executable(${PROJECT_NAME} src/main.cpp)

# vendor setup
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory("libs/glfw")
add_subdirectory("libs/glad")
target_link_libraries(${PROJECT_NAME} "glfw" "glad")