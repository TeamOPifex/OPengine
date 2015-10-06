function(copy_file_to_binaries FILE_TO_COPY)

		file(COPY
			${OPIFEX_ENGINE_REPOSITORY}${FILE_TO_COPY}
			DESTINATION ${PROJECT_BINARY_DIR}/Binaries/${BINARY_TARGET_DIRECTORY}/${BINARY_RELEASE_MODE}/)

endfunction(copy_file_to_binaries)
