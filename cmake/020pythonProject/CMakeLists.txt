cmake_minimum_required(VERSION 3.20)
set(ProjectName "projectname")
set(ENTRY "example")
set(PYTHON "Python3")
project(${ProjectName})

set(CMAKE_CXX_STANDARD 17)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/pybind11")

find_package(${PYTHON} COMPONENTS Development REQUIRED)

pybind11_add_module(${ENTRY} example.cpp)

target_link_libraries(${ENTRY} PRIVATE ${PYTHON}::Python)

