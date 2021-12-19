#----------------------------------------------------------------------------------------------------------------------
#   XMidiCtrl - MIDI Controller plugin for X-Plane
#
#   Copyright (c) 2021-2022 Marco Auer
#
#   XMidiCtrl is free software: you can redistribute it and/or modify it under the terms of the
#   GNU Affero General Public License as published by the Free Software Foundation, either version 3
#   of the License, or (at your option) any later version.
#
#   XMidiCtrl is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
#   the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU Affero General Public License for more details.
#
#   You should have received a copy of the GNU Affero General Public License along with XMidiCtrl.
#   If not, see <https://www.gnu.org/licenses/>.
#----------------------------------------------------------------------------------------------------------------------

# Set required CMake version
cmake_minimum_required(VERSION 3.16)

# Set project details
project(XMidiCtrl VERSION 0.51 DESCRIPTION "MIDI Controller Plugin for X-Plane 11")
set(PROJECT_SIGNATURE mauer.xmidictrl)

message("-----------------------------------------------")
message("-- Configure ${PROJECT_NAME} ${PROJECT_VERSION}")
message("-----------------------------------------------")

# Create version file with project name and version numbers
message("-- Creating version file")
configure_file(
        "${PROJECT_SOURCE_DIR}/version.h.in"
        "${PROJECT_BINARY_DIR}/version.h"
)

add_definitions("-include ${PROJECT_BINARY_DIR}/version.h")
include_directories(${PROJECT_BINARY_DIR})

# use highest optimization level and optimize for size
if (NOT MSVC)
    add_compile_options(-O3 -Os)
endif()

# set compiler options
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wno-sign-compare -Wno-free-nonheap-object -fPIC")

# Add subdirectories
add_subdirectory(lib)
add_subdirectory(src)

