#---------------------------------------------------------------------------------------------------------------------
#   MIT License
#
#   Copyright (c) 2021 Marco Auer
#
#   Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
#   documentation files (the "Software"), to deal in the Software without restriction, including without limitation
#   the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and
#   to permit persons to whom the Software is furnished to do so, subject to the following conditions:
#
#   The above copyright notice and this permission notice shall be included in all copies or substantial portions of
#   the Software.
#
#   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
#   THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
#   CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
#   IN THE SOFTWARE.
#---------------------------------------------------------------------------------------------------------------------

# Set required CMake version
cmake_minimum_required(VERSION 3.16)

# Set project details
project(XMidiCtrl VERSION 0.03 DESCRIPTION "A Midi Controller Plugin for X-Plane 11")

message("-----------------------------------------------")
message("-- Configure ${PROJECT_NAME} ${PROJECT_VERSION}")
message("-----------------------------------------------")

# Create version file with project name and version numbers
message("-- Creating version file")
configure_file(
        "${PROJECT_SOURCE_DIR}/Version.h.in"
        "${PROJECT_BINARY_DIR}/Version.h"
)

add_definitions("-include ${PROJECT_BINARY_DIR}/Version.h")
include_directories(${PROJECT_BINARY_DIR})

# use highest optimization level
if (NOT MSVC)
    add_compile_options(-O3)
endif()

# set compiler options
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wno-free-nonheap-object -fPIC")

# Add subdirectories
add_subdirectory(lib)
add_subdirectory(src)
