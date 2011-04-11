set( SRCS Loader.cc Registry.cc PluginInfo.cc
  SharedLibrary.cc SharedLibraryImpl_posix.cc )
if( WIN32) 
  set( SRCS ${SRCS} SharedLibraryImpl_win32.cc )
else()
  set( SRCS ${SRCS} SharedLibraryImpl_posix.cc )
endif()

add_library( Utils ${SRCS} )
target_link_libraries( Utils ${UTILS_LIB} )