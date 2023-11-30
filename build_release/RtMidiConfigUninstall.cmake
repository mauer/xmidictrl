if(NOT EXISTS "C:/Users/marco/Projects/xmidictrl/build_release/lib/rtmidi/install_manifest.txt")
  message(FATAL_ERROR "Cannot find install manifest: \"C:/Users/marco/Projects/xmidictrl/build_release/lib/rtmidi/install_manifest.txt\"")
endif(NOT EXISTS "C:/Users/marco/Projects/xmidictrl/build_release/lib/rtmidi/install_manifest.txt")

file(READ "C:/Users/marco/Projects/xmidictrl/build_release/lib/rtmidi/install_manifest.txt" files)
string(REGEX REPLACE "\n" ";" files "${files}")
foreach(file ${files})
  message(STATUS "Uninstalling \"$ENV{DESTDIR}${file}\"")
  if(EXISTS "$ENV{DESTDIR}${file}")
    exec_program(
      "C:/Users/marco/AppData/Local/JetBrains/Toolbox/apps/CLion/ch-0/223.7571.171/bin/cmake/win/bin/cmake.exe" ARGS "-E remove \"$ENV{DESTDIR}${file}\""
      OUTPUT_VARIABLE rm_out
      RETURN_VALUE rm_retval
      )
    if(NOT "${rm_retval}" STREQUAL 0)
      message(FATAL_ERROR "Problem when removing \"$ENV{DESTDIR}${file}\"")
    endif(NOT "${rm_retval}" STREQUAL 0)
  else(EXISTS "$ENV{DESTDIR}${file}")
    message(STATUS "File \"$ENV{DESTDIR}${file}\" does not exist.")
  endif(EXISTS "$ENV{DESTDIR}${file}")
endforeach(file)
