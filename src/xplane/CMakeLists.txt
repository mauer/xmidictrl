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

message("---- Directory: src/xplane")

# Set sources
set(XPLANE_SRC
        ${CMAKE_CURRENT_LIST_DIR}/commands.cpp
        ${CMAKE_CURRENT_LIST_DIR}/data.cpp
        ${CMAKE_CURRENT_LIST_DIR}/ImGuiFontAtlas.cpp
        ${CMAKE_CURRENT_LIST_DIR}/ImGuiWindow.cpp
        ${CMAKE_CURRENT_LIST_DIR}/xplane.cpp
        ${CMAKE_CURRENT_LIST_DIR}/xplane_window.cpp
        )

# Add sources to target
target_sources(${PROJECT_NAME} PRIVATE ${XPLANE_SRC})

# Include current directory
include_directories(${CMAKE_CURRENT_LIST_DIR})