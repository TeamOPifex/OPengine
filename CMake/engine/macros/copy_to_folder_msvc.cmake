
macro(copy_to_folder_msvc APPLICATION_TARGET RELATIVE_PATH FILE_PATH OUTPUT_PATH OPIFEX_MATCH )

	if( ${OPIFEX_MATCH} )
		populate_binary_directory()
		add_custom_command(TARGET ${APPLICATION_TARGET} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${PROJECT_SOURCE_DIR}${RELATIVE_PATH}${BINARY_TARGET_DIRECTORY}/${FILE_PATH}"
			${PROJECT_BINARY_DIR}${OUTPUT_PATH})
	endif()

endmacro(copy_to_folder_msvc)
