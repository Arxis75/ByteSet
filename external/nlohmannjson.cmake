set(JSON_PREFIX ${CMAKE_SOURCE_DIR}/build-external/nlohmann_json-3.12.0)
set(JSON_INSTALL ${JSON_PREFIX})

message(STATUS "Third-party: creating target 'nlohmann json'")

include(FetchContent)
FetchContent_Declare(
     json
     URL         https://github.com/nlohmann/json/archive/refs/tags/v3.12.0.tar.gz
     URL_HASH    SHA256=4b92eb0c06d10683f7447ce9406cb97cd4b453be18d7279320f7b2f025c10187
     UPDATE_DISCONNECTED true  # need this to avoid constant rebuild
     BINARY_DIR "${JSON_INSTALL}"
     SOURCE_DIR "${JSON_INSTALL}"
     SUBBUILD_DIR "${JSON_INSTALL}-subbuild"
)
FetchContent_MakeAvailable(json)

set(JSON_BuildTests OFF CACHE INTERNAL "")