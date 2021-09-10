#---------------------------------------------------------------------------------------------------------------------
#   XMidiCtrl - A MIDI Controller plugin for X-Plane 11
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

# Add toml11 for reading and writing the config files
message("-- Configuring build scripts for toml11")
add_subdirectory(toml11)

## Add the X-Plane SDK
include(${CMAKE_CURRENT_LIST_DIR}/xpsdk.cmake)

# Add ImGui for the User Interface
include(${CMAKE_CURRENT_LIST_DIR}/imgui.cmake)

# Add ImgWindow
#include(${CMAKE_CURRENT_LIST_DIR}/imgwindow.cmake)
