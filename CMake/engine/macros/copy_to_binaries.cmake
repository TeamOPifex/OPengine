function(copy_to_binaries FILE_TO_COPY)

	string(REPLACE "\\" "/" outputVar "${FILE_TO_COPY}")
	SET(COPY_DIR "${PROJECT_BINARY_DIR}/Binaries/${BINARY_TARGET_DIRECTORY}/${BINARY_RELEASE_MODE}")

	if(${OPIFEX_OS_WINDOWS})
		string(REPLACE "/" "\\" fileToCopyOver "${outputVar}")
		string(REPLACE "/" "\\" dirToCopyTo "${COPY_DIR}")
	endif()

	file(COPY
		${fileToCopyOver}
	DESTINATION ${dirToCopyTo})

endfunction(copy_to_binaries)
