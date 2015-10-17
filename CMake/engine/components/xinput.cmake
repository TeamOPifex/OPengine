macro(add_opifex_xinput APPLICATION_TARGET )

	if(${OPIFEX_OS_WINDOWS})
		message(STATUS "link xinput")
		# target_link_libraries(${APPLICATION_TARGET} XInput9_1_0.lib)
	endif()

endmacro(add_opifex_xinput)
