set(PROJECT_SOURCES main.cpp)

add_executable(ByteSet ${PROJECT_SOURCES})
#add_library(ByteSet STATIC ${PROJECT_SOURCES})

#Defines the root folder(s) for all targets
target_include_directories(ByteSet PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(ByteSet_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(data)