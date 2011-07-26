add_custom_target(doc ALL)
if(ENABLE_DOXYGEN)
  # TODO: we should put this snippet inside a cmake module
  # do not use UseDoxygen.cmake - too limited -
  find_package(Doxygen)
  if(NOT DOXYGEN_FOUND)
    message(WARNING "In order to generate doxygen documentation "
      "You need to install doxygen")
  endif()
  set(DOXYFILE_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/doxygen)
  file(MAKE_DIRECTORY ${DOXYFILE_OUTPUT_DIR})
  # configure our Doxyfile
  configure_file(Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
  # add new doxygen target
  add_custom_target(doxygen
    COMMAND ${DOXYGEN_EXECUTABLE} Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "generate doxygen documentation")
    # we might want doxygen pdf build
  if(ENABLE_DOXYGEN_PDF)
    find_package(LATEX)
    if(LATEX_COMPILER AND MAKEINDEX_COMPILER)
      add_custom_command(TARGET doxygen
        POST_BUILD
        COMMAND "${CMAKE_BUILD_TOOL}"
        WORKING_DIRECTORY ${DOXYFILE_OUTPUT_DIR}/latex
        COMMENT "Building pdf documentation in ${DOXYFILE_OUTPUT_DIR}/latex")
    else()
      message(WARNING "You need a latex and a makeindex compiler to build "
        "PDF documentation")
    endif()
  endif()
  # add dependency to doc target
  add_dependencies(doc doxygen)
  # ensure that we clean up generated files at clean target
  set_property(DIRECTORY APPEND PROPERTY
    ADDITIONAL_MAKE_CLEAN_FILES ${DOXYFILE_OUTPUT_DIR})
endif()

# put extras documentation here
if(ENABLE_EXTRAS_DOC)
  find_package(DBLATEX)
  if(DBLATEX_FOUND)
    include(UseDBLATEX)
    dblatex_generate(OUTPUT "authentication-specifications.pdf"
      STYLE "docbook-sysfera.sty"
      TOPLEVEL_TARGET doc
      authentification.docbook)
  endif()
endif()
