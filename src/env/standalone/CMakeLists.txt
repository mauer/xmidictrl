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

message("---- Directory: src/env/standalone")

# Set sources
set(ENV_STANDALONE_SRC
        ${CMAKE_CURRENT_LIST_DIR}/cmd_standalone.cpp
        ${CMAKE_CURRENT_LIST_DIR}/data_standalone.cpp
        ${CMAKE_CURRENT_LIST_DIR}/env_standalone.cpp
        )

# Add sources to target
target_sources(${PROJECT_NAME} PRIVATE ${ENV_STANDALONE_SRC})

# Add directory to includes
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR})