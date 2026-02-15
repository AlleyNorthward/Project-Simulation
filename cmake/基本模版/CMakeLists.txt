cmake_minimum_required(VERSION 3.20)
set(ENTRY "main")

project(${ENTRY})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "Generate compile_command.json" FORCE)

add_subdirectory(
  "${CMAKE_CURRENT_SOURCE_DIR}/web-page"
)

add_executable(
  ${ENTRY}
  main.cpp
)

target_link_libraries(
  ${ENTRY}
  PRIVATE
  ${interfaceLib}
  ${listLib}
  ${tableLib}
)
