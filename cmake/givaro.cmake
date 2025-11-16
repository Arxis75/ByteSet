set(GIVARO_PREFIX ${CMAKE_SOURCE_DIR}/build-external/givaro-4.2.1)
set(GIVARO_INSTALL ${GIVARO_PREFIX}/install)
set(GIVARO_LIB_DIR ${GIVARO_INSTALL}/lib)
set(GIVARO_LIBRARY ${GIVARO_LIB_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}givaro${CMAKE_STATIC_LIBRARY_SUFFIX})
set(GIVARO_INCLUDE_DIR ${GIVARO_INSTALL}/include)

#The following test avoids the lib rebuilding when it is already available at the project level (parent host building it for example).
if(NOT TARGET givaro::givaro)
  #The following test avoids to rebuild the lib when erasing the project buid folder.
  find_library(libgivaro NAMES libgivaro.a PATHS "${GIVARO_LIB_DIR}" NO_DEFAULT_PATH)
  if(NOT libgivaro)
    message(STATUS "👉 Third-party: creating target 'givaro'")

    include(FetchContent)
    include(ProcessorCount)
    ProcessorCount(Ncpu)
    include(ExternalProject)

    set(GIVARO_SOURCE_DIR ${GIVARO_PREFIX}/src/givaro)

    ExternalProject_Add(
      givaro
      DEPENDS gmp                              #this enforces build order!
      SOURCE_DIR ${OPENSSL_SOURCE_DIR}
      PREFIX ${GIVARO_PREFIX}                  #replace ${CMAKE_CURRENT_BINARY_DIR}
      URL https://github.com/linbox-team/givaro/releases/download/v4.2.1/givaro-4.2.1.tar.gz
      URL_HASH SHA256=feefb7445842ceb756f8bb13900d975b530551e488a2ae174bda7b636251de43
      UPDATE_DISCONNECTED true          # need this to avoid constant rebuild
      CONFIGURE_COMMAND 
        ${GIVARO_PREFIX}/src/givaro/configure
        --disable-shared CFLAGS=-g CXXFLAGS=-g
        --prefix=${GIVARO_INSTALL}      #replace /usr/local
        --with-gmp=${CMAKE_SOURCE_DIR}/build-external/gmp-6.3.0/install
      BUILD_COMMAND make -j${Ncpu}
      INSTALL_COMMAND make -j${Ncpu} install
      INSTALL_DIR ${GIVARO_INSTALL}
      TEST_COMMAND ""
      BUILD_BYPRODUCTS ${GIVARO_LIBRARY}  #Mandatory for Ninja
    )
  endif()

  # We cannot use find_library because ExternalProject_Add() is performed at build time.
  # And to please the property INTERFACE_INCLUDE_DIRECTORIES,
  # we make the include directory in advance (race condition).
  file(MAKE_DIRECTORY ${GIVARO_INCLUDE_DIR})

  add_library(givaro::givaro INTERFACE IMPORTED GLOBAL)
  add_dependencies(givaro::givaro givaro)
  target_include_directories(givaro::givaro INTERFACE ${GIVARO_INCLUDE_DIR})
  target_link_libraries(givaro::givaro INTERFACE ${GIVARO_LIBRARY} gmp::gmpxx gmp::gmp)
endif()