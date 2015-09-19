
macro(add_opifex_definitions APPLICATION_TARGET APPLICATION_DIR_DEPTH )

	add_definitions(-DGLEW_STATIC -D_CRT_SECURE_NO_WARNINGS)

	if(${OPIFEX_OPTION_RELEASE})

	else()
		if(${OPIFEX_OS_WINDOWS})
			SET(OPIFEX_REPO "${APPLICATION_DIR_DEPTH}${OPIFEX_REPOSITORY}/Assets/")
			add_definitions(-DOPIFEX_REPO="${OPIFEX_REPO}")
		else()
			SET(OPIFEX_REPO "${OPIFEX_REPOSITORY}/Assets/")
			add_definitions(-DOPIFEX_REPO="${OPIFEX_REPO}")
		endif()
	endif()

	set_target_properties(${APPLICATION_TARGET} PROPERTIES LINKER_LANGUAGE CXX)

	if(${OPIFEX_OPTION_RELEASE})
		if(${OPIFEX_OS_WINDOWS})
			add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)
			target_link_libraries(${APPLICATION_TARGET}	libcpmtd0.lib)
		endif()
	endif()

endmacro(add_opifex_definitions)
