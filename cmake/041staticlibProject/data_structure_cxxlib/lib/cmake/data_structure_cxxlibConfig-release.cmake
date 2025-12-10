#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "data_structure_cxxlib::data_structure_cxxlib" for configuration "Release"
set_property(TARGET data_structure_cxxlib::data_structure_cxxlib APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(data_structure_cxxlib::data_structure_cxxlib PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/data_structure_cxxlib.lib"
  )

list(APPEND _cmake_import_check_targets data_structure_cxxlib::data_structure_cxxlib )
list(APPEND _cmake_import_check_files_for_data_structure_cxxlib::data_structure_cxxlib "${_IMPORT_PREFIX}/lib/data_structure_cxxlib.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
