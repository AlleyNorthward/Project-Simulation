if (${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
    option(IS_ROOT "是否为根?" ON)
else()
    option(IS_ROOT "是否为根?" OFF)
endif()

if (IS_ROOT)
    cmake_minimum_required(VERSION 3.20)
    project(TTesting)

    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)

    set(CURRENT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
    get_filename_component(SEARCH_DIR "${CMAKE_SOURCE_DIR}" DIRECTORY)
else()
    set(CURRENT_ROOT ${CMAKE_SOURCE_DIR})
    set(SEARCH_DIR ${CMAKE_SOURCE_DIR})
endif()

set(ENTRY "main")

add_executable(
    ${ENTRY}
    main.cpp
)

add_subdirectory("${SEARCH_DIR}/course" "${CURRENT_ROOT}/build/course")
add_subdirectory("${SEARCH_DIR}/name" "${CURRENT_ROOT}/build/name")
add_subdirectory("${SEARCH_DIR}/study" "${CURRENT_ROOT}/build/study")

target_link_libraries(
    ${ENTRY}
    ${courseLib}
    ${nameLib}
    ${study_selfLib}
    ${study_togetherLib}
)











