set(XPENV_SRC ${CMAKE_CURRENT_LIST_DIR}/PluginLogEntry.cpp
              ${CMAKE_CURRENT_LIST_DIR}/PluginLogger.cpp 
              ${CMAKE_CURRENT_LIST_DIR}/XPlanePlugin.cpp)

set(XPENV_HDR ${CMAKE_CURRENT_LIST_DIR}/PluginLogEntry.h
              ${CMAKE_CURRENT_LIST_DIR}/PluginLogger.h 
              ${CMAKE_CURRENT_LIST_DIR}/XPlanePlugin.h)

add_library(xpenv STATIC ${XPENV_SRC} ${XPENV_HDR})

target_include_directories(xpenv INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

if (WIN32)
    add_definitions(-DIBM=1)
    target_link_libraries(xpenv ${PROJECT_SOURCE_DIR}/lib/xpsdk/Libraries/Win/XPLM_64.lib)
elseif (APPLE)
    add_definitions(-DAPL=1)
    target_link_libraries(xpenv "-F${PROJECT_SOURCE_DIR}/lib/xpsdk/Libraries/Mac -framework XPLM")
elseif (UNIX)
    add_definitions(-DLIN=1)
endif()