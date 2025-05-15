file(GLOB_RECURSE src "src/*.cpp")
add_library(cone_prim SHARED ${src})

if(NOT LIBCONFIG_FOUND)
    pkg_search_module(LIBCONFIG REQUIRED libconfig++)
endif()

target_link_libraries(cone_prim
        PRIVATE ${LIBCONFIG_LIBRARIES}
        PRIVATE racist
)

set_target_properties(cone_prim
        PROPERTIES PREFIX "${CMAKE_SOURCE_DIR}/plugins/"
)
