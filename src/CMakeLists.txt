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

message("-- Configuring build scripts for ${PROJECT_NAME}")

# RtMidi
find_package(PkgConfig REQUIRED)
pkg_check_modules(RTMIDI REQUIRED rtmidi)

include_directories(${RTMIDI_INCLUDEDIR})
include_directories(${IMGUI_INCLUDEDIR})

# Build shared library
add_library(${PROJECT_NAME} SHARED)

# Include sub directories
include(${CMAKE_CURRENT_LIST_DIR}/utils/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/environment/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/plugin/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/mapping/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/config/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/midi/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/ui/CMakeLists.txt)

message("---- Directory: src")

# Source and header files
set(XMIDICTRL_SRC
        ${CMAKE_CURRENT_LIST_DIR}/main.cpp
        )

target_sources(${PROJECT_NAME} PRIVATE ${XMIDICTRL_SRC})

# include current directory
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR})

# link required dependencies
if (WIN32)
    target_link_libraries(${PROJECT_NAME} PUBLIC -static -static-libgcc -static-libstdc++ toml11 ${RTMIDI_STATIC_LIBRARIES} winmm xpsdk imgui)
elseif (APPLE)
    target_link_libraries(${PROJECT_NAME} "-F${PROJECT_SOURCE_DIR}/lib/xpsdk/Libraries/Mac -framework XPLM")
elseif (UNIX)
endif ()

# Set output properties for X-Plane standards
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}")
set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".xpl")