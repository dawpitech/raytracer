cmake_minimum_required(VERSION 3.10)

project(Raytracer
        VERSION 1.0.0
        DESCRIPTION "Advance raytracer, web-compatible"
        LANGUAGES CXX
)

# Detect if we're using Emscripten
if(EMSCRIPTEN)
    set(WEB_BUILD TRUE)
    message(STATUS "Configuring for Emscripten web build")
else()
    set(NATIVE_BUILD TRUE)
    message(STATUS "Configuring for native build")

    # Only set generator for native builds
    set(CMAKE_GENERATOR
            "Unix Makefiles"
            CACHE STRING "generator"
    )
endif()

file(GLOB_RECURSE sources "src/*.cpp")
add_executable(raytracer ${sources})

# Include directories common to both builds
target_include_directories(raytracer PRIVATE src)

# Platform-specific configuration
if(WEB_BUILD)
    # Web build with Emscripten

    # Set HTML output
    set(CMAKE_EXECUTABLE_SUFFIX ".html")

    # Add compiler definition to handle SDL include differences
    target_compile_definitions(raytracer PRIVATE EMSCRIPTEN_BUILD=1)

    # Set Emscripten-specific compile and link flags for SDL
    set(EM_FLAGS "")
    list(APPEND EM_FLAGS "-sUSE_SDL=2")
    list(APPEND EM_FLAGS "-sUSE_SDL_TTF=2") # Add if you need TTF
    list(APPEND EM_FLAGS "-sUSE_SDL_IMAGE=2") # Add if you need IMAGE
    list(APPEND EM_FLAGS "-sSDL2_IMAGE_FORMATS='[\"png\",\"jpg\"]'") # Optional format specification
    list(APPEND EM_FLAGS "-sWASM=1")
    list(APPEND EM_FLAGS "-sALLOW_MEMORY_GROWTH=1")
    list(APPEND EM_FLAGS "-sEXPORTED_RUNTIME_METHODS=['ccall','cwrap']")
    list(APPEND EM_FLAGS "-sTOTAL_MEMORY=67108864")
    list(APPEND EM_FLAGS "-sSTACK_SIZE=5120000")

    # Convert list to string of flags
    string(REPLACE ";" " " EM_FLAGS_STR "${EM_FLAGS}")

    # Apply flags to both compile and link stages
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EM_FLAGS_STR}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${EM_FLAGS_STR}")

    # Only preload assets if directory exists
    set(ASSETS_DIR "${CMAKE_SOURCE_DIR}/assets")
    if(EXISTS ${ASSETS_DIR} AND IS_DIRECTORY ${ASSETS_DIR})
        message(STATUS "Assets directory found, adding preload flag")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --preload-file ${ASSETS_DIR}@/assets")
    else()
        message(STATUS "Assets directory not found at ${ASSETS_DIR}, skipping preload")
    endif()

    # Output directory for web build
    set_target_properties(raytracer PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/web"
    )

else()
    # Native build

    # Find SDL2 packages
    include(FindPkgConfig)
    pkg_search_module(SDL2 REQUIRED sdl2)
    pkg_search_module(SDL2_TTF REQUIRED SDL2_ttf)
    pkg_search_module(SDL2_IMAGE REQUIRED SDL2_image)

    # Link with native SDL2 libraries
    target_link_libraries(raytracer PRIVATE ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})

    # Add SDL2 include dirs
    target_include_directories(raytracer PRIVATE ${SDL2_IMAGE_INCLUDE_DIRS})

    # Output properties for native build
    set_target_properties(raytracer PROPERTIES PREFIX "${CMAKE_SOURCE_DIR}/")
endif()
