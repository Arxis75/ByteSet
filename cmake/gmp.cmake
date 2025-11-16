set(GMP_PREFIX ${CMAKE_SOURCE_DIR}/build-external/gmp-6.3.0) #${CMAKE_CURRENT_BINARY_DIR})#
set(GMP_INSTALL_DIR ${GMP_PREFIX}/install)
set(GMP_LIB_DIR ${GMP_INSTALL_DIR}/lib)
set(GMP_C_LIBRARY ${GMP_LIB_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}gmp${CMAKE_STATIC_LIBRARY_SUFFIX})      # C library, mandatory for Givaro
set(GMP_CXX_LIBRARY ${GMP_LIB_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}gmpxx${CMAKE_STATIC_LIBRARY_SUFFIX})  # C++ library
set(GMP_LIBRARIES ${GMP_C_LIBRARY} ${GMP_CXX_LIBRARY})
set(GMP_INCLUDE_DIR ${GMP_INSTALL_DIR}/include)

#The following test avoids the lib rebuilding when it is already available at the project level (parent host building it for example).
if(NOT (TARGET gmp::gmp AND TARGET gmp::gmpxx))
  #The following test avoids to rebuild the lib when erasing the project buid folder.
  find_library(libgmp NAMES libgmp.a PATHS "${GMP_LIB_DIR}" NO_DEFAULT_PATH)
  find_library(libgmpxx NAMES libgmpxx.a PATHS "${GMP_LIB_DIR}" NO_DEFAULT_PATH)
  if(NOT libgmp OR NOT libgmpxx)
    message(STATUS "👉 Third-party: creating target 'gmp'")

    include(ProcessorCount)
    ProcessorCount(Ncpu)
    include(ExternalProject)

    set(GMP_SOURCE_DIR ${GMP_PREFIX}/src/gmp)
    
    ExternalProject_Add(
      gmp
      SOURCE_DIR ${OPENSSL_SOURCE_DIR}
      PREFIX ${GMP_PREFIX}                  #replace ${CMAKE_CURRENT_BINARY_DIR}
      URL https://ftp.gnu.org/gnu/gmp/gmp-6.3.0.tar.gz
      URL_HASH SHA256=e56fd59d76810932a0555aa15a14b61c16bed66110d3c75cc2ac49ddaa9ab24c
      UPDATE_DISCONNECTED true          # need this to avoid constant rebuild
      CONFIGURE_COMMAND 
        ${GMP_SOURCE_DIR}/configure 
        --disable-shared --enable-assert --enable-alloca=debug --disable-assembly CFLAGS=-g --enable-cxx CXXFLAGS=-g  # debug parameters
        --prefix=${GMP_INSTALL_DIR}     #replace /usr/local
      BUILD_COMMAND make -j${Ncpu}
      INSTALL_COMMAND make -j${Ncpu} install
      INSTALL_DIR ${GMP_INSTALL_DIR}
      TEST_COMMAND ""#make -j${Ncpu} check#
      BUILD_BYPRODUCTS ${GMP_LIBRARIES} #Mandatory for Ninja: https://stackoverflow.com/questions/54866067/cmake-and-ninja-missing-and-no-known-rule-to-make-it
    )
  endif()

  # We cannot use find_library because ExternalProject_Add() is performed at build time.
  # And to please the property INTERFACE_INCLUDE_DIRECTORIES,
  # we make the include directory in advance (race condition).
  file(MAKE_DIRECTORY ${GMP_INCLUDE_DIR})

  add_library(gmp::gmp INTERFACE IMPORTED GLOBAL)
  add_dependencies(gmp::gmp gmp)
  target_include_directories(gmp::gmp INTERFACE ${GMP_INCLUDE_DIR})
  target_link_libraries(gmp::gmp INTERFACE ${GMP_C_LIBRARY})

  add_library(gmp::gmpxx INTERFACE IMPORTED GLOBAL)
  add_dependencies(gmp::gmpxx gmp)
  target_include_directories(gmp::gmpxx INTERFACE ${GMP_INCLUDE_DIR})
  target_link_libraries(gmp::gmpxx INTERFACE ${GMP_CXX_LIBRARY})
  endif()