cmake_minimum_required(VERSION 3.16)

project(ByteSetTest VERSION 1.0 LANGUAGES CXX)

set(PROJECT_SOURCES main.cpp)

add_executable(ByteSetTest ${PROJECT_SOURCES})

#Defines the root folder(s) for all targets
target_include_directories(
    ByteSetTest
    PUBLIC ${PROJECT_SOURCE_DIR}/include
    PUBLIC ${PROJECT_SOURCE_DIR}/src      #PRIVATE ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(ByteSetTest PRIVATE LibByteSet GTest::gtest)

add_subdirectory(unit_tests)

enable_testing()

include(GoogleTest)
gtest_discover_tests(ByteSetTest)