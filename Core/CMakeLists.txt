file(GLOB_RECURSE sources "src/*.cpp")
add_executable(raytracer ${sources})
target_include_directories(raytracer PRIVATE src)

if(NOT LIBCONFIG_FOUND)
    pkg_search_module(LIBCONFIG REQUIRED libconfig++)
endif()

if(NOT SDL2_FOUND)
    pkg_search_module(SDL2 REQUIRED sdl2)
endif()

target_link_libraries(raytracer
        PRIVATE ${LIBCONFIG_LIBRARIES}
        PRIVATE ${SDL2_LIBRARIES}
        PRIVATE racist
        PRIVATE math
)

set_target_properties(raytracer
        PROPERTIES PREFIX "${CMAKE_SOURCE_DIR}/"
)
