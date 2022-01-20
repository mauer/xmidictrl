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

# Add RtMidi for accessing MIDI devices
message("-- Configuring build scripts for RtMidi")

# Configre RtMidi
set(RTMIDI_BUILD_STATIC_LIBS ON)
set(RTMIDI_BUILD_TESTING OFF)

add_definitions(-DTARGET_OS_IPHONE=0)

# Teach MingW std::threads
if (MINGW)
    option(MINGW_STDTHREADS_GENERATE_STDHEADERS "" ON)
    add_subdirectory(mingw-std-threads)
endif()

if (WIN32)
    set(RTMIDI_API_WINMM ON)
    set(RTMIDI_API_JACK OFF)
endif()

add_subdirectory(rtmidi)
 
# Add the X-Plane SDK
include(${CMAKE_CURRENT_LIST_DIR}/xpsdk.cmake)

# Add ImGui for the User Interface
include(${CMAKE_CURRENT_LIST_DIR}/imgui.cmake)

