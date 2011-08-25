include( plugins )

add_plugin( PersonA PersonA.cc )


file(TO_NATIVE_PATH "${TEST_FILES_OUTPUT_PATH}" TESTFILESOUTPUTPATH)

dadi_test(DADIPlugginTests)


#add_executable( loader main.cc )
#target_link_libraries( loader dadi )
