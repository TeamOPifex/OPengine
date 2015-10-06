function(copy_to_binaries FILE_TO_COPY)

	string(REPLACE "\\" "/" outputVar "${FILE_TO_COPY}")
	file(COPY
		${outputVar}
		DESTINATION ${PROJECT_BINARY_DIR}/Binaries/${BINARY_TARGET_DIRECTORY}/${BINARY_RELEASE_MODE}/)

endfunction(copy_to_binaries)
