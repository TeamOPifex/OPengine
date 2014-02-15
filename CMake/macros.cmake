
macro(add_source_group FILTER_NAME SOURCE_PATH TARGET_LIST)

	file(TO_NATIVE_PATH ${FILTER_NAME} NEW_FILTER_NAME)
	file(GLOB TEMP_SRC
		"${SOURCE_PATH}/*.h"
		"${SOURCE_PATH}/*.cpp"
		"${SOURCE_PATH}/*.c"
		"${SOURCE_PATH}/*.mm"
	)
	source_group("${NEW_FILTER_NAME}" FILES ${TEMP_SRC})
	list(APPEND ${TARGET_LIST} "${TEMP_SRC}")

endmacro(add_source_group)

macro(copy_source_group SOURCE_PATH DESTINATION_TARGET)

	message(STATUS "COPY FROM '${SOURCE_PATH}'' to '${DESTINATION_TARGET}'")
	file(GLOB TEMP_SRC "${SOURCE_PATH}/*")
	file(COPY ${TEMP_SRC} DESTINATION ${DESTINATION_TARGET}/)

endmacro(copy_source_group)

macro(OPIFEX_EVAL name)
   IF(${ARGN})
     SET(${name} 1)
   ELSE(${ARGN})
     SET(${name} 0)
   ENDIF(${ARGN})
endmacro(OPIFEX_EVAL)

macro(populate_binary_directory)
	if("${OPIFEX_OS}" STREQUAL "OPIFEX_WIN32")
		SET(BINARY_TARGET_DIRECTORY "win32")
	elseif("${OPIFEX_OS}" STREQUAL "OPIFEX_WIN64")
		SET(BINARY_TARGET_DIRECTORY "win64")
	elseif("${OPIFEX_OS}" STREQUAL "OPIFEX_OSX32")
		SET(BINARY_TARGET_DIRECTORY "osx32")
	elseif("${OPIFEX_OS}" STREQUAL "OPIFEX_OSX64")
		SET(BINARY_TARGET_DIRECTORY "osx64")
	elseif("${OPIFEX_OS}" STREQUAL "OPIFEX_LINUX32")
		SET(BINARY_TARGET_DIRECTORY "linux32")
	elseif("${OPIFEX_OS}" STREQUAL "OPIFEX_LINUX64")
		SET(BINARY_TARGET_DIRECTORY "linux64")
	endif()
endmacro(populate_binary_directory)

macro(output_library APPLICATION_TARGET LIBRARY_NAME )

	if( "${OPIFEX_OS}" STREQUAL "${OPIFEX_ANDROID}" )

	else()
		populate_binary_directory()
		SET(COPY_BINARY_RELATIVE_DIRECTORY "/${LIBRARY_NAME}/")
		SET(COPY_BINARY_LIBRARY "lib${LIBRARY_NAME}.a")

		if("${OPIFEX_OS}" STREQUAL "OPIFEX_WIN32" OR "${OPIFEX_OS}" STREQUAL "OPIFEX_WIN64")
			SET(COPY_BINARY_RELATIVE_DIRECTORY "/${LIBRARY_NAME}/Debug/")
			SET(COPY_BINARY_LIBRARY "${LIBRARY_NAME}.lib")
		endif()

		add_custom_command(TARGET ${APPLICATION_TARGET} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${PROJECT_BINARY_DIR}${COPY_BINARY_RELATIVE_DIRECTORY}${COPY_BINARY_LIBRARY}"
			${PROJECT_BINARY_DIR}/Binaries/${BINARY_TARGET_DIRECTORY}/)
	endif()

endmacro(output_library)


macro(output_binary APPLICATION_TARGET RELATIVE_PATH FILE_PATH OPIFEX_MATCH )

	if( ${OPIFEX_MATCH} )
		populate_binary_directory()
		add_custom_command(TARGET ${APPLICATION_TARGET} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${PROJECT_SOURCE_DIR}${RELATIVE_PATH}${BINARY_TARGET_DIRECTORY}/${FILE_PATH}"
			${PROJECT_BINARY_DIR}/Binaries/${BINARY_TARGET_DIRECTORY}/)
	endif()

endmacro(output_binary)


macro(copy_to_folder APPLICATION_TARGET RELATIVE_PATH FILE_PATH OUTPUT_PATH OPIFEX_MATCH )

	
	if( ${OPIFEX_MATCH} )
		populate_binary_directory()
		add_custom_command(TARGET ${APPLICATION_TARGET} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${PROJECT_SOURCE_DIR}${RELATIVE_PATH}${BINARY_TARGET_DIRECTORY}/${FILE_PATH}"
			${PROJECT_BINARY_DIR}${OUTPUT_PATH})
			
	endif()

endmacro(copy_to_folder)