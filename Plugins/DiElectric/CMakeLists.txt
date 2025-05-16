file(GLOB_RECURSE src "src/*.cpp")
add_library(dielec_mat SHARED ${src})

if(NOT LIBCONFIG_FOUND)
    pkg_search_module(LIBCONFIG REQUIRED libconfig++)
endif()

target_link_libraries(dielec_mat
        PRIVATE ${LIBCONFIG_LIBRARIES}
        PRIVATE racist
)

set_target_properties(dielec_mat
        PROPERTIES PREFIX "${CMAKE_SOURCE_DIR}/plugins/"
)
