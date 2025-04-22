cmake_minimum_required(VERSION 3.10)

set(CMAKE_GENERATOR
    "Unix Makefiles"
    CACHE STRING "generator"
)

project(Raytracer
    VERSION 1.0.0
    DESCRIPTION "Advance raytracer, web-compatible"
    LANGUAGES CXX
)

file(GLOB_RECURSE sources  "src/*.cpp")

add_executable(raytracer ${sources})
set_target_properties(raytracer PROPERTIES PREFIX "${CMAKE_SOURCE_DIR}/")
