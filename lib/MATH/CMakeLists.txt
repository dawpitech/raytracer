cmake_minimum_required(VERSION 3.10)

project(math
        VERSION 1.0.0
        DESCRIPTION "MATH moi cette lib"
        LANGUAGES CXX
)

add_library(math INTERFACE)
target_include_directories(math INTERFACE include)

add_subdirectory(dummy)
