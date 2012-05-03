include(plugins)
include_directories(${SIGAR_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
add_plugin(SigarCori SigarCori.cc)
install_plugin(SigarCori cori)

target_link_libraries(SigarCori ${SIGAR_LIBRARIES} dadi)
