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

message("---- Directory: src/profile/map/map_out")

# Add sources
set(MAP_OUT_SRC
        ${CMAKE_CURRENT_LIST_DIR}/map_out.cpp
        ${CMAKE_CURRENT_LIST_DIR}/map_out_con.cpp
        ${CMAKE_CURRENT_LIST_DIR}/map_out_drf.cpp
        ${CMAKE_CURRENT_LIST_DIR}/map_out_list.cpp
        ${CMAKE_CURRENT_LIST_DIR}/map_out_sld.cpp
        )

# Add sources to target
target_sources(${PROJECT_NAME} PRIVATE ${MAP_OUT_SRC})

# Add directory to includes
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR})