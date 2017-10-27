
macro(add_source_group FILTER_NAME SOURCE_PATH TARGET_LIST)

	file(TO_NATIVE_PATH "${FILTER_NAME}" NEW_FILTER_NAME)
	if(${OPIFEX_OS_LINUX})
		file(GLOB TEMP_SRC
			"${SOURCE_PATH}/*.h"
			"${SOURCE_PATH}/*.cpp"
			"${SOURCE_PATH}/*.c"
			"${SOURCE_PATH}/*.cc"
			"${SOURCE_PATH}/*.ico"
			"${SOURCE_PATH}/*.rc"
		)
	else()
		file(GLOB TEMP_SRC
			"${SOURCE_PATH}/*.h"
			"${SOURCE_PATH}/*.cpp"
			"${SOURCE_PATH}/*.c"
			"${SOURCE_PATH}/*.cc"
			"${SOURCE_PATH}/*.mm"
			"${SOURCE_PATH}/*.m"
			"${SOURCE_PATH}/*.ico"
			"${SOURCE_PATH}/*.rc"
		)
	endif()
	source_group("${NEW_FILTER_NAME}" FILES ${TEMP_SRC})
	list(APPEND ${TARGET_LIST} "${TEMP_SRC}")

	#message(STATUS "GLOBBED for ${FILTER_NAME}")
  #FOREACH(path ${TEMP_SRC})
		#message(STATUS "   PATH: ${path}")
	#ENDFOREACH()

endmacro(add_source_group)

macro(add_source_group_recurse ROOT_PATH SOURCE_PATH TARGET_LIST)
	if(${OPIFEX_OS_LINUX})
		FILE(GLOB_RECURSE new_list
			"${ROOT_PATH}/${SOURCE_PATH}/*.h"
			"${ROOT_PATH}/${SOURCE_PATH}/*.cpp"
			"${ROOT_PATH}/${SOURCE_PATH}/*.c"
			"${ROOT_PATH}/${SOURCE_PATH}/*.cc"
			"${ROOT_PATH}/${SOURCE_PATH}/*.ico"
			"${ROOT_PATH}/${SOURCE_PATH}/*.rc")
	else()
		FILE(GLOB_RECURSE new_list
			"${ROOT_PATH}/${SOURCE_PATH}/*.h"
			"${ROOT_PATH}/${SOURCE_PATH}/*.cpp"
			"${ROOT_PATH}/${SOURCE_PATH}/*.c"
			"${ROOT_PATH}/${SOURCE_PATH}/*.cc"
			"${ROOT_PATH}/${SOURCE_PATH}/*.mm"
			"${ROOT_PATH}/${SOURCE_PATH}/*.m"
			"${ROOT_PATH}/${SOURCE_PATH}/*.ico"
			"${ROOT_PATH}/${SOURCE_PATH}/*.rc")
	endif()
    SET(dir_list "")
    FOREACH(file_path ${new_list})
        GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
        SET(dir_list ${dir_list} ${dir_path})
    ENDFOREACH()
    LIST(REMOVE_DUPLICATES dir_list)
    #SET(${TARGET_LIST} ${dir_list})

    FOREACH(dir_path ${dir_list})

			message(STATUS "dir_path: ${dir_path}")
				#message(STATUS "PATH: ${dir_path} == ${SOURCE_PATH}")
        #if(${dir_path} STREQUAL ${SOURCE_PATH})
        #    add_source_group(/ ${SOURCE_PATH} ${TARGET_LIST})
            # message(STATUS "PATH: /")
        #else()
            string(LENGTH ${ROOT_PATH} source_path_len)
            string(LENGTH ${dir_path} dir_path_len)
				#message(STATUS "lens: ${source_path_len}, ${dir_path_len}")

            # math(EXPR len "${dir_path_len} - ${source_path_len}")
            string(SUBSTRING ${dir_path} ${source_path_len} ${dir_path_len} trunc_dir_path)

						#message(STATUS "Substring: ${dir_path} ${source_path_len} ${dir_path_len} == ${trunc_dir_path}")
            # string(SUBSTRING ${trunc_dir_path} 1 -1 trunc_dir_path)
						SET(EMPTY_STR "")
						if(trunc_dir_path STREQUAL EMPTY_STR)
							SET(trunc_dir_path "/")
						endif()

            add_source_group(${trunc_dir_path} ${dir_path} ${TARGET_LIST})
            # message(STATUS "PATH: ${trunc_dir_path} || ${dir_path}")
        #endif()
    ENDFOREACH()

endmacro(add_source_group_recurse)
