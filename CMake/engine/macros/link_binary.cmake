
macro(link_from_binaries APPLICATION_TARGET)

	SET(TMP_BIN_DIR "")

	set_binary_output(TMP_BIN_DIR)

	SET(extra_macro_args ${ARGN})

	list(LENGTH extra_macro_args num_extra_args)
    if (${num_extra_args} GREATER 0)

		foreach( item IN LISTS extra_macro_args )

	        message ("Got an optional arg: ${TMP_BIN_DIR} ${item} ${APPLICATION_TARGET}")
			target_link_libraries(${APPLICATION_TARGET} ${TMP_BIN_DIR}/${item})


    	endforeach()

    endif ()


endmacro(link_from_binaries)
