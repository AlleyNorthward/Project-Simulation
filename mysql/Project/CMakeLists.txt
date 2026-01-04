cmake_minimum_required(VERSION 3.25)
project(Hotel LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 可选择编译模块: sql login
set(COMPILE_MODULE sql)
set(USE_TEST ${COMPILE_MODULE} CACHE STRING "灵活选择编译")

find_package(Qt6 REQUIRED COMPONENTS Core Sql Widgets)

add_subdirectory(${COMPILE_MODULE})
add_subdirectory(test)
