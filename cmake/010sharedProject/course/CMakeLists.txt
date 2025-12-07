set(courseLib "courselib" CACHE STRING "The name of the course library")

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
    ${courseLib}
    SHARED
    ${LIB1_SRC}
    ${LIB1_INCLUDE}
)

target_include_directories(
    ${courseLib}
    PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_compile_definitions(
    ${courseLib}
    PRIVATE
    courseLibExport
)

