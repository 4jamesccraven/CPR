cmake_minimum_required(VERSION 3.14)
project(cpr VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

# CLI11
FetchContent_Declare(
    CLI11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11
    GIT_TAG v2.5.0
    FIND_PACKAGE_ARGS
)

FetchContent_MakeAvailable(CLI11)
# /CLI11
file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(cpr ${SOURCES})
target_include_directories(cpr PRIVATE include)
target_link_libraries(cpr PRIVATE CLI11::CLI11)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
