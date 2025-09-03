#
#   REFERÊNCIA: 
#   https://github.com/libsdl-org/SDL/blob/main/docs/INTRO-cmake.md
#   
#
#
#

cmake_minimum_required(VERSION 3.16)
project(main C)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# A pasta com o código fonte da SDL3 deve estar em lib/SDL3

add_subdirectory(lib/SDL3 EXCLUDE_FROM_ALL)
set(USE_SDL3_TARGET SDL3::SDL3)

# Create your game executable target as usual
if(WIN32)
    add_executable(main WIN32 main.c)
else()
    add_executable(main main.c)
endif()


# Link to the actual SDL3 library.
target_link_libraries(main PRIVATE ${USE_SDL3_TARGET})