macro(add_opifex_targets APPLICATION_TARGET)

	if(${OPIFEX_OS_DESKTOP})
		target_link_libraries(${APPLICATION_TARGET}	${OPENGL_LIBRARY} glfw ${GLFW_LIBRARIES} GLEW_158)
	endif()

	if(${OPIFIX_OPTION_AUDIO})
		target_link_libraries(${APPLICATION_TARGET}	${OPENAL_LIBRARY})
	endif()

	if( ${OPIFEX_OS_WINDOWS} )
		target_link_libraries(${APPLICATION_TARGET}	Winmm.lib)
	endif()

endmacro(add_opifex_targets)
