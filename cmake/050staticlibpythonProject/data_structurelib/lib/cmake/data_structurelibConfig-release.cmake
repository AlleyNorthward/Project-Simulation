#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "data_structurelib::data_structurelib" for configuration "Release"
set_property(TARGET data_structurelib::data_structurelib APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(data_structurelib::data_structurelib PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/data_structurelib.lib"
  )

list(APPEND _cmake_import_check_targets data_structurelib::data_structurelib )
list(APPEND _cmake_import_check_files_for_data_structurelib::data_structurelib "${_IMPORT_PREFIX}/lib/data_structurelib.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
