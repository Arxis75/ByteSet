cmake_minimum_required(VERSION 3.16)

project(ByteSet VERSION LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# first we can indicate the documentation build as an option and set it to ON by default
option(BUILD_DOC "Build documentation" ON)

add_subdirectory(external)
add_subdirectory(tests)
add_subdirectory(src)