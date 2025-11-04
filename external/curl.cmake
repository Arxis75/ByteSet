#set(CURL_PREFIX ${CMAKE_BINARY_DIR}-external/ethash-1.0.1)
set(CURL_PREFIX ${CMAKE_SOURCE_DIR}/build-external/curl-8.16.0)
set(CURL_INSTALL ${CURL_PREFIX})   #still unable to separate src/install
set(CURL_LIB_DIR ${CURL_INSTALL}/lib)
set(CURL_LIBCURL_LIBRARY ${CURL_LIB_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}curl-d${CMAKE_STATIC_LIBRARY_SUFFIX})
set(CURL_INCLUDE_DIR ${CURL_PREFIX}/include)

find_library(libcurl NAMES libcurl-d.a PATHS "${CURL_LIB_DIR}" NO_DEFAULT_PATH)
#find_library(libkeccak NAMES libkeccak.a PATHS "${CURL_LIB_DIR}/keccak" NO_DEFAULT_PATH)
if(NOT libcurl)
   message(STATUS "Third-party: creating target 'curl'")

   set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build static libcurl")
   set(CURL_USE_OPENSSL ON CACHE BOOL "Use OpenSSL")
   set(CURL_USE_LIBPSL OFF CACHE BOOL "Disable use of libpsl")
   # Pass the existing OpenSSL targets to curl
   #set(CURL_OPENSSL_LIBRARIES OpenSSL::SSL OpenSSL::Crypto CACHE INTERNAL "")

   include(FetchContent)
   FetchContent_Declare(
        curl
        URL         https://github.com/curl/curl/releases/download/curl-8_16_0/curl-8.16.0.tar.gz
        URL_HASH    SHA256=a21e20476e39eca5a4fc5cfb00acf84bbc1f5d8443ec3853ad14c26b3c85b970
        UPDATE_DISCONNECTED true  # need this to avoid constant rebuild
        BINARY_DIR "${CURL_INSTALL}"
        SOURCE_DIR "${CURL_INSTALL}"
        SUBBUILD_DIR "${CURL_INSTALL}-subbuild"
    )
    FetchContent_MakeAvailable(curl)
else()
    add_library(libcurl INTERFACE IMPORTED GLOBAL)
    add_dependencies(libcurl curl)
    target_include_directories(libcurl INTERFACE ${CURL_INCLUDE_DIR})
    target_link_libraries(libcurl INTERFACE ${CURL_LIBCURL_LIBRARY})
    
    #add_library(ethash::ethash INTERFACE IMPORTED GLOBAL)
    #add_dependencies(ethash::ethash ethash)
    #target_include_directories(ethash::ethash INTERFACE ${CURL_INCLUDE_DIR})
    #target_link_libraries(ethash::ethash INTERFACE ${CURL_CURL_LIBRARY})
endif()