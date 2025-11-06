if(BUILD_DOC)
    FIND_PACKAGE( Doxygen )
    IF(DOXYGEN_FOUND)
        MESSAGE(STATUS "Doxygen found: ${DOXYGEN_EXECUTABLE} -- ${DOXYGEN_VERSION}")
        # Set Doxygen input and output files.
        SET(DOXYGEN_INPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
        SET(DOXYGEN_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/build-docs)
        SET(DOXYGEN_INDEX_FILE ${DOXYGEN_OUTPUT_DIR}/xml/index.xml)
        SET(DOXYFILE_IN ${DOXYGEN_INPUT_DIR}/Doxyfile.in)
        SET(DOXYFILE_OUT ${CMAKE_SOURCE_DIR}/build-external/doxygen-${DOXYGEN_VERSION}/Doxyfile)
        # Generate DoxyFile from the input file.
        CONFIGURE_FILE(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)
        # Create Output directory.
        FILE(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIR})
        # Command for generating doc from Doxygen config file.
        ADD_CUSTOM_COMMAND(WORKING_DIRECTORY ${DOXYGEN_OUTPUT_DIR}
                           OUTPUT ${DOXYGEN_INDEX_FILE}
                           COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
                           MAIN_DEPENDENCY ${DOXYFILE_OUT} ${DOXYFILE_IN}
                           COMMENT "Generating Doxygen documentation"
                           VERBATIM)
        # Create CMake Target for generating doc.
        ADD_CUSTOM_TARGET(docs ALL DEPENDS ${DOXYGEN_INDEX_FILE})
    endif()
endif()