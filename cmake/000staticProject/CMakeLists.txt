cmake_minimum_required(VERSION 3.12)
project(Testing)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/course")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/name")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/study")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src")
