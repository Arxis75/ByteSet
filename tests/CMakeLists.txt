cmake_minimum_required(VERSION 3.0)

project(ByteSet_test NONE)

add_executable(ByteSet_test main.cpp)

target_link_libraries(ByteSet_test PRIVATE GTest::gtest)

enable_testing()

add_subdirectory(unit_tests)

include(GoogleTest)
gtest_discover_tests(ByteSet_test)