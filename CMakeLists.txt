cmake_minimum_required(VERSION 4.0)
project(moderngl_cg)

set(CMAKE_CXX_STANDARD 20)

find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

add_executable(moderngl_cg main.cpp)

target_link_libraries(moderngl_cg glfw GLEW::glew)

# Target for the C program (test.c)
add_executable(test test.c)
# Use this simpler syntax as it is often sufficient
target_link_libraries(test PRIVATE "-framework GLUT" "-framework OpenGL")
# Add the preprocessor definition to silence deprecation warnings
target_compile_definitions(test PRIVATE GL_SILENCE_DEPRECATION)

# Target for the C program (test.c)
add_executable(catavento catavento.c)
# Use this simpler syntax as it is often sufficient
target_link_libraries(catavento PRIVATE "-framework GLUT" "-framework OpenGL")
# Add the preprocessor definition to silence deprecation warnings
target_compile_definitions(catavento PRIVATE GL_SILENCE_DEPRECATION)

# Target for the C program (test.c)
add_executable(futebol futebol.c)
# Use this simpler syntax as it is often sufficient
target_link_libraries(futebol PRIVATE "-framework GLUT" "-framework OpenGL")
# Add the preprocessor definition to silence deprecation warnings
target_compile_definitions(futebol PRIVATE GL_SILENCE_DEPRECATION)
