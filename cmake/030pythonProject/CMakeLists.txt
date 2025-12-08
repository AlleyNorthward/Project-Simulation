cmake_minimum_required(VERSION 3.20)
set(ProjectName "CtoPython")
set(ENTRY "data_structure")
set(PYTHON "Python3")
project(${ProjectName})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "Generate compile_command.json" FORCE)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/data_structure")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/transform")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/pybind11")

find_package(${PYTHON} COMPONENTS Development REQUIRED)

pybind11_add_module(${ENTRY} bind.cpp)

# target_include_directories(
#     ${ENTRY}
#     PUBLIC
#     "${CMAKE_SOURCE_DIR}/data_structure/include"
#     "${CMAKE_SOURCE_DIR}/transform/include"
# )

target_link_libraries(
    ${ENTRY}
    PRIVATE
    ${PYTHON}::Python
    ${transformLib}
)











