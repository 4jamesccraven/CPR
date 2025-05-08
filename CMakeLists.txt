cmake_minimum_required(VERSION 3.10)
project(cpr VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(cpr src/main.cpp)

target_include_directories(cpr PRIVATE include)

file(GLOB_RECURSE SOURCES "src/*.cpp")

target_sources(cpr PRIVATE ${SOURCES})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
