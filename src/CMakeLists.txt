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
  logging/NullChannel.cc
  logging/SyslogChannel.cc)

if(USE_LOGSERVICE)
  set(logging_SRCS ${logging_SRCS} logging/LogServiceChannel.cc)
endif()

set(SRCS Attributes.cc Options.cc Loader.cc Registry.cc PluginInfo.cc
  SharedLibrary.cc SharedLibraryImpl_posix.cc ${logging_SRCS})

if(WIN32)
  set(SRCS ${SRCS} SharedLibraryImpl_win32.cc)
else()
  set(SRCS ${SRCS} SharedLibraryImpl_posix.cc)
endif()

add_library(dadi ${SRCS})
target_link_libraries(dadi ${DADI_LIBS})
install(TARGETS dadi DESTINATION lib)
