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

message("---- Directory: src/ui")

# Add sources
set(UI_SRC
        ${CMAKE_CURRENT_LIST_DIR}/about_window.cpp
        ${CMAKE_CURRENT_LIST_DIR}/ImGuiFontAtlas.cpp
        ${CMAKE_CURRENT_LIST_DIR}/ImGuiWindow.cpp
        ${CMAKE_CURRENT_LIST_DIR}/menu.cpp
        ${CMAKE_CURRENT_LIST_DIR}/messages_window.cpp
        ${CMAKE_CURRENT_LIST_DIR}/devices_window.cpp
        ${CMAKE_CURRENT_LIST_DIR}/settings_window.cpp
        ${CMAKE_CURRENT_LIST_DIR}/XPlaneWindow.cpp
        )

# Add sources to target
target_sources(${PROJECT_NAME} PRIVATE ${UI_SRC})

# Add directory to includes
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR})