cmake_minimum_required(VERSION 3.20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

project(VisualCell)

add_subdirectory(./src)
add_subdirectory(./shaders)
add_subdirectory(./images)