#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "AlgVis::AlgVis" for configuration ""
set_property(TARGET AlgVis::AlgVis APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(AlgVis::AlgVis PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "C"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libAlgVis.a"
  )

list(APPEND _cmake_import_check_targets AlgVis::AlgVis )
list(APPEND _cmake_import_check_files_for_AlgVis::AlgVis "${_IMPORT_PREFIX}/lib/libAlgVis.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
