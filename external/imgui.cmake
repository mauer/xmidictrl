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

# Static library for Dear ImGui
message("-- Configuring build scripts for Dear ImGui")

# Find OpenGL
include(FindOpenGL)

# Add sources
set(IMGUI_SRC
        ${CMAKE_CURRENT_LIST_DIR}/imgui/imgui.cpp
        ${CMAKE_CURRENT_LIST_DIR}/imgui/imgui_demo.cpp
        ${CMAKE_CURRENT_LIST_DIR}/imgui/imgui_draw.cpp
        ${CMAKE_CURRENT_LIST_DIR}/imgui/imgui_tables.cpp
        ${CMAKE_CURRENT_LIST_DIR}/imgui/imgui_widgets.cpp
        ${CMAKE_CURRENT_LIST_DIR}/imgui/misc/cpp/imgui_stdlib.cpp
        )

# Set global variable with include directory
set(IMGUI_INCLUDEDIR ${CMAKE_CURRENT_LIST_DIR}/imgui PARENT_SCOPE)

# Build static library
add_library(imgui STATIC ${IMGUI_SRC})

# Add custom config file
message("---- Set config file to ${CMAKE_CURRENT_LIST_DIR}/imgui_config.h")
target_compile_definitions(imgui PRIVATE -DIMGUI_USER_CONFIG="${CMAKE_CURRENT_LIST_DIR}/imgui_config.h")

# Link required libraries
target_link_libraries(imgui PUBLIC OpenGL::GL)

# Include directories to target
target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_LIST_DIR}/imgui)
target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_LIST_DIR}/imgui/misc/cpp)