
macro(copy_from_binaries_on_build APPLICATION_TARGET FILE_PATH OPIFEX_MATCH )

	if(${OPIFEX_OPTION_RELEASE})
		SET(BINARY_RELEASE_MODE "release")
	else()
		SET(BINARY_RELEASE_MODE "debug")
	endif()

	if( ${OPIFEX_MATCH} )
		populate_binary_directory()
		add_custom_command(TARGET ${APPLICATION_TARGET} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${OPIFEX_BINARIES}/${BINARY_TARGET_DIRECTORY}/${BINARY_RELEASE_MODE}/${FILE_PATH}"
			${PROJECT_BINARY_DIR}/${BINARY_RELEASE_MODE})

	endif()

endmacro(copy_from_binaries_on_build)
