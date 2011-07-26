# find dblatex
include(FindLATEX)

find_package(LATEX)
find_program(DBLATEX_EXECUTABLE 
  NAMES dblatex
  DOC "docbook to latex converter")


include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(DBLATEX
  FAIL_MESSAGE DEFAULT_MSG
  REQUIRED_VARS DBLATEX_EXECUTABLE PDFLATEX_COMPILER)

if(DBLATEX_FOUND)
  mark_as_advanced(DBLATEX_EXECUTABLE)
endif()