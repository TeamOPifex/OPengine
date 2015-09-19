
macro(add_opifex_assets APPLICATION_TARGET )

	unset(OPIFEX_ASSETS)

	if(${OPIFEX_OPTION_RELEASE})
		SET(OPIFEX_ASSETS NULL)
		add_definitions(-DOPIFEX_ASSETS=NULL)
	else()

		add_definitions(-D_DEBUG)
		SET(OPIFEX_ASSETS "${PROJECT_SOURCE_DIR}/Assets/")
		add_definitions(-DOPIFEX_ASSETS="${OPIFEX_ASSETS}")

		message(STATUS "OPIFEX ASSETS SET TO: ${OPIFEX_ASSETS}")

	endif()

endmacro(add_opifex_assets)
