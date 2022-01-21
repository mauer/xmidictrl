# Toolchain for Visual Studio builds via command line/NMAKE

# We don't search but _set_ the location of libcurl as we provide a ready-made static lib
set(CURL_INCLUDE_DIR        "${CMAKE_SOURCE_DIR}/lib/CURL"              CACHE PATH          "libcurl headers"               FORCE)
set(CURL_LIBRARY_RELEASE    "${CMAKE_SOURCE_DIR}/lib/CURL/libcurl.lib"  CACHE FILEPATH      "static libcurl release lib"    FORCE)
set(CURL_LIBRARY_DEBUG      "${CMAKE_SOURCE_DIR}/lib/CURL/libcurl.lib"  CACHE FILEPATH      "static libcurl debug lib"      FORCE)

set(CMAKE_EXE_LINKER_FLAGS                  "/machine:x64 /INCREMENTAL:NO /LTCG"        CACHE STRING        "Linker Flags"                  FORCE)
set(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO   "/machine:x64 /INCREMENTAL:NO /LTCG /DEBUG" CACHE STRING        "Linker Flags"                  FORCE)
