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
if (WIN32)
    target_link_libraries(xpsdk INTERFACE ${CMAKE_CURRENT_LIST_DIR}/xpsdk/Libraries/Win/XPLM_64.lib)
elseif (APPLE)
    target_link_libraries(xpsdk INTERFACE "-F${CMAKE_CURRENT_LIST_DIR}/xpsdk/Libraries/Mac -framework XPLM")
endif()


# Include directories to target
target_include_directories(xpsdk INTERFACE ${CMAKE_CURRENT_LIST_DIR}/xpsdk/CHeaders/Widgets)
target_include_directories(xpsdk INTERFACE ${CMAKE_CURRENT_LIST_DIR}/xpsdk/CHeaders/Wrappers)
target_include_directories(xpsdk INTERFACE ${CMAKE_CURRENT_LIST_DIR}/xpsdk/CHeaders/XPLM)
