#----------------------------------------------------------------------------------------------------------------------
#   XMidiCtrl - MIDI Controller plugin for X-Plane
#
#   Copyright (c) 2022 Michael Hasling
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

cmake_minimum_required(VERSION 3.10)
project(xmidictrl_core)

set(CMAKE_CXX_STANDARD 20)

if (NOT TARGET toml11)
    # Add toml11 for reading and writing the config files
    message("-- Configuring build scripts for toml11")
    add_subdirectory(../../lib/toml11 lib/toml11)
endif()

if (NOT TARGET rtmidi)
    # Add RtMidi for accessing MIDI devices
    message("-- Configuring build scripts for RtMidi")

    # Configre RtMidi
    set(RTMIDI_BUILD_STATIC_LIBS ON)
    set(RTMIDI_BUILD_TESTING OFF)
    set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

    # Disable build for iPhone
    if (APPLE)
        add_definitions(-DTARGET_OS_IPHONE=0)
    endif()

    if (WIN32)
        set(RTMIDI_API_WINMM ON)
        set(RTMIDI_API_JACK OFF)
    endif()

    add_subdirectory(../../lib/rtmidi lib/rtmidi)
endif()

# Build the xmidictrl_core static library
add_library(${PROJECT_NAME} STATIC)
include(${CMAKE_CURRENT_LIST_DIR}/common/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/profile/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/utils/CMakeLists.txt)
target_link_libraries(${PROJECT_NAME} toml11 rtmidi)
