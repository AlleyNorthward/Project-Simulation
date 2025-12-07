set(nameLib "namelib" CACHE STRING "The name of the name library")

file(
    GLOB
    LIB1_SRC
    "src/*.cpp"
)

file(
    GLOB
    LIB1_INCLUDE
    "include/*.hpp"
)

add_library(
    ${nameLib}
    SHARED
    ${LIB1_SRC}
    ${LIB1_INCLUDE}
)

target_include_directories(
    ${nameLib}
    PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_compile_definitions(
    ${nameLib}
    PRIVATE
    nameLibExport
)












