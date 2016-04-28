
macro(add_source_group FILTER_NAME SOURCE_PATH TARGET_LIST)

	file(TO_NATIVE_PATH "${FILTER_NAME}" NEW_FILTER_NAME)
	file(GLOB TEMP_SRC
		"${SOURCE_PATH}/*.h"
		"${SOURCE_PATH}/*.cpp"
		"${SOURCE_PATH}/*.c"
		"${SOURCE_PATH}/*.cc"
		"${SOURCE_PATH}/*.mm"
		"${SOURCE_PATH}/*.m"
	)
	source_group("${NEW_FILTER_NAME}" FILES ${TEMP_SRC})
	list(APPEND ${TARGET_LIST} "${TEMP_SRC}")

endmacro(add_source_group)

macro(add_source_group_recurse SOURCE_PATH TARGET_LIST)
    FILE(GLOB_RECURSE new_list
			"${SOURCE_PATH}/*.h"
			"${SOURCE_PATH}/*.cpp"
			"${SOURCE_PATH}/*.c"
			"${SOURCE_PATH}/*.cc"
			"${SOURCE_PATH}/*.mm"
			"${SOURCE_PATH}/*.m")
    SET(dir_list "")
    FOREACH(file_path ${new_list})
        GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
        SET(dir_list ${dir_list} ${dir_path})
    ENDFOREACH()
    LIST(REMOVE_DUPLICATES dir_list)
    SET(${TARGET_LIST} ${dir_list})

    FOREACH(dir_path ${dir_list})

        if(${dir_path} STREQUAL ${SOURCE_PATH})
            add_source_group(/ ${SOURCE_PATH} ${TARGET_LIST})
            # message(STATUS "PATH: /")
        else()
            string(LENGTH ${SOURCE_PATH} source_path_len)
            string(LENGTH ${dir_path} dir_path_len)
            math(EXPR len "${dir_path_len} - ${source_path_len}")
            string(SUBSTRING ${dir_path} ${source_path_len} ${dir_path_len} trunc_dir_path)
            add_source_group(trunc_dir_path ${dir_path} ${TARGET_LIST})
            # message(STATUS "PATH: ${trunc_dir_path}")
        endif()
    ENDFOREACH()

endmacro(add_source_group_recurse)
