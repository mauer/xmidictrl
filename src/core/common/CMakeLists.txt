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

message("---- Directory: src/common")

# Add sources
set(COMMON_SRC
        ${CMAKE_CURRENT_LIST_DIR}/app_services.h
        ${CMAKE_CURRENT_LIST_DIR}/config.cpp
        ${CMAKE_CURRENT_LIST_DIR}/config.h
        ${CMAKE_CURRENT_LIST_DIR}/inbound_task.h
        ${CMAKE_CURRENT_LIST_DIR}/info_msg.h
        ${CMAKE_CURRENT_LIST_DIR}/midi_logger.cpp
        ${CMAKE_CURRENT_LIST_DIR}/midi_logger.h
        ${CMAKE_CURRENT_LIST_DIR}/midi_message.cpp
        ${CMAKE_CURRENT_LIST_DIR}/midi_message.h
        ${CMAKE_CURRENT_LIST_DIR}/outbound_task.h
        ${CMAKE_CURRENT_LIST_DIR}/text_log_msg.h
        ${CMAKE_CURRENT_LIST_DIR}/text_logger.cpp
        ${CMAKE_CURRENT_LIST_DIR}/text_logger.h
        ${CMAKE_CURRENT_LIST_DIR}/xmc_types.h
        )

# Add sources to target
target_sources(${PROJECT_NAME} PRIVATE ${COMMON_SRC})

# Add directory to includes
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})
