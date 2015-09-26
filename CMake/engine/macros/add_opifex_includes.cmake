macro(add_opifex_includes)

	set_binary_output(BINARY_FOLDER)

	include_directories(
		.
		../
		${BINARY_FOLDER}
	)

	add_external_opifex_includes()
	
endmacro(add_opifex_includes)
