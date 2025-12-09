cmake_minimum_required(VERSION 3.20)
set(ENTRY "main")
set(data_structureLib "data_structurelib")
project(${ENTRY})

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "Generate compile_command.json" FORCE)

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${data_structureLib}/lib/cmake")

find_package(${data_structureLib} REQUIRED)

add_executable(
    ${ENTRY}
    main.c
)

target_link_libraries(
    ${ENTRY} 
    PRIVATE 
    ${data_structureLib}::data_structurelib
)







