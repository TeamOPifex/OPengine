macro(add_opifex_targets APPLICATION_TARGET)

	if(NOT ${OPIFEX_OS_IOS})
		target_link_libraries(${APPLICATION_TARGET}	${OPENGL_LIBRARY} glfw ${GLFW_LIBRARIES} GLEW_158)
	endif()

	if( ${OPIFEX_OS_WINDOWS} )
		target_link_libraries(${APPLICATION_TARGET}	Winmm.lib)
	endif()

endmacro(add_opifex_targets)
