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

# Build X-Plane SDK as interface library
message("-- Configuring build scripts for X-Plane SDK")

add_library(xpsdk INTERFACE)

# Set X-Plane SDK Version
target_compile_definitions(xpsdk INTERFACE -DXPLM303=1 -DXPLM302=1 -DXPLM301=1 -DXPLM300=1 -DXPLM210=1 -DXPLM200=1)

# Set Operating System
if (WIN32)
    message("---- Set Operating System to IBM")
    target_compile_definitions(xpsdk INTERFACE -DIBM=1)
elseif (APPLE)
    message("---- Set Operating System to APPLE")
    target_compile_definitions(xpsdk INTERFACE -DAPL=1)
elseif (UNIX)
    message("---- Set Operating System to Linux")
    target_compile_definitions(xpsdk INTERFACE -DLIN=1)
endif()

# Link required libraries
target_link_libraries(xpsdk INTERFACE ${CMAKE_CURRENT_LIST_DIR}/xpsdk/Libraries/Win/XPLM_64.lib)

# Include directories to target
target_include_directories(xpsdk INTERFACE ${CMAKE_CURRENT_LIST_DIR}/xpsdk/CHeaders/Widgets)
target_include_directories(xpsdk INTERFACE ${CMAKE_CURRENT_LIST_DIR}/xpsdk/CHeaders/Wrappers)
target_include_directories(xpsdk INTERFACE ${CMAKE_CURRENT_LIST_DIR}/xpsdk/CHeaders/XPLM)