cmake_minimum_required(VERSION 3.16)

project(ByteSet VERSION 1.0 LANGUAGES C CXX)

# Flags must be defined AFTER the project declaration
set(CMAKE_CXX_STANDARD 17)  # 11 => 17 to allow omitting '<>' when instanciating a template class with the default parameter
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Og")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Og -Wall")

# first we can indicate the documentation build as an option and set it to ON by default
option(BUILD_DOC "Build documentation" ON)

add_subdirectory(external)
add_subdirectory(tests)
add_subdirectory(src)