#
#    Find dadi library
#

#  The following variables are set
# DADI_FOUND		- The library was found
# DADI_INCLUDE_DIRS	- Directories where to find header files
# DADI_LIBRARIES	- Files to link against to use dadi

find_path(DADI_INCLUDE_DIR
  NAMES dadi/dadi.hh
  PATHS /usr/local/include
        /usr/include
        ${DADI_INSTALL_DIR}/include
  NO_DEFAULT_PATH
)
mark_as_advanced(DADI_INCLUDE_DIR)

find_library(DADI_LIBRARY
  NAMES ${CMAKE_SHARED_LIBRARY_PREFIX}dadi${CMAKE_SHARED_LIBRARY_SUFFIX}
  PATHS /usr/local/lib
        /usr/lib
        ${DADI_INSTALL_DIR}/lib
)
mark_as_advanced(DADI_INSTALL_LIBRARY)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(DADI DEFAULT_MSG DADI_LIBRARY DADI_INCLUDE_DIR)

if (DADI_FOUND)
  set(DADI_LIBRARIES ${DADI_LIBRARY})
  set(DADI_INCLUDE_DIRS ${DADI_INCLUDE_DIRS})
endif (DADI_FOUND)
