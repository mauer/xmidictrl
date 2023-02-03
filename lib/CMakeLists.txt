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

# Add toml11 for reading and writing the config files
message("-- Configuring build scripts for toml11")
add_subdirectory(toml11)

# Teach MinGW std::threads
if (MINGW)
    message("-- Configuring build scripts for mingw-std-threads")
    option(MINGW_STDTHREADS_GENERATE_STDHEADERS "" ON)
    add_subdirectory(mingw-std-threads)
endif()

# Add headers for fonts
include(${CMAKE_CURRENT_LIST_DIR}/fonts.cmake)

# Add RtMidi for accessing MIDI devices
message("-- Configuring build scripts for RtMidi")

# Configre RtMidi
set(RTMIDI_BUILD_STATIC_LIBS ON)
set(RTMIDI_BUILD_TESTING OFF)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

# Disable build for iPhone
if (APPLE)
    add_definitions(-DTARGET_OS_IPHONE=0)
endif()

if (WIN32)
    set(RTMIDI_API_WINMM ON)
    set(RTMIDI_API_JACK OFF)
endif()

add_subdirectory(rtmidi)
 
# Add the X-Plane SDK
include(${CMAKE_CURRENT_LIST_DIR}/xpsdk.cmake)

# Add GLFW
##message("-- Configuring build scripts for GLFW")
##message("-- OSX SysRoot" ${CMAKE_OSX_SYSROOT})
##set(GLFW_BUILD_EXAMPLES OFF)
##set(GLFW_BUILD_TESTS OFF)
##set(GLFW_BUILD_DOCS OFF)

#add_subdirectory(glfw)

# Add ImGui for the User Interface
include(${CMAKE_CURRENT_LIST_DIR}/imgui.cmake)

