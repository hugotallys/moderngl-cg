cmake_minimum_required(VERSION 4.0)
project(moderngl_cg)

set(CMAKE_CXX_STANDARD 20)

find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

add_executable(moderngl_cg main.cpp)

target_link_libraries(moderngl_cg glfw GLEW::glew)
