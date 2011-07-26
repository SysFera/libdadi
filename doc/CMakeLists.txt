add_custom_target(doc ALL)
if(ENABLE_DOXYGEN)
  # do not use UseDoxygen.cmake - too limited _
  find_package(Doxygen)
  if(NOT DOXYGEN_FOUND)
    message(FATAL_ERROR "In order to generate doxygen documentation"
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
