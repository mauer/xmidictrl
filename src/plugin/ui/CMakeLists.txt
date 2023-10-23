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

message("---- Directory: src/ui")

# Add sources
set(UI_SRC
        ${CMAKE_CURRENT_LIST_DIR}/about_window.cpp
        ${CMAKE_CURRENT_LIST_DIR}/devices_window.cpp
        ${CMAKE_CURRENT_LIST_DIR}/info_window.cpp
        ${CMAKE_CURRENT_LIST_DIR}/log_window.cpp
        ${CMAKE_CURRENT_LIST_DIR}/menu.cpp
        ${CMAKE_CURRENT_LIST_DIR}/messages_window.cpp
        ${CMAKE_CURRENT_LIST_DIR}/profile_window.cpp
        ${CMAKE_CURRENT_LIST_DIR}/settings_window.cpp
        )

# Add sources to target
target_sources(${PROJECT_NAME} PRIVATE ${UI_SRC})

# Add directory to includes
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR})
