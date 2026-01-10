cmake_minimum_required(VERSION 3.25)

add_library(
  tabsLib
  STATIC
  include/HomeTab.hpp
  src/HomeTab.cpp
  include/RoomsTab.hpp
  src/RoomsTab.cpp
  include/BookingTab.hpp
  src/BookingTab.cpp
  include/OrderTab.hpp
  src/OrderTab.cpp
  include/SettingsTab.hpp
  src/SettingsTab.cpp
)

target_include_directories(
  tabsLib
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_compile_features(
  tabsLib
  PUBLIC
  cxx_std_17
)

set_target_properties(
  tabsLib
  PROPERTIES
  AUTOMOC ON
  AUTOUIC ON
  AUTORCC ON
)

target_link_libraries(
  tabsLib
  PUBLIC
  utilsLib
  sqlLib
  Qt6::Widgets
)
