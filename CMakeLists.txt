cmake_minimum_required (VERSION 2.6)
project (POLANDBALL)

if (CMAKE_COMPILER_IS_GNUCC)
    execute_process (COMMAND ${CMAKE_CXX_COMPILER} -dumpversion
                     OUTPUT_VARIABLE GCC_VERSION)
    string (STRIP ${GCC_VERSION} GCC_VERSION)

    if (GCC_VERSION VERSION_GREATER 4.7.0)
        set (CMAKE_CXX_FLAGS "-Wall -pedantic -std=c++11")
    elseif (GCC_VERSION VERSION_GREATER 4.6.0)
        set (CMAKE_CXX_FLAGS "-Wall -pedantic -std=c++0x")
    else ()
        message(FATAL_ERROR "GCC 4.6.0 or higher is required!")
    endif ()
endif ()

if (UNIX)
    # Override include directiries for SDL2
    set (SDL_INCLUDE_DIR "/usr/include/SDL2")
    set (SDL_IMAGE_INCLUDE_DIRS "/usr/include/SDL2")
endif ()

find_package (OpenGL REQUIRED)
find_package (SDL REQUIRED)
find_package (SDL_image REQUIRED)

file (GLOB_RECURSE POLANDBALL_SOURCES "src/*.cpp")
include_directories ("src" "src/math")

set (POLANDBALL_EXECUTABLE "polandball")

# Override libraries for SDL2
set (SDL_LIBRARY_FIXED "/usr/lib64/libSDL2.so")
set (SDL_IMAGE_LIBRARIES_FIXED "/usr/lib64/libSDL2_image.so")

add_executable (${POLANDBALL_EXECUTABLE} ${POLANDBALL_SOURCES})

target_link_libraries (${POLANDBALL_EXECUTABLE} ${OPENGL_LIBRARIES})
target_link_libraries (${POLANDBALL_EXECUTABLE} ${SDL_LIBRARY_FIXED})
target_link_libraries (${POLANDBALL_EXECUTABLE} ${SDL_IMAGE_LIBRARIES_FIXED})