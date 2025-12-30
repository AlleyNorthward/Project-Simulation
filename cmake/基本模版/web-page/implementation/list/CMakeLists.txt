set(listLib "listlib" CACHE STRING "列表模块")

file(
  GLOB
  LIB_SRC
  "src/*.cpp"
)

if(NOT LIB_SRC)
    file(WRITE "${CMAKE_CURRENT_SOURCE_DIR}/src/dummy.cpp" "// dummy file for CMake")
    list(APPEND LIB_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/dummy.cpp")
endif()

add_library(
  ${listLib}
  STATIC
  ${LIB_SRC}
)

target_include_directories(
  ${listLib}
  PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(
  ${listLib}
  PUBLIC
  ${interfaceLib}
)
