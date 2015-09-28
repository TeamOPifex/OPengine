
macro(add_opifex_assets )

	set_opifex_assets()

	if(${OPIFEX_OPTION_RELEASE})
		add_definitions(-DOPIFEX_ASSETS=NULL)
	else()
		add_definitions(-D_DEBUG)
		add_definitions(-DOPIFEX_ASSETS="${OPIFEX_ASSETS}")
	endif()

endmacro(add_opifex_assets)
