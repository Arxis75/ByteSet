file(GLOB SOURCES "*.h" "*.cpp")

target_sources(ByteSetTest PUBLIC ${SOURCES})

add_subdirectory(constructors)