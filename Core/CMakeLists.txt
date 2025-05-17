file(GLOB_RECURSE sources "src/*.cpp")
add_executable(raytracer ${sources})
target_include_directories(raytracer PRIVATE src)

if(NOT LIBCONFIG_FOUND)
    pkg_search_module(LIBCONFIG REQUIRED libconfig++)
endif()

if(NOT SDL2_FOUND)
    pkg_search_module(SDL2 REQUIRED sdl2)
endif()

if(NOT SFML_FOUND)
    pkg_search_module(SFML REQUIRED sfml-all)
endif()

target_link_libraries(raytracer
        PRIVATE ${LIBCONFIG_LIBRARIES}
        PRIVATE ${SDL2_LIBRARIES}
        PRIVATE sfml-graphics sfml-window sfml-system
        PRIVATE racist
        PRIVATE math
)

set_target_properties(raytracer
        PROPERTIES PREFIX "${CMAKE_SOURCE_DIR}/"
)
