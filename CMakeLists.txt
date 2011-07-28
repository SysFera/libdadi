cmake_minimum_required(VERSION "2.8.2")

project(libdadi)

# set default build type to Debug
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING
    "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
    FORCE)
endif()

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# setup Boost
find_package(Boost 1.44 REQUIRED
  date_time
  iostreams
  filesystem
  regex
  system
  thread
  program_options
  unit_test_framework)

find_package(Threads REQUIRED)
# fix DSO linking issue due to thread library
set(Boost_LIBRARIES ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
set(Boost_USE_MULTITHREADED ON)
# TODO: remove this as soon as Boost > 1.46 is required
add_definitions( "-DBOOST_FILESYSTEM_VERSION=3" )

# if we are on Apple, then we cannot build static libs
if (APPLE)
  if (NOT BUILD_SHARED_LIBS)
    message("Cannot build static libraries on Apple platforms")
  endif (NOT BUILD_SHARED_LIBS)

  set(BUILD_SHARED_LIBS ON CACHE BOOL "Build libraries as shared libraries." FORCE)
endif()

## code coverage stuff
if(ENABLE_TESTING AND CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage")
  set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -lgcov")
  set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} -lgcov")
endif()

## specific stuff for LogServiceChannel
## TODO: remove this or make it optional
option(USE_LOGSERVICE "Build libdadi with logservice support" OFF)
if(USE_LOGSERVICE)
  find_package(LOGSERVICE)
  if(LOGSERVICE_FOUND)
    add_definitions(-DUSE_LOG_SERVICE)
  else(LOGSERVICE_FOUND)
    set(LOGSERVICE_DIR "" CACHE PATH "Root of log service install tree.")
  endif (LOGSERVICE_FOUND)
  find_package( OmniORB REQUIRED )
  include( UseOmniORB )
  set( BUILD_SHARED_LIBS ON CACHE BOOL
    "Build libraries as shared libraries." FORCE )
  if( BUILD_SHARED_LIBS )
    set(OMNIORB4_LIBRARIES ${OMNIORB4_LIBRARIES_sh})
  else()
    set(OMNIORB4_LIBRARIES ${OMNIORB4_LIBRARIES_st})
  endif()
endif()

# setup documentation
include(CMakeDependentOption)
option(ENABLE_DOC "Build documentation" OFF)
# if ENABLE_DOC is enabled we build doxygen html and man pages documentation by default
cmake_dependent_option(ENABLE_DOXYGEN "Build doxygen documentation" ON
  "ENABLE_DOC" OFF)
cmake_dependent_option(ENABLE_DOXYGEN_PDF "Build doxygen documentation (PDF)" OFF
  "ENABLE_DOXYGEN" OFF)
cmake_dependent_option(ENABLE_EXTRAS_DOC "Build extrasdocumentation" OFF
  "ENABLE_DOC" OFF)

if(ENABLE_DOC)
  add_subdirectory(doc)
endif()

# set output path
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
set(MANIFEST_OUTPUT_PATH ${PROJECT_BINARY_DIR}/etc)

include_directories(${Boost_INCLUDE_DIR}
  ${PROJECT_SOURCE_DIR}/include)

set(DADI_LIBS ${Boost_LIBRARIES}  ${CMAKE_DL_LIBS})

if(USE_LOGSERVICE)
  include_directories(${LOG_INCLUDE_DIR})
  set(DADI_LIBS ${DADI_LIBS} ${OMNIORB4_LIBRARIES} ${LOGCOMPONENT_BASE_LIBRARY} ${LOG_LIBRARY})
endif()

add_subdirectory(src)
add_subdirectory(samples)

option(ENABLE_TESTING "Provide tests execution" OFF)
if(ENABLE_TESTING)
  set(TEST_FILES_OUTPUT_PATH ${PROJECT_BINARY_DIR}/Testing/test_files)
  include(dadiTestingMacros)
  dadi_test_setup()
  add_subdirectory(Testing)
endif()

# Installation
#
set(PUBLIC_HEADERS
  include/IPlugin.hh
  include/Loader.hh
  include/PluginInfo.hh
  include/Registry.hh
  include/SharedLibrary.hh
  include/config.hh
  include/options.hh
  include/singleton.hh
  include/Attributes.hh
  include/Logging.hh
  include/Logging/Channel.hh
  include/Logging/ConsoleChannel.hh
  include/Logging/FileChannel.hh
  include/Logging/FileStrategy.hh
  include/Logging/Logger.hh
  include/Logging/LogServiceChannel.hh
  include/Logging/Message.hh
  include/Logging/MultiChannel.hh
  include/Logging/NullChannel.hh
)

install(FILES ${PUBLIC_HEADERS}
  DESTINATION include/dadi
)
install(FILES include/dadi.hh
  DESTINATION include/
)
