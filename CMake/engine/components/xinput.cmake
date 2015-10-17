macro(add_opifex_xinput APPLICATION_TARGET )

	if(${OPIFEX_OS_WINDOWS})
		message(STATUS "link xinput")

		message(STATUS "MSVS VERSION : ${MSVC_VERSION}")

		if(${MSVC_VERSION} GREATER 1700) # vs2013 = 1800
			target_link_libraries(${APPLICATION_TARGET} XInput9_1_0.lib)
		endif()

	endif()

endmacro(add_opifex_xinput)
