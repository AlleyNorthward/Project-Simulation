set(data_structure_cxxLib "data_structure_cxxlib" CACHE STRING "C++版本")
set(data_structureLib "data_structurelib")

if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(IS_ROOT TRUE)
else()
    set(IS_ROOT FALSE)
endif()

if(IS_ROOT)
    cmake_minimum_required(VERSION 3.20)
    project(${data_structure_cxxLib})
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

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

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${data_structureLib}/lib/cmake")

find_package(${data_structureLib} REQUIRED)

add_library(
    ${data_structure_cxxLib}
    STATIC
    ${LIB1_SRC}
)

target_include_directories(
    ${data_structure_cxxLib}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(
    ${data_structure_cxxLib}
    PUBLIC
    ${data_structureLib}::data_structurelib
)

if(IS_ROOT)
    install(
        TARGETS ${data_structure_cxxLib}
        EXPORT ${data_structure_cxxLib}Targets
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
    )

    install(
        FILES ${LIB1_INCLUDE}
        DESTINATION "include"
    )

    install(
        EXPORT ${data_structure_cxxLib}Targets
        FILE ${data_structure_cxxLib}Config.cmake
        NAMESPACE ${data_structure_cxxLib}::
        DESTINATION lib/cmake
    )
endif()













