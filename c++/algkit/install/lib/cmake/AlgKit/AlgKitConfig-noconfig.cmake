#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "AlgKit::AlgKit" for configuration ""
set_property(TARGET AlgKit::AlgKit APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(AlgKit::AlgKit PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "CXX"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libAlgKit.a"
  )

list(APPEND _cmake_import_check_targets AlgKit::AlgKit )
list(APPEND _cmake_import_check_files_for_AlgKit::AlgKit "${_IMPORT_PREFIX}/lib/libAlgKit.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
