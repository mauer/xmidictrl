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

project(xmidictrl-tests)
message("-- Configuring build scripts for ${PROJECT_NAME} Unit Tests")

set(CMAKE_CXX_STANDARD 20)

# Include the ImGui files
include_directories(${IMGUI_INCLUDEDIR})

# Add sources
set(TEST_SRC
        ${CMAKE_CURRENT_LIST_DIR}/main.cpp
        ${CMAKE_CURRENT_LIST_DIR}/test_map_in_cmd.cpp
        ${CMAKE_CURRENT_LIST_DIR}/test_midi_message.cpp
        )

add_executable(${PROJECT_NAME} ${TEST_SRC})

if (WIN32)
    message("Build unit tests for Windows Platform")
elseif (UNIX)
    message("Build unit tests for Linux Platform")
endif ()

target_link_libraries(${PROJECT_NAME} toml11 fmt::fmt-header-only rtmidi imgui doctest xmidictrl-common xmidictrl-env-tests xmidictrl-map-tests)

include(${CMAKE_SOURCE_DIR}/external/doctest/scripts/cmake/doctest.cmake)
doctest_discover_tests(${PROJECT_NAME})