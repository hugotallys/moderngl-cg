cmake_minimum_required(VERSION 4.0)
project(moderngl_cg)

set(CMAKE_CXX_STANDARD 20)

find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

# Modern OpenGL - Triangle
add_executable(moderngl_cg modern-triangle/main.cpp)
target_link_libraries(moderngl_cg glfw GLEW::glew)

# Classic OpenGL - Pinwheel
add_executable(pinwheel pinwheel/main.c)
target_link_libraries(pinwheel PRIVATE "-framework GLUT" "-framework OpenGL")
target_compile_definitions(pinwheel PRIVATE GL_SILENCE_DEPRECATION)

# Classic OpenGL - Soccer (with modular classes)
add_executable(soccer
    soccer/main.cpp
    soccer/SoccerField.cpp
    soccer/Ball.cpp
    soccer/Player.cpp
    soccer/Camera.cpp
)
target_link_libraries(soccer PRIVATE "-framework GLUT" "-framework OpenGL")
target_compile_definitions(soccer PRIVATE GL_SILENCE_DEPRECATION)
