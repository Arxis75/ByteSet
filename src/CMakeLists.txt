file(GLOB BYTESET_PRIVATE_SRC
     CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
     "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

file(GLOB BYTESET_PUBLIC_HEADERS
     CONFIGURE_DEPENDS 
     "${PROJECT_SOURCE_DIR}/include/ByteSet/*.h"
     "${PROJECT_SOURCE_DIR}/include/ByteSet/*.tpp")

add_library(LibByteSet STATIC ${BYTESET_PRIVATE_SRC} ${BYTESET_PUBLIC_HEADERS} )

target_include_directories(LibByteSet
    PUBLIC
          $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
          $<INSTALL_INTERFACE:include>
          #${PROJECT_SOURCE_DIR}/include
    PRIVATE
          ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(LibByteSet PUBLIC givaro::givaro ethash::keccak OpenSSL::Crypto PRIVATE ${CMAKE_DL_LIBS})