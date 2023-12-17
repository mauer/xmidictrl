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
project(xmidictrl-common)

message("-- Configuring build scripts for library ${PROJECT_NAME}")

# Add sources
set(COMMON_SRC
        ${CMAKE_CURRENT_LIST_DIR}/map.cpp
        ${CMAKE_CURRENT_LIST_DIR}/midi_logger.cpp
        ${CMAKE_CURRENT_LIST_DIR}/midi_message.cpp
        ${CMAKE_CURRENT_LIST_DIR}/text_logger.cpp
        ${CMAKE_CURRENT_LIST_DIR}/toml_utils.cpp
        ${CMAKE_CURRENT_LIST_DIR}/utils.cpp
        )

# Build static library
add_library(${PROJECT_NAME} STATIC ${COMMON_SRC})
add_library(xmidictrl::common ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PUBLIC toml11 fmt::fmt-header-only)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})