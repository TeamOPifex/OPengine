
macro(set_opifex_assets )

	# unset(OPIFEX_ASSETS)

	if(${OPIFEX_OPTION_RELEASE})
		SET(OPIFEX_ASSETS NULL)
	else()
		SET(OPIFEX_ASSETS "${PROJECT_SOURCE_DIR}/Assets/")
		# message(STATUS "OPIFEX ASSETS SET TO: ${OPIFEX_ASSETS}")
	endif()

endmacro(set_opifex_assets)
