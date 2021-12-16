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


message("-- Configuring build scripts for ${PROJECT_NAME}")

set(CMAKE_CXX_STANDARD 20)

# RtMidi
find_package(PkgConfig REQUIRED)
pkg_check_modules(RTMIDI REQUIRED rtmidi)

include_directories(${RTMIDI_INCLUDEDIR})
include_directories(${IMGUI_INCLUDEDIR})

# Build shared library
add_library(${PROJECT_NAME} SHARED)

# Include sub directories
include(${CMAKE_CURRENT_LIST_DIR}/common/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/utils/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/xplane/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/profile/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/plugin/CMakeLists.txt)

message("---- Directory: src")

# Source and header files
set(XMIDICTRL_SRC
        ${CMAKE_CURRENT_LIST_DIR}/main.cpp
        )

target_sources(${PROJECT_NAME} PRIVATE ${XMIDICTRL_SRC})

# include current directory
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR})

# link required dependencies
if (WIN32)
    target_link_libraries(${PROJECT_NAME} -s -static -static-libgcc -static-libstdc++ toml11 ${RTMIDI_STATIC_LIBRARIES} winmm xpsdk imgui)
elseif (APPLE)
    target_link_libraries(${PROJECT_NAME} "-F${PROJECT_SOURCE_DIR}/lib/xpsdk/Libraries/Mac -framework XPLM")
elseif (UNIX)
    target_link_libraries(${PROJECT_NAME} PUBLIC -static-libgcc -static-libstdc++ -fPIC toml11 ${RTMIDI_STATIC_LIBRARIES} xpsdk imgui)
endif ()

# Set output properties for X-Plane standards
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}")
set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".xpl")
