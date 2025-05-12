file(GLOB_RECURSE src "src/*.cpp")
add_library(directional_light SHARED ${src})

if(NOT LIBCONFIG_FOUND)
    pkg_search_module(LIBCONFIG REQUIRED libconfig++)
endif()

target_link_libraries(directional_light
        PRIVATE ${LIBCONFIG_LIBRARIES}
        PRIVATE racist
)

set_target_properties(directional_light
        PROPERTIES PREFIX "${CMAKE_SOURCE_DIR}/plugins/"
)
