file(GLOB_RECURSE src "src/*.cpp")
add_library(triangle_prim SHARED ${src})

if(NOT LIBCONFIG_FOUND)
    pkg_search_module(LIBCONFIG REQUIRED libconfig++)
endif()

target_link_libraries(triangle_prim
        PRIVATE ${LIBCONFIG_LIBRARIES}
        PRIVATE racist
)

set_target_properties(triangle_prim
        PROPERTIES PREFIX "${CMAKE_SOURCE_DIR}/plugins/"
)
