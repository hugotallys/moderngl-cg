cmake_minimum_required(VERSION 3.10)
project(moderngl_cg)

set(CMAKE_CXX_STANDARD 20)

find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

# Modern OpenGL - Triangle
add_executable(moderngl_cg modern-triangle/main.cpp)
target_link_libraries(moderngl_cg glfw GLEW::glew)
set_target_properties(moderngl_cg PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")

# Copy shader files to build directory
add_custom_command(TARGET moderngl_cg POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/modern-triangle
    $<TARGET_FILE_DIR:moderngl_cg>/modern-triangle
)

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

# Add this after your soccer executable definition
add_custom_command(TARGET soccer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/soccer/assets
    $<TARGET_FILE_DIR:soccer>/assets
)

# Add this after your soccer executable definition
set_target_properties(soccer PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")


target_link_libraries(soccer PRIVATE "-framework GLUT" "-framework OpenGL -framework CoreAudio -framework CoreFoundation")
target_compile_definitions(soccer PRIVATE GL_SILENCE_DEPRECATION)
