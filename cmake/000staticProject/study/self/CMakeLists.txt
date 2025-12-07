set(study_selfLib "study_selflib")

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
    ${study_selfLib}
    STATIC
    ${LIB1_SRC}
    ${LIB1_INCLUDE}
)

target_include_directories(
    ${study_selfLib}
    PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)











