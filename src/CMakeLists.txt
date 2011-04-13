add_definitions(-DMODULE_PREFIX="${CMAKE_SHARED_MODULE_PREFIX}")
add_definitions(-DMODULE_SUFFIX="${CMAKE_SHARED_MODULE_SUFFIX}")

set( SRCS options.cc Loader.cc Registry.cc PluginInfo.cc
  SharedLibrary.cc SharedLibraryImpl_posix.cc )
if( WIN32) 
  set( SRCS ${SRCS} SharedLibraryImpl_win32.cc )
else()
  set( SRCS ${SRCS} SharedLibraryImpl_posix.cc )
endif()

add_library( dadi ${SRCS} )
target_link_libraries( dadi ${DADI_LIBS} )
