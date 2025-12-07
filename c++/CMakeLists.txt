cmake_minimum_required(VERSION 3.15)

project(MyCppProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "Generate compile_command.json" FORCE)

# set(CMAKE_C_COMPILER "F:/Qt/Qt.6.9/Tools/mingw1310_64/bin/gcc.exe" CACHE STRING "C compiler" FORCE)
# set(CMAKE_CXX_COMPILER "F:/Qt/Qt.6.9/Tools/mingw1310_64/bin/g++.exe" CACHE STRING "C++ compiler" FORCE)

add_subdirectory(utils)
add_subdirectory(mathlib)

add_executable(MyCppApp src/main.cpp)

target_link_libraries(MyCppApp PRIVATE utils mathlib)

