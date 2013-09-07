cmake_minimum_required (VERSION 2.6)
cmake_policy(SET CMP0015 NEW)
project (POLANDBALL)

if (CMAKE_COMPILER_IS_GNUCC)
    execute_process (COMMAND ${CMAKE_CXX_COMPILER} -dumpversion
                     OUTPUT_VARIABLE GCC_VERSION)
    string (STRIP ${GCC_VERSION} GCC_VERSION)

    if (GCC_VERSION VERSION_GREATER 4.7.0)
        set (CMAKE_CXX_FLAGS "-Wall -pedantic -std=gnu++11 -g")
    else ()
        message(FATAL_ERROR "GCC 4.7.0 or higher is required!")
    endif ()
endif ()

find_package (OpenGL REQUIRED)

include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2_TTF REQUIRED SDL2_ttf)
pkg_search_module(SDL2_IMAGE REQUIRED SDL2_image)
pkg_search_module(JSON_C REQUIRED json-c)
pkg_search_module(GLEW REQUIRED glew)

set (POLANDBALL_DATADIR ${CMAKE_INSTALL_PREFIX}/share/rubik CACHE PATH "Data directory")
set (POLANDBALL_RESOURCES "assets" "fonts" "shaders" "sounds" "textures")
set (POLANDBALL_INCLUDE "src" "src/common" "src/game" "src/math" "src/opengl" "src/signals" "src/utils")

set (MATH_LIBRARY "math")
set (MATH_INCLUDE ${MATH_LIBRARY}/src)
add_subdirectory (${MATH_LIBRARY})

file (GLOB_RECURSE POLANDBALL_SOURCES "src/*.cpp")
include_directories (${POLANDBALL_INCLUDE} ${MATH_INCLUDE} ${PROJECT_BINARY_DIR})

set (POLANDBALL_NAME "PolandBall")
set (POLANDBALL_DESCRIPTION "PolandBall the Gaem")
set (POLANDBALL_VERSION 0.0.1)

configure_file (Config.h.in Config.h @ONLY)

set (POLANDBALL_EXECUTABLE "polandball")
add_executable (${POLANDBALL_EXECUTABLE} ${POLANDBALL_SOURCES})
add_dependencies (${POLANDBALL_EXECUTABLE} ${MATH_LIBRARY})

target_link_libraries (${POLANDBALL_EXECUTABLE} ${OPENGL_LIBRARIES})
target_link_libraries (${POLANDBALL_EXECUTABLE} ${SDL2_LIBRARIES})
target_link_libraries (${POLANDBALL_EXECUTABLE} ${SDL2_TTF_LIBRARIES})
target_link_libraries (${POLANDBALL_EXECUTABLE} ${SDL2_IMAGE_LIBRARIES})
target_link_libraries (${POLANDBALL_EXECUTABLE} ${JSON_C_LIBRARIES})
target_link_libraries (${POLANDBALL_EXECUTABLE} ${GLEW_LIBRARIES})
target_link_libraries (${POLANDBALL_EXECUTABLE} ${MATH_LIBRARY}-shared)

install (TARGETS ${POLANDBALL_EXECUTABLE} DESTINATION bin)
install (DIRECTORY ${POLANDBALL_RESOURCES} DESTINATION ${POLANDBALL_DATADIR})
