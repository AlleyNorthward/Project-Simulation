cmake_minimum_required(VERSION 3.25)

add_library(
  utilsLib
  STATIC
  src/anim.cpp
  include/ImageSlider.hpp
  src/ImageSlider.cpp
)

target_include_directories(
  utilsLib
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_compile_features(
  utilsLib
  PUBLIC
  cxx_std_17
)

set_target_properties(
  utilsLib
  PROPERTIES
  AUTOMOC ON
  AUTOUIC ON
  AUTORCC ON
)

target_link_libraries(
  utilsLib
  PUBLIC
  Qt6::Widgets
)
