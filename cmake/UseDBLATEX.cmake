# generate pdf from docbook
include(CMakeParseArguments)

# @brief dblatex_generate: generate pdf from docbook sources
# @param[optional] STYLE latex stylesheet
# @param[optional] OUTPUT output file 
# @param[optional] TOPLEVEL_TARGET set top-level target
# (by default, it will be named after firt list element)
# @param ARGN docbook sources
macro(dblatex_generate)
  # parse named arguments
  set(options "")
  set(oneValueArgs STYLE OUTPUT TOPLEVEL_TARGET)
  set(multiValueArgs "")
  cmake_parse_arguments(dblatex
    "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
  # we need at least that macros list docbook sources to be built
  if(NOT dblatex_UNPARSED_ARGUMENTS)
    message(FATAL_ERROR
      "dblatex_generate can't generate document without any sources")
  endif()
  if(NOT dblatex_OUTPUT)
    list(GET dblatex_UNPARSED_ARGUMENTS 0 FIRST_ELT)
    string(REGEX REPLACE
      "(.*)\\.docbook" "\\1.pdf" dblatex_OUTPUT ${FIRST_ELT})
  endif()
  set(dblatex_OUTPUT_OPT
    "--output=${dblatex_OUTPUT}")
  if(dblatex_STYLE)
    set(dblatex_STYLE
      "--texstyle=${CMAKE_CURRENT_SOURCE_DIR}/${dblatex_STYLE}")
  endif()
  foreach(loop_var ${dblatex_UNPARSED_ARGUMENTS})
    set(dblatex_UNPARSED_ARGUMENTS_OPT
      "${dblatex_UNPARSED_ARGUMENTS_OPT}"
      "${CMAKE_CURRENT_SOURCE_DIR}/${loop_var}")
  endforeach()
  
  add_custom_target(${dblatex_OUTPUT})
  add_custom_command(TARGET ${dblatex_OUTPUT} 
    COMMAND ${DBLATEX_EXECUTABLE}
    ${dblatex_STYLE}
    ${dblatex_OUTPUT_OPT}
    ${dblatex_UNPARSED_ARGUMENTS_OPT}
    DEPENDS ${dblatex_UNPARSED_ARGUMENTSZ}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "generate pdf from docbook sources")
  if(dblatex_TOPLEVEL_TARGET)
    add_dependencies(${dblatex_TOPLEVEL_TARGET}
      ${dblatex_OUTPUT})
  endif()
endmacro()