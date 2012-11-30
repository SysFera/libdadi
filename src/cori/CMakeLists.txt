include(plugins)
include_directories(${SIGAR_INCLUDE_DIR} ${CURL_INCLUDE_DIR} {CMAKE_CURRENT_BINARY_DIR})
add_plugin(SigarCori SigarCori.cc)
install_plugin(SigarCori cori)
add_plugin(PilgrimCori PilgrimCori.cc)
install_plugin(PilgrimCori cori)

target_link_libraries(SigarCori ${SIGAR_LIBRARIES} dadi)
target_link_libraries(PilgrimCori ${CURL_LIBRARIES} dadi)
