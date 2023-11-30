#----------------------------------------------------------------
# Generated CMake target import file for configuration "MinSizeRel".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "RtMidi::rtmidi" for configuration "MinSizeRel"
set_property(TARGET RtMidi::rtmidi APPEND PROPERTY IMPORTED_CONFIGURATIONS MINSIZEREL)
set_target_properties(RtMidi::rtmidi PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_MINSIZEREL "CXX"
  IMPORTED_LOCATION_MINSIZEREL "${_IMPORT_PREFIX}/lib/rtmidi.lib"
  )

list(APPEND _cmake_import_check_targets RtMidi::rtmidi )
list(APPEND _cmake_import_check_files_for_RtMidi::rtmidi "${_IMPORT_PREFIX}/lib/rtmidi.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
