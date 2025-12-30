set(interfaceLib "interfacelib" CACHE STRING "接口")

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
  ${interfaceLib}
  STATIC
  ${LIB_SRC}
)

target_include_directories(
  ${interfaceLib}
  PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
  "${CMAKE_CURRENT_SOURCE_DIR}/internal"
)
