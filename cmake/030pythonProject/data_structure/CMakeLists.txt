set(data_structureLib "data_structurelib" CACHE STRING "数据结构库")

file(
    GLOB
    LIB1_SRC
    "src/*.c"
)

file(
    GLOB
    LIB1_INCLUDE
    "include/*.h"
)

add_library(
    ${data_structureLib}
    STATIC
    ${LIB1_SRC}
    ${LIB1_INCLUDE}
)


target_include_directories(
    ${data_structureLib}
    PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)



