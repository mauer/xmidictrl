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
project(XMidiCtrl VERSION 1.03 DESCRIPTION "MIDI Controller Plugin for X-Plane 11/12")
set(PROJECT_SIGNATURE mauer.xmidictrl)

message("-- Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}")

# use highest optimization level and optimize for size
if (MSVC)
    add_compile_options("$<$<CONFIG:RELEASE>:/O2>")
else()
    add_compile_options("$<$<CONFIG:RELEASE>:-O3>" "$<$<CONFIG:RELEASE>:-Os>")
endif()

# set compiler options
if (MSVC)
    add_compile_options("/Zc:__cplusplus")
endif()

# disable warning no-free-nonheap-object because of toml11
if (MSVC)
    add_compile_options("/W4")
else()
    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND CMAKE_CXX_COMPILER_VERSION VERSION_EQUAL 10.0.0)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wno-sign-compare -fPIC")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wno-sign-compare -Wno-free-nonheap-object -fPIC")
    endif()
endif()

# Add subdirectories
add_subdirectory(lib)
add_subdirectory(src)
