# TODO: comment
macro( add_plugin PLUGIN_NAME PLUGIN_SRCS )
  add_library( ${PLUGIN_NAME} MODULE ${PLUGIN_SRCS} )
  target_link_libraries( ${PLUGIN_NAME} ${LIBS} )
  message( STATUS "name: ${PLUGIN_NAME}")
  set( PLUGIN_NAME ${PLUGIN_NAME} )
  set( PLUGIN_PATH ${LIBRARY_OUTPUT_PATH} )
  set_target_properties( ${PLUGIN_NAME} PROPERTIES DEFINE_SYMBOL "DIET_EXPORTS" )
  configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/${PLUGIN_NAME}.xml.in ${CMAKE_CURRENT_BINARY_DIR}/${PLUGIN_NAME}.xml )
endmacro()
