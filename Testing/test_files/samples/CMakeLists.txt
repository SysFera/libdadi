file(GLOB SAMPLE_FILES "*.xml" "*.ini" "*.json")

include(copyMacros)
copy_file(${SAMPLE_FILES} DESTINATION ${TEST_FILES_OUTPUT_PATH})