
macro(add_opifex_spine APPLICATION_TARGET )

	if(${OPIFEX_OPTION_SPINE})
		target_link_libraries(${APPLICATION_TARGET} SPINE)
	endif()

endmacro(add_opifex_spine)
