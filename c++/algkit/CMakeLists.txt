cmake_minimum_required(VERSION 3.14)
project(AlgKit LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB ALGKIT_SRC "src/*.cpp")

add_library(AlgKit STATIC ${ALGKIT_SRC})

target_include_directories(AlgKit
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

install(TARGETS AlgKit
        EXPORT AlgKitTargets
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include)

install(DIRECTORY include/ DESTINATION include)

install(EXPORT AlgKitTargets
        FILE AlgKitConfig.cmake
        NAMESPACE AlgKit::
        DESTINATION lib/cmake/AlgKit)
