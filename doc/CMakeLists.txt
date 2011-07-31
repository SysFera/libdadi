add_custom_target(doc ALL)
if(ENABLE_DOXYGEN)
  include(UseDoxygen)
  generate_doxygen(FORMATS ${DOXYGEN_FORMATS})
endif()

# put extras documentation here
if(ENABLE_EXTRAS_DOC)
  find_package(DBLATEX)
  if(DBLATEX_FOUND)
    include(UseDBLATEX)
    dblatex_generate(OUTPUT "authentication-specifications.pdf"
      STYLE "docbook-sysfera.sty"
      TOPLEVEL_TARGET doc
      authentification.docbook)
  endif()
endif()
