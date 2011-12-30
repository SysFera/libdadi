add_definitions(-DMODULE_PREFIX="${CMAKE_SHARED_MODULE_PREFIX}")
add_definitions(-DMODULE_SUFFIX="${CMAKE_SHARED_MODULE_SUFFIX}")

set(logging_SRCS logging/Channel.cc
  logging/ConsoleChannel.cc
  logging/FileChannel.cc
  logging/RotateStrategy.cc
  logging/ArchiveStrategy.cc
  logging/PurgeStrategy.cc
  logging/Logger.cc
  logging/Message.cc
  logging/MultiChannel.cc
  logging/NullChannel.cc)

if(USE_LOGSERVICE)
  set(logging_SRCS ${logging_SRCS} logging/LogServiceChannel.cc)
endif()

set(SRCS Attributes.cc Options.cc Loader.cc Registry.cc PluginInfo.cc
  SharedLibrary.cc ConfigMgr.cc cori/CoriMgr.cc ${logging_SRCS})

if(WIN32)
  set(SRCS ${SRCS} SharedLibraryImpl_win32.cc)
else()
  set(SRCS ${SRCS}
    SharedLibraryImpl_posix.cc
    logging/SyslogChannel.cc)
endif()

## build libdadi
add_library(dadi ${SRCS})
target_link_libraries(dadi ${DADI_LIBS})
set_target_properties(dadi PROPERTIES
  VERSION ${DADI_VERSION}
  SOVERSION ${DADI_VERSION_MAJOR})

## installation
install(TARGETS dadi 
  COMPONENT runtime
  ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
  LIBRARY DESTINATION ${LIB_INSTALL_DIR}
  RUNTIME DESTINATION ${LIB_INSTALL_DIR})

add_subdirectory(cori)