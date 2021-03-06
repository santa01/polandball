#
# Copyright (c) 2013 Pavlo Lavrenenko
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#

cmake_minimum_required (VERSION 2.6)
project (POLANDBALL)

if (CMAKE_COMPILER_IS_GNUCC)
    execute_process (COMMAND ${CMAKE_CXX_COMPILER} -dumpversion
                     OUTPUT_VARIABLE GCC_VERSION)
    string (STRIP ${GCC_VERSION} GCC_VERSION)
endif ()

if (DEFINED GCC_VERSION AND GCC_VERSION VERSION_GREATER 4.7.0)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -std=gnu++11")
    set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
    set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
    set (CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} -Os")
    set (CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -g -O3")
else ()
    message (FATAL_ERROR "GCC 4.7.0 or higher is required!")
endif ()

find_package (OpenGL REQUIRED)
find_package (GLEW REQUIRED)

include (FindPkgConfig)
pkg_search_module (SDL2 REQUIRED sdl2)
pkg_search_module (SDL2_TTF REQUIRED SDL2_ttf)
pkg_search_module (SDL2_IMAGE REQUIRED SDL2_image)
pkg_search_module (JSON_C REQUIRED json-c)
pkg_search_module (MATH REQUIRED math)

include (CheckIncludeFileCXX)
CHECK_INCLUDE_FILE_CXX (Signals.h HAVE_SIGNALS_H)
if (NOT HAVE_SIGNALS_H)
    message (FATAL_ERROR "Header Signals.h not found!")
endif ()

set (POLANDBALL_DATADIR ${CMAKE_INSTALL_PREFIX}/share/rubik
     CACHE PATH "Data directory")
set (POLANDBALL_RESOURCES assets fonts shaders sounds textures)
set (POLANDBALL_INCLUDE src src/common src/game src/opengl src/utils)

file (GLOB_RECURSE POLANDBALL_SOURCES src/*.cpp)
include_directories (${POLANDBALL_INCLUDE} ${MATH_INCLUDE_DIRS}
                     ${PROJECT_BINARY_DIR})

set (POLANDBALL_NAME PolandBall)
set (POLANDBALL_DESCRIPTION "PolandBall the Gaem")
set (POLANDBALL_VERSION 0.0.1)

configure_file (Config.h.in Config.h @ONLY)

set (POLANDBALL_EXECUTABLE polandball)
add_executable (${POLANDBALL_EXECUTABLE} ${POLANDBALL_SOURCES})

target_link_libraries (${POLANDBALL_EXECUTABLE} ${OPENGL_LIBRARIES})
target_link_libraries (${POLANDBALL_EXECUTABLE} ${SDL2_LIBRARIES})
target_link_libraries (${POLANDBALL_EXECUTABLE} ${SDL2_TTF_LIBRARIES})
target_link_libraries (${POLANDBALL_EXECUTABLE} ${SDL2_IMAGE_LIBRARIES})
target_link_libraries (${POLANDBALL_EXECUTABLE} ${JSON_C_LIBRARIES})
target_link_libraries (${POLANDBALL_EXECUTABLE} ${GLEW_LIBRARIES})
target_link_libraries (${POLANDBALL_EXECUTABLE} ${MATH_LIBRARIES})

install (TARGETS ${POLANDBALL_EXECUTABLE} DESTINATION bin)
install (DIRECTORY ${POLANDBALL_RESOURCES} DESTINATION ${POLANDBALL_DATADIR})
