#set(ETHASH_PREFIX ${CMAKE_BINARY_DIR}-external/ethash-1.0.1)
set(ETHASH_PREFIX ${CMAKE_SOURCE_DIR}/build-external/ethash-1.1.0)
set(ETHASH_INSTALL ${ETHASH_PREFIX})   #still unable to separate src/install
set(ETHASH_LIB_DIR ${ETHASH_INSTALL}/lib)
set(ETHASH_KECCAK_LIBRARY ${ETHASH_LIB_DIR}/keccak/${CMAKE_STATIC_LIBRARY_PREFIX}keccak${CMAKE_STATIC_LIBRARY_SUFFIX})
set(ETHASH_ETHASH_LIBRARY ${ETHASH_LIB_DIR}/ethash/${CMAKE_STATIC_LIBRARY_PREFIX}ethash${CMAKE_STATIC_LIBRARY_SUFFIX})
set(ETHASH_INCLUDE_DIR ${ETHASH_PREFIX}/include)

find_library(libethash NAMES libethash.a PATHS "${ETHASH_LIB_DIR}/ethash" NO_DEFAULT_PATH)
find_library(libkeccak NAMES libkeccak.a PATHS "${ETHASH_LIB_DIR}/keccak" NO_DEFAULT_PATH)
if(NOT libethash OR NOT libkeccak)
   message(STATUS "Third-party: creating target 'ethash'")

   include(FetchContent)
   FetchContent_Declare(
        ethash
        URL         https://github.com/chfast/ethash/archive/refs/tags/v1.1.0.tar.gz
        URL_HASH    SHA256=73b327f3c23f407389845d936c1138af6328c5841a331c1abe3a2add53c558aa
        UPDATE_DISCONNECTED true  # need this to avoid constant rebuild
        BINARY_DIR "${ETHASH_INSTALL}"
        SOURCE_DIR "${ETHASH_INSTALL}"
        SUBBUILD_DIR "${ETHASH_INSTALL}-subbuild"
    )
    FetchContent_MakeAvailable(ethash)
else()
    add_library(ethash::keccak INTERFACE IMPORTED GLOBAL)
    add_dependencies(ethash::keccak ethash)
    target_include_directories(ethash::keccak INTERFACE ${ETHASH_INCLUDE_DIR})
    target_link_libraries(ethash::keccak INTERFACE ${ETHASH_KECCAK_LIBRARY})
    
    #add_library(ethash::ethash INTERFACE IMPORTED GLOBAL)
    #add_dependencies(ethash::ethash ethash)
    #target_include_directories(ethash::ethash INTERFACE ${ETHASH_INCLUDE_DIR})
    #target_link_libraries(ethash::ethash INTERFACE ${ETHASH_ETHASH_LIBRARY})
endif()