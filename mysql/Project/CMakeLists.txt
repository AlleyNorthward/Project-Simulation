cmake_minimum_required(VERSION 3.25)
project(Hotel LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(M "auto" CACHE STRING "外部调用短名")
set(COMPILE_MODULE ${M} CACHE STRING "内部使用完整名. 可选编译模块:login/utils/sql/auto/pages")

message(STATUS "当前编译模块:${COMPILE_MODULE}")

find_package(Qt6 REQUIRED COMPONENTS Core Sql Widgets)

if(NOT ${COMPILE_MODULE} STREQUAL "utils")
  # 如果, 单纯是为了测试utils模块, 那么那么无需添加这个目录. 如果其它模块依赖了它, 那么必须添加, 否则编译不通过. 
  # 但是现在也暴露了一个问题, 随着后续文件增多, 这里会变得愈发复杂. 又应当如何测试呢? 

  add_subdirectory(utils)
endif()

if(${COMPILE_MODULE} STREQUAL "pages")
  # 确实. 你看, 等到以后的话, 各种依赖, 各种`if`语句, 越来越多, 还能记得谁是谁吗?
  # 我觉得, 不如给给一种情况写个if语句, 这样就舒服很多了. 总之不杂乱.
  add_subdirectory(login)
endif()

if(NOT ${COMPILE_MODULE} STREQUAL "auto")
  # 这个auto, 是相当于一个灵活地可执行文件, 可以快速编译, 快速获取反馈. 它没有模块, 所以, 如果不是它, 我们需要将对应模块添加进去, 如果是它, 无需添加模块, 只需要添加子模块test即可. 
  # 但这仍存在上述问题, 文件越来越多, cmake的if语句也会越来越的. 真的有必要这样吗? 有没有cmake合理优化的方案策略呢? 
  add_subdirectory(${COMPILE_MODULE})
endif()

add_subdirectory(test)
