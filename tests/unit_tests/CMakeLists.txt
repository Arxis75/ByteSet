file(GLOB SOURCES "*.h" "*.cpp")

target_sources(ByteSet_test PUBLIC ${SOURCES})

add_subdirectory(constructors)