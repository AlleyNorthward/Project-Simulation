set(tableLib "tablelib" CACHE STRING "表格")

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
  ${tableLib}
  STATIC
  ${LIB_SRC}
)

target_include_directories(
  ${tableLib}
  PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(
  ${tableLib}
  PUBLIC
  ${interfaceLib}
)
