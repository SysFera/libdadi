include(plugins)
include_directories(${SIGAR_INCLUDE_DIR})
add_plugin(SigarCori SigarCori.cc)
target_link_libraries(SigarCori ${SIGAR_LIBRARIES} dadi)
