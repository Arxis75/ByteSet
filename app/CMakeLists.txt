set(PROJECT_SOURCES main.cpp)

add_executable(ByteSetApp ${PROJECT_SOURCES})

#Defines the root folder(s) for all targets
target_include_directories(
    ByteSetApp
    PUBLIC ${PROJECT_SOURCE_DIR}/include
    PUBLIC ${PROJECT_SOURCE_DIR}/src      #PRIVATE ${PROJECT_SOURCE_DIR}/src
)

#target_link_libraries(ByteSetApp PUBLIC LibByteSet)
target_link_libraries(ByteSetApp PUBLIC LibByteSet PRIVATE nlohmann_json::nlohmann_json libcurl)    #for testing purpose