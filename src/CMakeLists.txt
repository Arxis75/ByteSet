set(PROJECT_SOURCES main.cpp)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ByteSet MANUAL_FINALIZATION ${PROJECT_SOURCES})
    qt_finalize_executable(ByteSet)
else()
    add_executable(ByteSet ${PROJECT_SOURCES})
endif()

#Defines the root folder(s) for all targets
target_include_directories(ByteSet PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(ByteSet_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(data)