
macro(add_source_group FILTER_NAME SOURCE_PATH TARGET_LIST)

	file(TO_NATIVE_PATH ${FILTER_NAME} NEW_FILTER_NAME)
	if(${OPIFEX_OS_IOS})
		file(GLOB TEMP_SRC
			"${SOURCE_PATH}/*.h"
			"${SOURCE_PATH}/*.cpp"
			"${SOURCE_PATH}/*.c"
			"${SOURCE_PATH}/*.cc"
			"${SOURCE_PATH}/*.mm"
			"${SOURCE_PATH}/*.m"
		)
	else()
		file(GLOB TEMP_SRC
			"${SOURCE_PATH}/*.h"
			"${SOURCE_PATH}/*.cpp"
			"${SOURCE_PATH}/*.c"
			"${SOURCE_PATH}/*.cc"
			"${SOURCE_PATH}/*.mm"
			"${SOURCE_PATH}/*.m"
			"${SOURCE_PATH}/*.xib"
			"${SOURCE_PATH}/*.storyboard"
			"${SOURCE_PATH}/*.plist"
		)
	endif()
	source_group("${NEW_FILTER_NAME}" FILES ${TEMP_SRC})
	list(APPEND ${TARGET_LIST} "${TEMP_SRC}")

endmacro(add_source_group)
