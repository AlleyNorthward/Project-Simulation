cmake_minimum_required(VERSION 3.25)
project(CpuSimulation LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(
  GLOB_RECURSE CpuSources CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/log/Log.cpp"
)

add_executable(
  CpuSimulation main.cpp
  ${CpuSources}
)

target_include_directories(CpuSimulation PRIVATE 
  "${CMAKE_CURRENT_SOURCE_DIR}/log"
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_compile_features(CpuSimulation PRIVATE cxx_std_20)
