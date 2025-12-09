set(transformLib "transformlib" CACHE STRING "C to CXX 转换库, 易于绑定")

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
    ${transformLib}
    STATIC
    ${LIB1_SRC}
)

target_include_directories(
    ${transformLib}
    PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(
    ${transformLib}
    PUBLIC
    ${data_structureLib}
)










