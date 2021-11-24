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

message("---- Directory: src/map_in")

# Add sources
set(MAP_IN_SRC
        ${CMAKE_CURRENT_LIST_DIR}/map_in.cpp
        ${CMAKE_CURRENT_LIST_DIR}/map_in_cmd.cpp
        ${CMAKE_CURRENT_LIST_DIR}/map_in_drf.cpp
        ${CMAKE_CURRENT_LIST_DIR}/map_in_enc.cpp
        ${CMAKE_CURRENT_LIST_DIR}/map_in_int.cpp
        ${CMAKE_CURRENT_LIST_DIR}/map_in_list.cpp
        ${CMAKE_CURRENT_LIST_DIR}/map_in_pnp.cpp
        ${CMAKE_CURRENT_LIST_DIR}/map_in_sld.cpp
        )

# Add sources to target
target_sources(${PROJECT_NAME} PRIVATE ${MAP_IN_SRC})

# Add directory to includes
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR})