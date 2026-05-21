cmake_minimum_required(VERSION 3.14)

project(AlgVis LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

file(GLOB ALGVIS_SRC "src/*.c")

add_library(AlgVis STATIC ${ALGVIS_SRC})

target_include_directories(AlgVis
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

install(TARGETS AlgVis
        EXPORT AlgVisTargets
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include)

install(DIRECTORY include/ DESTINATION include)

install(EXPORT AlgVisTargets
        FILE AlgVisConfig.cmake
        NAMESPACE AlgVis::
        DESTINATION lib/cmake/AlgVis)
