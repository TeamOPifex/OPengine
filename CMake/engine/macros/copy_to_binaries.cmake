macro(copy_to_binaries FILE_TO_COPY)

	file(COPY
		${FILE_TO_COPY}
		DESTINATION ${PROJECT_BINARY_DIR}/Binaries/${BINARY_TARGET_DIRECTORY}/${BINARY_RELEASE_MODE}/)

endmacro(copy_to_binaries)
