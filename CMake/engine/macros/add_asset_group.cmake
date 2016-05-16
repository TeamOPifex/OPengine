macro(add_asset_group FILTER_NAME SOURCE_PATH TARGET_LIST)

	file(TO_NATIVE_PATH ${FILTER_NAME} NEW_FILTER_NAME)
	file(GLOB TEMP_SRC
		"${SOURCE_PATH}/*.js"
		"${SOURCE_PATH}/*.wav"
		"${SOURCE_PATH}/*.ogg"
		"${SOURCE_PATH}/*.opm"
		"${SOURCE_PATH}/*.opf"
		"${SOURCE_PATH}/*.opss"
		"${SOURCE_PATH}/*.ops"
		"${SOURCE_PATH}/*.png"
		"${SOURCE_PATH}/*.vert"
		"${SOURCE_PATH}/*.frag"
		"${SOURCE_PATH}/*.meta"
		"${SOURCE_PATH}/*.ini"
		"${SOURCE_PATH}/*.lua"
	)
	source_group("${NEW_FILTER_NAME}" FILES ${TEMP_SRC})
	list(APPEND ${TARGET_LIST} "${TEMP_SRC}")

endmacro(add_asset_group)
