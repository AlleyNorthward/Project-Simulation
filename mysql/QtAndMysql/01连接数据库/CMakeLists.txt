cmake_minimum_required(VERSION 3.20)
project(QtSqlConnection)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "Generate compile_command.json" FORCE)

find_package(Qt6 REQUIRED COMPONENTS Core Sql)

add_executable(main main.cpp)

target_link_libraries(main Qt6::Core Qt6::Sql)










