cmake_minimum_required(VERSION 3.25)

add_library(
  loginLib
  STATIC
  include/LoginWindow.hpp
  src/LoginWindow.cpp
)

target_include_directories(
  loginLib
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_compile_features(
  loginLib
  PUBLIC
  cxx_std_17
)

set_target_properties(
  loginLib
  PROPERTIES
  AUTOMOC ON
  AUTOUIC ON
  AUTORCC ON
)

target_link_libraries(
  loginLib
  PUBLIC
  Qt6::Widgets
)
