set(coreLib "corelib" CACHE STRING "核心库的名字")

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
    ${coreLib}
    STATIC
    ${LIB1_SRC}
    ${LIB1_INCLUDE}
)

target_include_directories(
    ${coreLib}
    PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)




















