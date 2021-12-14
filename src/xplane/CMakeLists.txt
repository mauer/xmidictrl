#---------------------------------------------------------------------------------------------------------------------
#   Copyright (c) 2021 Marco Auer <marco@marcoauer.de>
#
#   MIT License
#
#   Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
#   documentation files (the "Software"), to deal in the Software without restriction, including without limitation
#   the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and
#   to permit persons to whom the Software is furnished to do so, subject to the following conditions:
#
#   The above copyright notice and this permission notice shall be included in all copies or substantial portions of
#   the Software.
#
#   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
#   THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
#   CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
#   IN THE SOFTWARE.
#---------------------------------------------------------------------------------------------------------------------

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