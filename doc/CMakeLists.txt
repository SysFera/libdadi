add_custom_target(doc ALL)
if(ENABLE_DOXYGEN)
  include(UseDoxygen)
  generate_doxygen(FORMATS ${DOXYGEN_FORMATS})
endif()

# put extras documentation here
if(ENABLE_EXTRAS_DOC)
  find_package(LATEX)
  if(PDFLATEX_COMPILER)
    include(UseLATEX)
    add_latex_document("authentification-specifications.tex"
      IMAGE_DIRS fig images
      MANGLE_TARGET_NAMES
      DEFAULT_PDF)
    add_custom_target(pdf)
    add_dependencies(pdf authentification-specifications_pdf)
  endif()
endif()
