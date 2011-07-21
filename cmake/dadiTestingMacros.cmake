###############################################################################
# dadi_test_setup: macro that setup our test environment
# it add a test-xml target
###############################################################################

macro(dadi_test_setup)
  include(CTest)
  enable_testing()

  if(ENABLE_REPORTS)
    # add a test-xml targets that allows us to generate Boost.Test xml reports
    add_custom_target(test-xml)
    if(NOT DEFINED REPORT_OUTPUT_PATH)
      set(REPORT_OUTPUT_PATH ${PROJECT_BINARY_DIR}/reports
        CACHE PATH "Directory where the reports will be stored")
    endif()
    file(MAKE_DIRECTORY ${REPORT_OUTPUT_PATH})
  endif()

  # set base test directory
  set(BASE_TESTS_DIR "${PROJECT_SOURCE_DIR}/Testing")
endmacro()

###############################################################################
# dadi_test: macro that setup a test
# @param[in] NAME  test filename stripped from extension
# @param[in] DISABLED (optional) when defined, remove test from master suite
#                     and display a message to the user.
###############################################################################

macro(dadi_test NAME)
  if(${ARGC} MATCHES 2)
    if(${ARGV1} MATCHES "DISABLED")
      message(STATUS "${ARGV0}: explicitly disabled by developer")
      set(${ARGV0}-DISABLED ON)
    endif()
  endif()

  if(NOT DEFINED ${ARGV0}-DISABLED)
    # create unit tests executable
    add_executable(${NAME}
      "${NAME}.cc"
      #entry point
      ${BASE_TESTS_DIR}/src/common/TestRunner.cc)

    # link libraries
    target_link_libraries(${NAME}
      dadi
      ${DADI_LIBS})

    # test executable installation has not been tested yet -sic-
    # install(TARGETS ${NAME} DESTINATION bin)

    string(REGEX REPLACE "DADI(.*)" "\\1" TEST_NAME ${NAME})

    add_test(${TEST_NAME} ${EXECUTABLE_OUTPUT_PATH}/${NAME})

    # prevent Boost.Test to catch unrelated exceptions
    set_property(TEST ${TEST_NAME}
      PROPERTY ENVIRONMENT "BOOST_TEST_CATCH_SYSTEM_ERRORS=no;")
    # just make sure that our test are run in a serial fashion
    set_property(TEST ${TEST_NAME} PROPERTY RUN_SERIAL ON)

    #
    if(ENABLE_REPORTS)
      add_custom_target(${TEST_NAME}-xml
      COMMAND ${CMAKE_COMMAND}
      -DTEST_PROG=${NAME}
      -DBIN_PATH=${EXECUTABLE_OUTPUT_PATH}
      -DREPORT_PATH=${REPORT_OUTPUT_PATH}
      -P ${PROJECT_SOURCE_DIR}/cmake/runtest.cmake)
      add_dependencies(test-xml ${TEST_NAME}-xml)
    endif()
  endif()
endmacro()
