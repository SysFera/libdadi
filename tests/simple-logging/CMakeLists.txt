add_executable( simple-logging main.cc )
target_link_libraries( simple-logging ${DADI_LIBS} dadi 
  ${OMNIORB4_LIBRARIES} ${LOGCOMPONENT_BASE_LIBRARY} )