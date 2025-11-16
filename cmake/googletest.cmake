set(PREFIX ${CMAKE_SOURCE_DIR}/build-external/googletest-1.17.0)
set(INSTALL ${PREFIX}/install)

#The following test avoids the lib rebuilding when it is already available at the project level (parent host building it for example).
if(NOT TARGET GTest::gtest)
    message(STATUS "👉 Third-party: creating target 'gtest'")

    include(FetchContent)
    FetchContent_Declare(
        gtest
        URL         https://github.com/google/googletest/releases/download/v1.17.0/googletest-1.17.0.tar.gz
        URL_HASH    SHA256=65fab701d9829d38cb77c14acdc431d2108bfdbf8979e40eb8ae567edf10b27c
        UPDATE_DISCONNECTED true  # need this to avoid constant rebuild
        BINARY_DIR "${INSTALL}"
        SOURCE_DIR "${INSTALL}"
        SUBBUILD_DIR "${INSTALL}-subbuild"
    )
    FetchContent_MakeAvailable(gtest)

    # Force all four gtest/gmock libraries into custom folder
    foreach(tgt gtest gtest_main gmock gmock_main)
        set_target_properties(${tgt} PROPERTIES
            ARCHIVE_OUTPUT_DIRECTORY "${INSTALL}/lib"
            LIBRARY_OUTPUT_DIRECTORY "${INSTALL}/lib"
            RUNTIME_OUTPUT_DIRECTORY "${INSTALL}/lib"
        )
    endforeach()
endif()