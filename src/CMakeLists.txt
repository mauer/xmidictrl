# set compiler options
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")

# X-Plane SDK
add_definitions(-DXPLM303=1 -DXPLM302=1 -DXPLM301=1 -DXPLM300=1 -DXPLM210=1 -DXPLM200=1)

include_directories("${PROJECT_SOURCE_DIR}/lib/xpsdk/CHeaders/XPLM")
link_directories("${PROJECT_SOURCE_DIR}/lib/xpsdk/Libraries/Win")

# RtMidi
find_package( PkgConfig REQUIRED )
pkg_check_modules(RTMIDI REQUIRED rtmidi)

include_directories(${RTMIDI_INCLUDEDIR})

# include sub modules
add_subdirectory(xpenv)

# Source and header files
set(XMIDICTRL_SRC Device.cpp
                  EventHandler.cpp 
                  main.cpp
                  Plugin.cpp
                  Settings.cpp
                  XMidiCtrlMenu.cpp)

set(XMIDICTRL_HDR Device.h
                  EventHandler.h
                  Plugin.h
                  Settings.h
                  XMidiCtrlMenu.h)

# add library
add_library(${PROJECT_NAME} SHARED ${XMIDICTRL_SRC} ${XMIDICTRL_HDR})

# link required dependencies
if (WIN32)
    add_definitions(-DIBM=1)
    target_link_libraries(${PROJECT_NAME} PUBLIC -static -static-libgcc -static-libstdc++ toml11 ${RTMIDI_STATIC_LIBRARIES} winmm xpenv XPLM_64.lib)
elseif (APPLE)
    add_definitions(-DAPL=1)
    target_link_libraries(${PROJECT_NAME} "-F${PROJECT_SOURCE_DIR}/lib/xpsdk/Libraries/Mac -framework XPLM")
elseif (UNIX)
    add_definitions(-DLIN=1)
endif()

# set output properties
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "XMidiCtrl")
set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".xpl")