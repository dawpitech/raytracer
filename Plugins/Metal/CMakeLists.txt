file(GLOB_RECURSE src "src/*.cpp")
add_library(metal_mat SHARED ${src})

if(NOT LIBCONFIG_FOUND)
    pkg_search_module(LIBCONFIG REQUIRED libconfig++)
endif()

target_link_libraries(metal_mat
        PRIVATE ${LIBCONFIG_LIBRARIES}
        PRIVATE racist
)

set_target_properties(metal_mat
        PROPERTIES PREFIX "${CMAKE_SOURCE_DIR}/plugins/"
)
