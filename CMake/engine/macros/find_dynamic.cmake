
function(find_dynamic OPIFEX_LIBRARY OPIFEX_NAME)
	populate_binary_directory()
	SET(OPIFEX_LIBRARY_NAME "lib${OPIFEX_NAME}.dylib")


	if(${OPIFEX_OS_WINDOWS})
		SET(OPIFEX_LIBRARY_NAME "${OPIFEX_NAME}.lib")
	endif()

	if(${OPIFEX_OS_ANDROID})
		SET(BINARY_RELEASE_MODE "")
	else()
		# message(STATUS "Find Library in release? ${BINARY_RELEASE_MODE}")
		if(${OPIFEX_OPTION_RELEASE})
			SET(BINARY_RELEASE_MODE "release/")
		else()
			SET(BINARY_RELEASE_MODE "debug/")
		endif()
	endif()


	find_library( ${OPIFEX_LIBRARY} NAMES ${OPIFEX_LIBRARY_NAME} PATHS
	"${OPIFEX_BINARIES}/${BINARY_TARGET_DIRECTORY}/${BINARY_RELEASE_MODE}" PARENT_SCOPE
	NO_DEFAULT_PATH)

	# message(STATUS "Looking for '${OPIFEX_LIBRARY_NAME}' in '${OPIFEX_BINARIES}/${BINARY_TARGET_DIRECTORY}/${BINARY_RELEASE_MODE}'")
	# message(STATUS "		Found: '${${OPIFEX_LIBRARY}}'")
endfunction(find_dynamic)
