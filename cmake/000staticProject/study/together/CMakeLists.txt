set(study_togetherLib "study_togetherlib")

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
    ${study_togetherLib}
    STATIC
    ${LIB1_SRC}
    ${LIB1_INCLUDE}
)

target_include_directories(
    ${study_togetherLib}
    PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
