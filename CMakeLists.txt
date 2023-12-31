cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(RayTracing)


# Extra warnings, treat warnings as error

add_compile_options(-Wall -Wextra -pedantic -Werror)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")

add_library(RayTracer
    src/renderer.cpp
    src/scene_node.cpp
)

set_target_properties(RayTracer 
    PROPERTIES 
    CXX_STANDARD 17
)
target_include_directories(
    RayTracer
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE src
)

#target_link_libraries(RayTracer PRIVATE OpenMP::OpenMP_CXX)

# Find Dependencies
find_package(SDL2 REQUIRED)

add_executable(Example01
    example/example_01.cpp
)

target_link_libraries(Example01 PRIVATE 
    RayTracer
    SDL2::SDL2
)

set_target_properties(Example01 
    PROPERTIES 
    CXX_STANDARD 17
)
