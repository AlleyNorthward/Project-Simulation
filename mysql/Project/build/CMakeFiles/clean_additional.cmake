# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "")
  file(REMOVE_RECURSE
  "login\\CMakeFiles\\loginLib_autogen.dir\\AutogenUsed.txt"
  "login\\CMakeFiles\\loginLib_autogen.dir\\ParseCache.txt"
  "login\\loginLib_autogen"
  )
endif()
