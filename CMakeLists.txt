cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project(RayTracing)


# Extra warnings, treat warnings as error
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

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

# Find Dependencies
find_package(SDL2 REQUIRED)

add_executable(RayTracerApp
    app/main.cpp
)

target_link_libraries(RayTracerApp PRIVATE 
    RayTracer
    SDL2::SDL2
)

set_target_properties(RayTracerApp 
    PROPERTIES 
    CXX_STANDARD 17
)
