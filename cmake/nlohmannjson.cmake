set(PREFIX ${CMAKE_SOURCE_DIR}/build-external/nlohmann_json-3.12.0)
set(INSTALL ${PREFIX})

if(NOT TARGET nlohmann_json::nlohmann_json)
    message(STATUS "Third-party: creating target 'nlohmann_json'")

    set(JSON_BuildTests OFF CACHE INTERNAL "")

    include(FetchContent)
    FetchContent_Declare(
        nlohmann_json
        URL         https://github.com/nlohmann/json/archive/refs/tags/v3.12.0.tar.gz
        URL_HASH    SHA256=4b92eb0c06d10683f7447ce9406cb97cd4b453be18d7279320f7b2f025c10187
        UPDATE_DISCONNECTED true  # need this to avoid constant rebuild
        BINARY_DIR "${INSTALL}"
        SOURCE_DIR "${INSTALL}"
        SUBBUILD_DIR "${INSTALL}-subbuild"
    )
    FetchContent_MakeAvailable(nlohmann_json)
endif()