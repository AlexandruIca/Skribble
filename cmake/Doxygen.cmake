function(enable_doxygen)
  option(BUILD_DOCS "Build the documentation that comes with Skribble" OFF)

  if(BUILD_DOCS)
    find_package(Doxygen)

    if(Doxygen_FOUND)
      add_custom_target(
        documentation ALL
        COMMAND ${CMAKE_COMMAND} -E echo "Building docs..."
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        VERBATIM)
    else()
      message("You need doxygen to build the documentation!")
    endif()
  endif()

  option(BUILD_DOCS_MCSS
         "Build the documentation that comes with Skribble using Doxygen-mcss"
         OFF)

  if(BUILD_DOCS_MCSS)
    add_custom_target(
      documentation_mcss ALL
      COMMAND ${CMAKE_COMMAND} -E echo "Building docs using m-css..."
      COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/../m.css/documentation/doxygen.py
              ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile-mcss
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
      VERBATIM)
  endif()
endfunction()
