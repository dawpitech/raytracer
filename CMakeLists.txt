cmake_minimum_required(VERSION 3.10)

project(Raytracer
        VERSION 1.0.0
        DESCRIPTION "Advance raytracer, web-compatible"
        LANGUAGES CXX
)

set(SANITIZER "NONE" CACHE STRING "Enable sanitizers")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -g3 -O0")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3 -O0")
endif()

if (SANITIZER STREQUAL "MEMORY")
    add_link_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
    add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
elseif (SANITIZER STREQUAL "THREAD")
    add_link_options(-fsanitize=thread -fno-omit-frame-pointer)
    add_compile_options(-fsanitize=thread -fno-omit-frame-pointer)
endif ()

include(FindPkgConfig)

add_subdirectory(lib/MATH)
add_subdirectory(lib/RACIST)

add_subdirectory(Core)

# Primitives plugins
file(MAKE_DIRECTORY plugins)
add_subdirectory(Plugins/Sphere)

# Materials plugins
add_subdirectory(Plugins/Diffuse)
add_subdirectory(Plugins/Metal)
add_subdirectory(Plugins/DirectionalLight)
add_subdirectory(Plugins/Plane)
