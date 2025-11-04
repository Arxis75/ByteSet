file(GLOB SOURCES "*.h" "*.cpp"  "*.tpp")

target_sources(ByteSet PRIVATE ${SOURCES})
target_link_libraries(ByteSet PRIVATE givaro::givaro ethash::keccak OpenSSL::Crypto ${CMAKE_DL_LIBS} nlohmann_json::nlohmann_json libcurl)

target_sources(ByteSet_test PRIVATE ${SOURCES})
target_link_libraries(ByteSet_test PRIVATE givaro::givaro ethash::keccak OpenSSL::Crypto ${CMAKE_DL_LIBS} nlohmann_json::nlohmann_json libcurl)