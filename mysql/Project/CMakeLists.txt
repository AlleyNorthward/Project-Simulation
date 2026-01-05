cmake_minimum_required(VERSION 3.25)
project(Hotel LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 可选择编译模块: sql login utils
set(COMPILE_MODULE login)
set(USE_TEST ${COMPILE_MODULE} CACHE STRING "灵活选择编译")

find_package(Qt6 REQUIRED COMPONENTS Core Sql Widgets)

if(NOT ${COMPILE_MODULE} STREQUAL "utils")
  # 如果, 单纯是为了测试utils模块, 那么那么无需添加这个目录. 如果其它模块依赖了它, 那么必须添加, 否则编译不通过. 
  # 但是现在也暴露了一个问题, 随着后续文件增多, 这里会变得愈发复杂. 又应当如何测试呢? 

  add_subdirectory(utils)
endif()

add_subdirectory(${COMPILE_MODULE})
add_subdirectory(test)
