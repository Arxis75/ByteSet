set(PREFIX ${CMAKE_SOURCE_DIR}/build-external/ethash-1.1.0)
set(INSTALL ${PREFIX}/install)

if(NOT TARGET ethash::ethash)
    message(STATUS "Third-party: creating target 'ethash'")

    include(FetchContent)
    FetchContent_Declare(
        ethash
        URL         https://github.com/chfast/ethash/archive/refs/tags/v1.1.0.tar.gz
        URL_HASH    SHA256=73b327f3c23f407389845d936c1138af6328c5841a331c1abe3a2add53c558aa
        UPDATE_DISCONNECTED true  # need this to avoid constant rebuild
        BINARY_DIR "${INSTALL}"
        SOURCE_DIR "${INSTALL}"
        SUBBUILD_DIR "${INSTALL}-subbuild"
    )
    FetchContent_MakeAvailable(ethash)
endif()