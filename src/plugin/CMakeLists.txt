#----------------------------------------------------------------------------------------------------------------------
#   XMidiCtrl - MIDI Controller plugin for X-Plane
#
#   Copyright (c) 2021-2024 Marco Auer
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

# Include fonts
include_directories(${FONTS_INCLUDEDIR})

# Include the ImGui files
include_directories(${IMGUI_INCLUDEDIR})

# Build shared library
add_library(${PROJECT_NAME} SHARED)

# add static libraries for xmidictrl
target_link_libraries(${PROJECT_NAME} xmidictrl-common xmidictrl-env xmidictrl-env-xplane xmidictrl-dev)

# link required dependencies
if (WIN32)
        message("Build for Windows Platform")
        if (MINGW)
                target_link_libraries(${PROJECT_NAME} mingw_stdthreads)
        endif()
        if (MSVC)
                target_link_libraries(${PROJECT_NAME} toml11 fmt::fmt-header-only rtmidi hidapi::hidapi winmm xpsdk imgui)
                include(GenerateExportHeader)
                generate_export_header(${PROJECT_NAME})
                add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                        $<TARGET_FILE:${PROJECT_NAME}> "${PROJECT_SOURCE_DIR}/package/${PROJECT_NAME}/win_x64/$<TARGET_FILE_NAME:${PROJECT_NAME}>"
                )
        else()
                target_link_libraries(${PROJECT_NAME} -s -static -static-libgcc -static-libstdc++ toml11 fmt::fmt-header-only rtmidi hidapi::hidapi winmm xpsdk imgui)
                set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/package/${PROJECT_NAME}/win_x64")
        endif()
elseif (APPLE)
        message("Build for Apple Platform")
        find_library(CORE_FOUNDATION_LIBRARY CoreFoundation REQUIRED)
        find_library(Cocoa_LIBRARY Cocoa REQUIRED)
        find_library(Security_LIBRARY Security REQUIRED)
        find_library(GSS_LIBRARY GSS REQUIRED)
        find_library(OpenGL_LIBRARY OpenGL REQUIRED)
        # Link OS X core system libraries.
        target_link_libraries(${PROJECT_NAME} ${CORE_FOUNDATION_LIBRARY} ${Cocoa_LIBRARY} ${Security_LIBRARY} ${GSS_LIBRARY} ${OpenGL_LIBRARY})
        target_link_libraries(${PROJECT_NAME} toml11 fmt::fmt-header-only rtmidi hidapi::hidapi xpsdk imgui)
        target_link_options(${PROJECT_NAME} PRIVATE "-Wl,-no_weak_imports")

        # Restrict set of symbols exported from the plugin to the ones required by XPLM:
        # target_link_options(${PROJECT_NAME} PRIVATE "-exported_symbols_list ${CMAKE_SOURCE_DIR}/src/xmidictrl.sym_mac")
        set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/package/${PROJECT_NAME}/mac_x64")
elseif (UNIX)
        message("Build for Linux Platform")
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/package/${PROJECT_NAME}/lin_64")
        target_link_libraries(${PROJECT_NAME} -static-libgcc -static-libstdc++ -fPIC toml11 fmt::fmt-header-only rtmidi hidapi::hidapi xpsdk imgui)
        set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/package/${PROJECT_NAME}/lin_x64")
endif ()

# Set output properties for X-Plane standards
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}")
set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".xpl")

# Add sources
set(PLUGIN_SRC
        ${CMAKE_CURRENT_LIST_DIR}/plugin.cpp
        ${CMAKE_CURRENT_LIST_DIR}/plugin_main.cpp
        ${CMAKE_CURRENT_LIST_DIR}/profile.cpp
        )

# Add sources to target
target_sources(${PROJECT_NAME} PRIVATE ${PLUGIN_SRC})

# Add directory to includes
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR})

# Add sub directories
include(${CMAKE_CURRENT_LIST_DIR}/ui/CMakeLists.txt)
