#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "RtMidi::rtmidi" for configuration "RelWithDebInfo"
set_property(TARGET RtMidi::rtmidi APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(RtMidi::rtmidi PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELWITHDEBINFO "CXX"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/rtmidi.lib"
  )

list(APPEND _cmake_import_check_targets RtMidi::rtmidi )
list(APPEND _cmake_import_check_files_for_RtMidi::rtmidi "${_IMPORT_PREFIX}/lib/rtmidi.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
