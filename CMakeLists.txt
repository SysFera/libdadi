cmake_minimum_required(VERSION "2.8.4")
project(libdadi)

## cmake modules
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
include(CMakeDependentOption)

# libdadi versionning
set(DADI_VERSION_MAJOR 0)
set(DADI_VERSION_MINOR 5)
set(DADI_VERSION_PATCH 1)
set(DADI_VERSION
  "${DADI_VERSION_MAJOR}.${DADI_VERSION_MINOR}.${DADI_VERSION_PATCH}")

## set default build type to Debug
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING
    "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
    FORCE)
endif()

## code coverage stuff
if(ENABLE_TESTING AND CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage")
  set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -lgcov")
  set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} -lgcov")
endif()

## set cmake output paths
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
set(MANIFEST_OUTPUT_PATH ${PROJECT_BINARY_DIR}/etc)

## installation paths
set(LIB_SUFFIX "" CACHE STRING
  "Define libraries installation path suffix (32/64)")
set(LIB_INSTALL_DIR "lib${LIB_SUFFIX}" CACHE STRING
  "Libraries installation path")
set(INCLUDE_INSTALL_DIR "include" CACHE PATH
  "Headers installation path")
set(CMAKE_MODULES_INSTALL_DIR "share/cmake/Modules" CACHE PATH
  "CMake modules installation path")
set(PKG_CONFIG_INSTALL_DIR "${LIB_INSTALL_DIR}/pkgconfig" CACHE PATH
  "Pkg-config modules installation path")
set(PLUGINS_INSTALL_DIR "libexec" CACHE PATH
  "libdadi plugins installation path")

## options
# shared libraries
option(BUILD_SHARED_LIBS "Build shared libraries" ON)
# documentation
option(ENABLE_DOC "Build documentation" OFF)
# when ENABLE_DOC is enabled, we build by default: doxygen html and man pages
cmake_dependent_option(ENABLE_DOXYGEN "Build doxygen documentation" ON
  "ENABLE_DOC" OFF)
set(DOXYGEN_FORMATS  "html;man"
  CACHE STRING "Doxygen output formats (allowed: html;man;pdf;rtf)")
mark_as_advanced(DOXYGEN_FORMATS)
cmake_dependent_option(ENABLE_EXTRAS_DOC "Build extrasdocumentation" OFF
  "ENABLE_DOC" OFF)
# unit test suite
option(ENABLE_TESTING "Provide tests execution" OFF)

#################### Packages #################################################
## setup Boost
find_package(Boost 1.46.1 REQUIRED
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

find_package(Sigar REQUIRED)

## specific stuff for LogServiceChannel
## TODO: remove this or make it optional
option(USE_LOGSERVICE "Build libdadi with logservice support" OFF)
if(USE_LOGSERVICE)
  find_package(LOGSERVICE)
  if(LOGSERVICE_FOUND)
    add_definitions(-DUSE_LOG_SERVICE)
  else(LOGSERVICE_FOUND)
    set(LOGSERVICE_DIR "" CACHE PATH "LogService root path")
  endif (LOGSERVICE_FOUND)
  find_package( OmniORB REQUIRED )
  include(UseOmniORB)
  set(BUILD_SHARED_LIBS ON CACHE BOOL
    "Build libraries as shared libraries." FORCE)
  if( BUILD_SHARED_LIBS )
    set(OMNIORB4_LIBRARIES ${OMNIORB4_LIBRARIES_sh})
  else()
    set(OMNIORB4_LIBRARIES ${OMNIORB4_LIBRARIES_st})
  endif()
endif()

## setup include paths
include_directories(${Boost_INCLUDE_DIR}
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_BINARY_DIR}/include)
## linked libraries
set(DADI_LIBS ${Boost_LIBRARIES} ${CMAKE_DL_LIBS})
if(USE_LOGSERVICE)
  include_directories(${LOG_INCLUDE_DIR})
  set(DADI_LIBS ${DADI_LIBS}
    ${OMNIORB4_LIBRARIES} ${LOGCOMPONENT_BASE_LIBRARY} ${LOG_LIBRARY})
endif()

#################### subdirectories ###########################################
## documentation
if(ENABLE_DOC)
  add_subdirectory(doc)
endif()

## headers
file(TO_NATIVE_PATH "${CMAKE_INSTALL_PREFIX}/${PLUGINS_INSTALL_DIR}/cori"
  CORI_PLUGINS_SYSTEM_DEFAULT_PATH)
