include_directories(
${Boost_INCLUDE_DIR}
)

include(dadiTestingMacros)
diet_test_setup()
diet_test(dadi_test)
