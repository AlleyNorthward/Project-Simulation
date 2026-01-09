cmake_minimum_required(VERSION 3.25)

add_library(
  pagesLib
  STATIC
  include/ShowPages.hpp
  src/ShowPages.cpp
  include/MainPage.hpp
  src/MainPage.cpp
)

target_include_directories(
  pagesLib
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_compile_features(
  pagesLib
  PUBLIC
  cxx_std_17
)

set_target_properties(
  pagesLib
  PROPERTIES
  AUTOMOC ON
  AUTOUIC ON
  AUTORCC ON
)

target_link_libraries(
  pagesLib
  PUBLIC
  utilsLib
  Qt6::Widgets
  loginLib
)
