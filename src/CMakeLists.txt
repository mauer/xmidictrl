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

message("-- Configuring build scripts for ${PROJECT_NAME}")

set(CMAKE_CXX_STANDARD 20)

# Include fonts
include_directories(${FONTS_INCLUDEDIR})

# Include the ImGui files
include_directories(${IMGUI_INCLUDEDIR})

# Build shared library
add_library(${PROJECT_NAME} SHARED)

# Include sub directories
add_subdirectory(common)
add_subdirectory(env)
add_subdirectory(map)
add_subdirectory(profile)

#include(${CMAKE_CURRENT_LIST_DIR}/utils/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/ui/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/plugin/CMakeLists.txt)

# Apple deprecated OpenGL...yea, we know, keep quiet
if (APPLE)
    set_source_files_properties(
            ${CMAKE_CURRENT_LIST_DIR}/ui/ImgFontAtlas.cpp
            ${CMAKE_CURRENT_LIST_DIR}/ui/imgui_window.cpp
        PROPERTIES
            COMPILE_OPTIONS "-Wno-deprecated-declarations")
endif()

# add static libraries for xmidictrl
target_link_libraries(${PROJECT_NAME} PUBLIC xmidictrl::common xmidictrl::env xmidictrl::profile)

# link required dependencies
if (WIN32)
    message("Build for Windows Platform")
    if (MINGW)
        target_link_libraries(${PROJECT_NAME} mingw_stdthreads)
    endif()
    if (MSVC)
        target_link_libraries(${PROJECT_NAME} PUBLIC toml11 fmt::fmt-header-only rtmidi winmm xpsdk imgui)
        include (GenerateExportHeader)
        GENERATE_EXPORT_HEADER(${PROJECT_NAME})
        add_custom_command (TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:${PROJECT_NAME}> "${PROJECT_SOURCE_DIR}/package/${PROJECT_NAME}/win_x64/$<TARGET_FILE_NAME:${PROJECT_NAME}>"
        )
    else()
        target_link_libraries(${PROJECT_NAME} -s -static -static-libgcc -static-libstdc++ toml11 fmt::fmt-header-only rtmidi winmm xpsdk imgui)
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
    target_link_libraries(${PROJECT_NAME} toml11 fmt::fmt-header-only rtmidi xpsdk imgui)
    target_link_options(${PROJECT_NAME} PRIVATE "-Wl,-no_weak_imports")

    # Restrict set of symbols exported from the plugin to the ones required by XPLM:
    # target_link_options(${PROJECT_NAME} PRIVATE "-exported_symbols_list ${CMAKE_SOURCE_DIR}/src/xmidictrl.sym_mac")
    set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/package/${PROJECT_NAME}/mac_x64")
elseif (UNIX)
    message("Build for Linux Platform")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/package/${PROJECT_NAME}/lin_64")
    target_link_libraries(${PROJECT_NAME} PUBLIC -static-libgcc -static-libstdc++ -fPIC toml11 fmt::fmt-header-only rtmidi xpsdk imgui)
    set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/package/${PROJECT_NAME}/lin_x64")
endif ()

# Set output properties for X-Plane standards
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}")
set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".xpl")
