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
set(XMIDICTRL_LIB_ENV xmidictrl-env)
project(${XMIDICTRL_LIB_ENV})

message("-- Configuring build scripts for library ${XMIDICTRL_LIB_ENV}")

# Add sources
set(ENV_SRC
        ${CMAKE_CURRENT_LIST_DIR}/environment.cpp
        ${CMAKE_CURRENT_LIST_DIR}/inbound_worker.cpp
        ${CMAKE_CURRENT_LIST_DIR}/settings.cpp
        )

set(ENV_SRC_XP
        ${CMAKE_CURRENT_LIST_DIR}/xplane/cmd_xplane.cpp
        ${CMAKE_CURRENT_LIST_DIR}/xplane/data_xplane.cpp
        ${CMAKE_CURRENT_LIST_DIR}/xplane/env_xplane.cpp
        )

set(ENV_SRC_TESTS
        ${CMAKE_CURRENT_LIST_DIR}/tests/cmd_tests.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tests/data_tests.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tests/env_tests.cpp
        )

# Build static library for X-Plane environment
add_library(${XMIDICTRL_LIB_ENV} STATIC)
add_library(xmidictrl::env ALIAS ${XMIDICTRL_LIB_ENV})

target_sources(${XMIDICTRL_LIB_ENV} PUBLIC ${ENV_SRC} ${ENV_SRC_XP} ${ENV_SRC_TESTS})

target_link_libraries(${XMIDICTRL_LIB_ENV} PUBLIC toml11 fmt::fmt-header-only xpsdk xmidictrl::common)
target_include_directories(${XMIDICTRL_LIB_ENV} PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(${XMIDICTRL_LIB_ENV} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/xplane)
target_include_directories(${XMIDICTRL_LIB_ENV} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/tests)
