cmake_minimum_required(VERSION 3.25)

add_library(
  utilsLib
  INTERFACE
)

target_include_directories(
  utilsLib
  INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_compile_features(
  utilsLib
  INTERFACE
  cxx_std_17
)

target_link_libraries(
  utilsLib
  INTERFACE
  Qt6::Core
)
