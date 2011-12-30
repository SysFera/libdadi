include(plugins)

add_plugin(SigarCori SigarCori.cc)
target_link_libraries(SigarCori ${SIGAR_LIBRARIES})
