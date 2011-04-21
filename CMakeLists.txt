cmake_minimum_required( VERSION "2.6" )

project( libutils )

set( CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake )

find_package( Boost 1.44 REQUIRED
  filesystem system thread program_options )

# TODO: remove this as soon as Boost > 1.46 is required
add_definitions( "-DBOOST_FILESYSTEM_VERSION=3" )

# set default build type to Debug
if( NOT CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE Debug CACHE STRING
    "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
    FORCE)
endif()

# set output path
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)



include_directories( ${Boost_INCLUDE_DIR}
  ${PROJECT_SOURCE_DIR}/include )

set( DADI_LIBS ${Boost_LIBRARIES}  ${CMAKE_DL_LIBS} )

add_subdirectory( src )
add_subdirectory( tests )

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
)

install(FILES ${PUBLIC_HEADERS}
  DESTINATION include/dadi
)
install(FILES include/dadi.hh
  DESTINATION include/
)
