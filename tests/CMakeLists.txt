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

message("-- Configuring build scripts for ${PROJECT_NAME} Unit Tests")

set(CMAKE_CXX_STANDARD 20)
set(TARGET_TESTS ${PROJECT_NAME}-Tests)

# Include the ImGui files
include_directories(${IMGUI_INCLUDEDIR})

# Add sources
set(TEST_SRC
        ${CMAKE_CURRENT_LIST_DIR}/main.cpp
        ${CMAKE_CURRENT_LIST_DIR}/test_map_in_cmd.cpp
        ${CMAKE_CURRENT_LIST_DIR}/test_midi_message.cpp
        )

add_executable(${TARGET_TESTS} ${TEST_SRC})
target_link_libraries(${TARGET_TESTS} PRIVATE toml11 fmt::fmt-header-only doctest xpsdk xmidictrl-common xmidictrl-env xmidictrl-map)
#target_link_libraries(${TARGET_TESTS} PRIVATE toml11 fmt::fmt-header-only imgui xpsdk doctest)

#target_include_directories(${TARGET_TESTS} PRIVATE ${PROJECT_SOURCE_DIR}/src/common)
#target_include_directories(${TARGET_TESTS} PRIVATE ${PROJECT_SOURCE_DIR}/src/env)
#target_include_directories(${TARGET_TESTS} PRIVATE ${PROJECT_SOURCE_DIR}/src/env/tests)
#target_include_directories(${TARGET_TESTS} PRIVATE ${PROJECT_SOURCE_DIR}/src/profile)
#target_include_directories(${TARGET_TESTS} PRIVATE ${PROJECT_SOURCE_DIR}/src/profile/map)
#target_include_directories(${TARGET_TESTS} PRIVATE ${PROJECT_SOURCE_DIR}/src/profile/map/map_in)
#target_include_directories(${TARGET_TESTS} PRIVATE ${PROJECT_SOURCE_DIR}/src/profile/map/map_out)
#target_include_directories(${TARGET_TESTS} PRIVATE ${PROJECT_SOURCE_DIR}/src/utils)




