# OMNI_IDL_OUTPUT: contains generated code
macro( omni_add_include INCDIR )
  set( OMNI_INCLUDE_DIR ${OMNI_INCLUDE_DIR} ${INCDIR} )
endmacro()

macro( omni_process_idl )
  if( NOT OMNI_OUTPUT_DIR )
    set( OMNI_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR} )
  endif()
  set( IDL_FLAGS "-bcxx -Wba -Wbtp -C${OMNI_OUTPUT_DIR}/" ) 
  separate_arguments( IDL_FLAGS )
  foreach( loop_var ${ARGV} )
    message(STATUS "loop_var: ${loop_var}" )
    add_custom_command(
      OUTPUT ${loop_var}SK.cc
      ${loop_var}DynSK.cc
      ${loop_var}.hh
      COMMAND  ${OMNIORB4_IDL_COMPILER} ARGS ${IDL_FLAGS}
      ${loop_var}.idl
      DEPENDS  ${loop_var}.idl
      IMPLICIT_DEPENDS CXX ${loop_var}.idl
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
      COMMENT "idl generated code: ${loop_var}SK.cc, ${loop_var}DynSK.cc and ${loop_var}.hh")
    set( OMNI_IDL_OUTPUT ${OMNI_IDL_OUTPUT} 
      ${CMAKE_CURRENT_BINARY_DIR}/${loop_var}SK.cc
      ${CMAKE_CURRENT_BINARY_DIR}/${loop_var}DynSK.cc )
  endforeach()
endmacro()
