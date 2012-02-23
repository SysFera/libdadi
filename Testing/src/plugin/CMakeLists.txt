include(plugins)

add_plugin(PersonA PersonA.cc)
add_plugin(PersonB PersonB.cc)
file(TO_NATIVE_PATH "${TEST_FILES_OUTPUT_PATH}" TESTFILESOUTPUTPATH)
dadi_test(DADIPluginTests)
dadi_test(DADIPluginInfoTests)
