include_directories(
  ${PROJECT_BINARY_DIR}/Testing/test_files/include
)

file(TO_NATIVE_PATH "${TEST_FILES_OUTPUT_PATH}" TESTFILESOUTPUTPATH)
dadi_test(DADIConfigFileTests)