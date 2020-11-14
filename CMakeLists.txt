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

cmake_minimum_required (VERSION 3.12)
project (POLANDBALL)

find_path (GRAPHENE_INCLUDE_DIRS NAMES GrapheneApi.h HINTS ENV GRAPHENE_DIR PATH_SUFFIXES include)
find_library (GRAPHENE_LIBRARIES NAMES graphene graphene-shared HINTS ENV GRAPHENE_DIR PATH_SUFFIXES lib)
if (NOT GRAPHENE_INCLUDE_DIRS OR NOT GRAPHENE_LIBRARIES)
    message (FATAL_ERROR "Could NOT find Graphene")
endif ()

find_path (MATH_INCLUDE_DIRS NAMES MathApi.h HINTS ENV MATH_DIR PATH_SUFFIXES include)
find_library (MATH_LIBRARIES NAMES math math-shared HINTS ENV MATH_DIR PATH_SUFFIXES lib)
if (NOT MATH_INCLUDE_DIRS OR NOT MATH_LIBRARIES)
    message (FATAL_ERROR "Could NOT find Math")
endif ()

find_path (SIGNALS_INCLUDE_DIRS NAMES Signals.h HINTS ENV SIGNALS_DIR PATH_SUFFIXES include)
if (NOT SIGNALS_INCLUDE_DIRS)
    message (FATAL_ERROR "Could NOT find Signals")
endif ()

find_path (JSON_C_INCLUDE_DIRS NAMES json-c/json.h HINTS ENV JSON_C_DIR PATH_SUFFIXES include)
find_library (JSON_C_LIBRARIES NAMES json-c HINTS ENV JSON_C_DIR PATH_SUFFIXES lib)
if (NOT JSON_C_INCLUDE_DIRS OR NOT JSON_C_LIBRARIES)
    message (FATAL_ERROR "Could NOT find json-c")
endif ()

set (POLANDBALL_NAME PolandBall)
set (POLANDBALL_DESCRIPTION "PolandBall the Gaem")
set (POLANDBALL_VERSION 0.0.1)

set (POLANDBALL_DATADIR ${CMAKE_INSTALL_PREFIX}/$<IF:$<BOOL:UNIX>,share/polandball,data> CACHE PATH "Data directory")

set (POLANDBALL_EXECUTABLE polandball)
set (POLANDBALL_RESOURCE_DIRS assets fonts shaders sounds textures)

# file (GLOB_RECURSE POLANDBALL_SOURCES src/*.cpp)
set (POLANDBALL_SOURCES src/main.cpp
                        src/ArgumentParser.cpp
                        src/PolandBall.cpp
                        # src/game/Collider.cpp
                        src/game/Shearable.cpp
                        src/game/Replicable.cpp
                        src/game/BaseEntity.cpp
                        src/game/EntityFactory.cpp
                        src/game/SpriteEntity.cpp
                        src/game/Pack.cpp
                        src/game/Player.cpp
                        src/game/Weapon.cpp
                        # src/game/LineEntity.cpp
                        # src/game/ShotTrace.cpp
                        # src/opengl/Line.cpp
                        src/utils/ResourceCache.cpp
)
# include_directories (src src/game src/opengl src/utils ${PROJECT_BINARY_DIR} ${GRAPHENE_INCLUDE_DIRS} ${MATH_INCLUDE_DIRS} ${SIGNALS_INCLUDE_DIRS} ${JSON_C_INCLUDE_DIRS})
include_directories (src src/game src/utils ${PROJECT_BINARY_DIR} ${GRAPHENE_INCLUDE_DIRS} ${MATH_INCLUDE_DIRS} ${SIGNALS_INCLUDE_DIRS} ${JSON_C_INCLUDE_DIRS})

add_executable (${POLANDBALL_EXECUTABLE} ${POLANDBALL_SOURCES})
set_target_properties (${POLANDBALL_EXECUTABLE} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
target_compile_options (${POLANDBALL_EXECUTABLE} PUBLIC
    $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Werror -pedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/WX>
)
target_compile_definitions (${POLANDBALL_EXECUTABLE} PUBLIC _USE_MATH_DEFINES)

set (POLANDBALL_LINK_LIBRARIES ${GRAPHENE_LIBRARIES} ${MATH_LIBRARIES} ${JSON_C_LIBRARIES})
target_link_libraries (${POLANDBALL_EXECUTABLE} ${POLANDBALL_LINK_LIBRARIES})

configure_file (Config.h.in Config.h @ONLY)

install (TARGETS ${POLANDBALL_EXECUTABLE} DESTINATION bin)
install (DIRECTORY ${POLANDBALL_RESOURCE_DIRS} DESTINATION ${POLANDBALL_DATADIR})
