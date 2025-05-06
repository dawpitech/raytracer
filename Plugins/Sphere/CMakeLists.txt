file(GLOB_RECURSE src "src/*.cpp")
add_library(sphere_prim SHARED ${src})

if(NOT LIBCONFIG_FOUND)
    pkg_search_module(LIBCONFIG REQUIRED libconfig++)
endif()

target_link_libraries(sphere_prim
        PRIVATE ${LIBCONFIG_LIBRARIES}
        PRIVATE racist
)

set_target_properties(sphere_prim
        PROPERTIES PREFIX "${CMAKE_SOURCE_DIR}/plugins/"
)
