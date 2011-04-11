# libdl package module sets:
# DL_FOUND 
# DL_INCLUDE_DIR 
# DL_LIBRARIES

include(CheckFunctionExists)

find_path(DL_INCLUDE_DIR
  dlfcn.h)

find_library(DL_LIBRARIES NAMES dl)

if(DL_LIBRARIES)
  set(DL_FOUND TRUE)
else()
  # if found then it's part of libc
  check_function_exists(dlopen DL_FOUND)
  set(DL_LIBRARIES "")
endif()