
macro(find_binary OPIFEX_LIBRARY OPIFEX_NAME NOT_STATIC)
	populate_binary_directory()

	if(${OPIFEX_OPTION_RELEASE})
		SET(BINARY_RELEASE_MODE "release/")
	else()
		SET(BINARY_RELEASE_MODE "debug/")
	endif()

	if(${OPIFEX_OS_ANDROID})

		SET(OPIFEX_LIBRARY_NAME "lib${OPIFEX_NAME}.a")
		SET(${OPIFEX_LIBRARY} "${OPIFEX_BINARIES}/${BINARY_TARGET_DIRECTORY}${BINARY_RELEASE_MODE}${OPIFEX_LIBRARY_NAME}")

		# message(STATUS "SETTING '${OPIFEX_LIBRARY}' to '${OPIFEX_BINARIES}/${BINARY_TARGET_DIRECTORY}/${BINARY_RELEASE_MODE}/${OPIFEX_LIBRARY_NAME}'")

		# message(STATUS "	RESULT '${${OPIFEX_LIBRARY}}'")

	else()
		SET(OPIFEX_LIBRARY_NAME "lib${OPIFEX_NAME}.a")

        #if(${OPIFEX_IOS})
		    SET(OPIFEX_LIBRARY_NAME "${OPIFEX_NAME}")
        #endif()

		if(${OPIFEX_OPTION_SHARED})
			if(${NOT_STATIC})
				SET(OPIFEX_LIBRARY_NAME "lib${OPIFEX_NAME}.dylib")
			endif()
		endif()

		if(${OPIFEX_OS_WINDOWS})
			SET(OPIFEX_LIBRARY_NAME "${OPIFEX_NAME}.lib")
        else()
    		SET(OPIFEX_LIBRARY_NAME "lib${OPIFEX_NAME}.a")
		endif()


		SET(${OPIFEX_LIBRARY} "${OPIFEX_BINARIES}/${BINARY_TARGET_DIRECTORY}/${BINARY_RELEASE_MODE}${OPIFEX_LIBRARY_NAME}")
		#find_library( ${OPIFEX_LIBRARY} NAMES ${OPIFEX_LIBRARY_NAME} PATHS
		#"${OPIFEX_BINARIES}/${BINARY_TARGET_DIRECTORY}/${BINARY_RELEASE_MODE}/"
		#NO_DEFAULT_PATH)

		if(EXISTS ${${OPIFEX_LIBRARY}})
			#message(STATUS "Looking for '${OPIFEX_LIBRARY_NAME}' in '${OPIFEX_BINARIES}/${BINARY_TARGET_DIRECTORY}/${BINARY_RELEASE_MODE}'")
			#message(STATUS "		Found: '${${OPIFEX_LIBRARY}}'")
		else()
			message( FATAL_ERROR "Could not find: ${OPIFEX_LIBRARY} at '${${OPIFEX_LIBRARY}}'" )
		endif()

	endif()
endmacro(find_binary)
