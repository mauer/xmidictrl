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
set(XMIDICTRL_LIB_MAP xmidictrl-map)
project(${XMIDICTRL_LIB_MAP})

message("-- Configuring build scripts for library ${XMIDICTRL_LIB_MAP}")

message("---- Directory: src/map")

# Add sources
set(MAP_SRC
        ${CMAKE_CURRENT_LIST_DIR}/map_init/map_init.cpp
        ${CMAKE_CURRENT_LIST_DIR}/map_in/map_in.cpp
        ${CMAKE_CURRENT_LIST_DIR}/map_in/map_in_cmd.cpp
        ${CMAKE_CURRENT_LIST_DIR}/map_in/map_in_drf.cpp
        ${CMAKE_CURRENT_LIST_DIR}/map_in/map_in_enc.cpp
        ${CMAKE_CURRENT_LIST_DIR}/map_in/map_in_label.cpp
        ${CMAKE_CURRENT_LIST_DIR}/map_in/map_in_list.cpp
        ${CMAKE_CURRENT_LIST_DIR}/map_in/map_in_pnp.cpp
        ${CMAKE_CURRENT_LIST_DIR}/map_in/map_in_sld.cpp
        ${CMAKE_CURRENT_LIST_DIR}/map_init/map_init.cpp
        ${CMAKE_CURRENT_LIST_DIR}/map_init/map_init_list.cpp
        ${CMAKE_CURRENT_LIST_DIR}/map_out/map_out.cpp
        ${CMAKE_CURRENT_LIST_DIR}/map_out/map_out_con.cpp
        ${CMAKE_CURRENT_LIST_DIR}/map_out/map_out_drf.cpp
        ${CMAKE_CURRENT_LIST_DIR}/map_out/map_out_list.cpp
        ${CMAKE_CURRENT_LIST_DIR}/map_out/map_out_sld.cpp
        )

# Build static library
add_library(${XMIDICTRL_LIB_MAP} STATIC ${MAP_SRC})
add_library(${XMIDICTRL_LIB_MAP}-tests STATIC ${MAP_SRC})

target_link_libraries(${XMIDICTRL_LIB_MAP} PUBLIC toml11 fmt::fmt-header-only xmidictrl-common xmidictrl-env)
target_link_libraries(${XMIDICTRL_LIB_MAP}-tests PUBLIC toml11 fmt::fmt-header-only xmidictrl-common xmidictrl-env-tests)

target_include_directories(${XMIDICTRL_LIB_MAP} PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(${XMIDICTRL_LIB_MAP} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/map_init)
target_include_directories(${XMIDICTRL_LIB_MAP} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/map_in)
target_include_directories(${XMIDICTRL_LIB_MAP} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/map_out)

target_include_directories(${XMIDICTRL_LIB_MAP}-tests PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(${XMIDICTRL_LIB_MAP}-tests PUBLIC ${CMAKE_CURRENT_LIST_DIR}/map_init)
target_include_directories(${XMIDICTRL_LIB_MAP}-tests PUBLIC ${CMAKE_CURRENT_LIST_DIR}/map_in)
target_include_directories(${XMIDICTRL_LIB_MAP}-tests PUBLIC ${CMAKE_CURRENT_LIST_DIR}/map_out)