configure_file("include/dadi/dadi-config.hh.in"
  "${PROJECT_BINARY_DIR}/include/dadi/dadi-config.hh")
install(FILES "${PROJECT_BINARY_DIR}/include/dadi/dadi-config.hh"
  DESTINATION "${INCLUDE_INSTALL_DIR}/dadi"
  COMPONENT development)
# do not forget the trailing slash or it will install the whole
# directory include
install(DIRECTORY "include/"
  DESTINATION "${INCLUDE_INSTALL_DIR}"
  COMPONENT development
  FILES_MATCHING PATTERN "*.hh")

## implementations
add_subdirectory(src)
add_subdirectory(samples)

## devel utilities
# since pkg-config placeholdes may conflicts with cmake's
# i enforce the @ONLY flag
configure_file("utils/dadi.pc.in"
  "${PROJECT_BINARY_DIR}/utils/dadi.pc"
  @ONLY)
install(FILES "${PROJECT_BINARY_DIR}/utils/dadi.pc"
  DESTINATION ${PKG_CONFIG_INSTALL_DIR}
  COMPONENT development)
install(FILES "utils/FindDadi.cmake"
  DESTINATION ${CMAKE_MODULES_INSTALL_DIR}
  COMPONENT development)

## tests
if(ENABLE_TESTING)
  set(TEST_FILES_OUTPUT_PATH ${PROJECT_BINARY_DIR}/Testing/test_files)
  include(dadiTestingMacros)
  dadi_test_setup()
  add_subdirectory(Testing)
endif()

## enable uninstall target
include(CMakeUninstall)

## CPack stuff (alpha)
set(CPACK_VERSION_MAJOR ${DADI_VERSION_MAJOR})
set(CPACK_VERSION_MINOR ${DADI_VERSION_MINOR})
set(CPACK_VERSION_PATCH ${DADI_VERSION_PATCH})
set(CPACK_VERSION ${DADI_VERSION})
set(CPACK_PACKAGE_VERSION ${DADI_VERSION})
set(CPACK_PACKAGE_CONTACT "SysFera <tech@sysfera.com>")
set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${PROJECT_SOURCE_DIR}/README")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY
  "Utility library for distributed applications")
set(CPACK_PACKAGE_VENDOR "http://sysfera.com")
set(CPACK_SOURCE_IGNORE_FILES
  "~$"
  "^${PROJECT_SOURCE_DIR}/\\\\.git"
  "^${PROJECT_SOURCE_DIR}/build/")
set(CPACK_COMPONENTS_ALL runtime development)
set(CPACK_COMPONENT_RUNTIME_DISPLAY_NAME "Runtime")
set(CPACK_COMPONENT_DEVELOPMENT_DISPLAY_NAME "Development files")
set(CPACK_COMPONENT_RUNTIME_DESCRIPTION
  "Utility library for distributed applications")
set(CPACK_COMPONENT_DEVELOPMENT_DESCRIPTION "Development files for libdadi")
set(CPACK_COMPONENT_DEVELOPMENT_DEPENDS libraries)
set(CPACK_PACKAGE_FILE_NAME "libdadi-${CPACK_VERSION}-${CMAKE_SYSTEM_NAME}")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "libdadi-${CPACK_VERSION}")

if(APPLE)
  set(CPACK_BUNDLE_NAME "libdadi")
endif()
if(NOT WIN32)
  set(CPACK_GENERATOR "TGZ;TBZ2")
  set(CPACK_SOURCE_GENERATOR "TGZ;TBZ2")
else()
  include(InstallRequiredSystemLibraries)
  set(CPACK_GENERATOR "ZIP;NSIS")
  set(CPACK_SOURCE_GENERATOR "ZIP")
  set(CPACK_BINARY_CYGWIN "OFF")
  set(CPACK_SOURCE_CYGWIN "OFF")
  set(CPACK_PACKAGE_FILE_NAME "libdadi-${CPACK_VERSION}-win32")
  set(CPACK_BINARY_NSIS "ON")
  set(CPACK_NSIS_MODIFY_PATH "OFF")
  set(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY} libdadi")
  set(CPACK_NSIS_HELP_LINK "http://sysfera.com")
  set(CPACK_NSIS_URL_INFO_ABOUT "http://sysfera.com")
  set(CPACK_NSIS_CONTACT "SysFera <tech@sysfera.com>")
  set(CPACK_PACKAGE_INSTALL_DIRECTORY "libdadi")
endif()

include(CPack)
