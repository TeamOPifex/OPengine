#!/usr/bin/cmake -P

macro(temp_name fname)
  if(${ARGC} GREATER 1) # Have to escape ARGC to correctly compare
    set(_base ${ARGV1})
  else(${ARGC} GREATER 1)
    set(_base ".cmake-tmp")
  endif(${ARGC} GREATER 1)
  set(_counter 0)
  while(EXISTS "${_base}${_counter}")
    math(EXPR _counter "${_counter} + 1")
  endwhile(EXISTS "${_base}${_counter}")
  set(${fname} "${_base}${_counter}")
endmacro(temp_name)

# Evaluate expression
# Suggestion from the Wiki: http://cmake.org/Wiki/CMake/Language_Syntax
# Unfortunately, no built-in stuff for this: http://public.kitware.com/Bug/view.php?id=4034
macro(eval expr)
  temp_name(_fname)
  file(WRITE ${_fname} "${expr}")
  include(${_fname})
  file(REMOVE ${_fname})
endmacro(eval)

function(add_marketplace_addons_projects)
  foreach(ADDON ${OPENGINE_ADDONS})
    message(STATUS "************ Addon: ${ADDON}")
    include(${OPIFEX_MARKETPLACE}/${ADDON}/addon.cmake)
    string(REPLACE "." ";" VERSION_LIST ${ADDON})
    list(GET VERSION_LIST 0 ADDON_NAME)
    eval("ADDON_${ADDON_NAME}_PROJECT(${CMAKE_CURRENT_BINARY_DIR})")
  endforeach()
endfunction(add_marketplace_addons_projects)

function(add_marketplace_addons APPLICATION_TARGET FOLDER)

  foreach(ADDON ${OPENGINE_ADDONS})

    message(STATUS "************ Addon: ${ADDON}")

    include(${OPIFEX_MARKETPLACE}/${ADDON}/addon.cmake)

    string(REPLACE "." ";" VERSION_LIST ${ADDON})
    list(GET VERSION_LIST 0 ADDON_NAME)

    eval("ADDON_${ADDON_NAME}(APPLICATION_TARGET ${FOLDER})")

    SET(TEMP_RESULT "")

    eval("ADDON_${ADDON_NAME}_LINK(TEMP_RESULT)")

    message(STATUS "************ Result: ${ADDON_NAME} ${TEMP_RESULT}")

    eval("target_link_libraries(${APPLICATION_TARGET} ${TEMP_RESULT})")
    add_definitions(-DADDON_${ADDON_NAME})

    eval("ADDON_${ADDON_NAME}_DEFINES(TEMP_RESULT)")
    add_definitions(${TEMP_RESULT})

  endforeach()

endfunction(add_marketplace_addons)

message(STATUS "ADDED add_marketplace_addons FUNCTION")
