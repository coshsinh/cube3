cmake_minimum_required(VERSION 3.15)

add_compile_options(-Wall -Wextra -pedantic)

project(cube3 C)

find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)

set(CMAKE_C_STANDARD 11)

file(GLOB SRC_FILES src/*.c)
add_executable(cube3 ${SRC_FILES})

target_include_directories(cube3 PRIVATE inc)

target_link_libraries(cube3 glfw)
target_link_libraries(cube3 OpenGL::GL)
