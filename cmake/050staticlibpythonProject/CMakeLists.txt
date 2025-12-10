cmake_minimum_required(VERSION 3.20)
set(ProjectName "CtoPython")
set(ENTRY "data_structure")
set(PYTHON "Python3")
set(data_structure_cxxLib "data_structure_cxxlib")
set(data_structureLib "data_structurelib")
project(${ENTRY})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "Generate compile_command.json" FORCE)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/pybind11")

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${data_structure_cxxLib}/lib/cmake")
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${data_structureLib}/lib/cmake")

find_package(${data_structureLib} REQUIRED)
find_package(${data_structure_cxxLib} REQUIRED)
find_package(${PYTHON} COMPONENTS Development REQUIRED)

pybind11_add_module(${ENTRY} bind.cpp)

target_link_libraries(
    ${ENTRY}
    PRIVATE
    ${data_structureLib}::data_structurelib
    ${data_structure_cxxLib}::data_structure_cxxlib
    ${PYTHON}::Python

)










