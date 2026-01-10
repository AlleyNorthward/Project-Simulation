cmake_minimum_required(VERSION 3.25)

add_library(
  sqlLib
  STATIC
  src/SqlConnection.cpp
  src/SqlQuery.cpp
)

target_include_directories(
  sqlLib
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_compile_features(
  sqlLib
  PUBLIC
  cxx_std_17
)

target_link_libraries(
  sqlLib
  PUBLIC
  utilsLib
  Qt6::Sql
)
