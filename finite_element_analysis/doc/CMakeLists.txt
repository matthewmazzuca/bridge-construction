add_custom_target(doc)
if(FEELPP_ENABLE_DOXYGEN AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/api )
  ADD_SUBDIRECTORY( api )
endif()
if(FEELPP_ENABLE_DOCUMENTATION)
ADD_SUBDIRECTORY( manual )
ADD_SUBDIRECTORY( snippets )
endif()
set(FEELPP_INSTALL_APPS ${FEELPP_INSTALL_APPS} PARENT_SCOPE)
