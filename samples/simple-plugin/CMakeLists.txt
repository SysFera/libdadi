include( plugins )

add_plugin( PersonA PersonA.cc )

add_executable( loader main.cc )
target_link_libraries( loader dadi )