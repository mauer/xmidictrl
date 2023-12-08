#----------------------------------------------------------------------------------------------------------------------
#   XMidiCtrl - MIDI Controller plugin for X-Plane
#
#   Copyright (c) 2021-2023 Marco Auer
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
set(XMIDICTRL_LIB_PROFILE xmidictrl-profile)
project(${XMIDICTRL_LIB_PROFILE})

message("-- Configuring build scripts for library ${XMIDICTRL_LIB_PROFILE}")

message("---- Directory: src/profile")

# Add sources
set(PROFILE_SRC

        ${CMAKE_CURRENT_LIST_DIR}/conversions.cpp
        ${CMAKE_CURRENT_LIST_DIR}/device.cpp
        ${CMAKE_CURRENT_LIST_DIR}/device_list.cpp
        ${CMAKE_CURRENT_LIST_DIR}/midi_device.cpp
        ${CMAKE_CURRENT_LIST_DIR}/profile.cpp
        ${CMAKE_CURRENT_LIST_DIR}/virtual_device.cpp
        )

# Build static library
add_library(${XMIDICTRL_LIB_PROFILE} STATIC ${PROFILE_SRC})
add_library(xmidictrl::profile ALIAS ${XMIDICTRL_LIB_PROFILE})

target_link_libraries(${XMIDICTRL_LIB_PROFILE} PUBLIC toml11 fmt::fmt-header-only rtmidi xmidictrl::common xmidictrl::env xmidictrl::map)

target_include_directories(${XMIDICTRL_LIB_PROFILE} PUBLIC ${CMAKE_CURRENT_LIST_DIR})
