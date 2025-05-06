cmake_minimum_required(VERSION 3.10)

project(racist
        VERSION 1.0.0
        DESCRIPTION "Raytracer Advanced Complementary Interfaces for Solids and Transforms"
        LANGUAGES CXX
)

#pkg_search_module(LIBCONFIG REQUIRED libconfig)

add_library(racist INTERFACE)

target_include_directories(racist INTERFACE include)
target_link_libraries(racist
        INTERFACE ${LIBCONFIG_LIBRARIES}
        INTERFACE math
)

add_subdirectory(dummy)
