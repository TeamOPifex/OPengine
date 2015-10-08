

function(copy_to_folder APPLICATION_TARGET RELATIVE_PATH FILE_PATH OUTPUT_PATH OPIFEX_MATCH )


	if( ${OPIFEX_MATCH} )
		populate_binary_directory()
		add_custom_command(TARGET ${APPLICATION_TARGET} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${PROJECT_SOURCE_DIR}${RELATIVE_PATH}${BINARY_TARGET_DIRECTORY}/${FILE_PATH}"
			${PROJECT_BINARY_DIR}${OUTPUT_PATH})

	endif()

endfunction(copy_to_folder)


function(copy_from_binaries APPLICATION_TARGET FILE_PATH OUTPUT_PATH )

	set_binary_output(TMP_BIN_DIR)

	populate_binary_directory()
	add_custom_command(TARGET ${APPLICATION_TARGET} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${TMP_BIN_DIR}/${FILE_PATH}"
	${PROJECT_BINARY_DIR}${OUTPUT_PATH})

endfunction(copy_from_binaries)
