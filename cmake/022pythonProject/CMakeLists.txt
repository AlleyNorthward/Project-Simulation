cmake_minimum_required(VERSION 3.20)
project(STATIC_PYTHON_TEST)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(ENTRY "example")
set(PYTHON "Python3")

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/core")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/pybind11")

find_package(${PYTHON} COMPONENTS Development REQUIRED)

pybind11_add_module(${ENTRY} example.cpp)

target_include_directories(
    ${ENTRY}
    PUBLIC
    "${CMAKE_SOURCE_DIR}/core/include"
)

target_link_libraries(${ENTRY} PRIVATE ${PYTHON}::Python)

target_link_libraries(
    ${ENTRY}
    PRIVATE
    ${coreLib}
)

