#----------------------------------------------------------------------------------------------------------------------
#   XMidiCtrl - MIDI Controller plugin for X-Plane
#
#   Copyright (c) 2021-2024 Marco Auer
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
project(xmidictrl-env-tests)

message("-- Configuring build scripts for library ${PROJECT_NAME}")

# Include fonts
include_directories(${FONTS_INCLUDEDIR})

# Add sources
set(ENV_SRC_TESTS
        ${CMAKE_CURRENT_LIST_DIR}/command_tests.cpp
        ${CMAKE_CURRENT_LIST_DIR}/data_tests.cpp
        ${CMAKE_CURRENT_LIST_DIR}/env_tests.cpp
)

# Build static library for X-Plane environment
add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME} PUBLIC ${ENV_SRC_TESTS})
target_link_libraries(${PROJECT_NAME} PUBLIC toml11 fmt::fmt-header-only imgui xpsdk xmidictrl-common xmidictrl-env)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})