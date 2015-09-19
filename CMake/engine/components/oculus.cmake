
macro(add_opifex_oculus APPLICATION_TARGET )

	if(${OPIFEX_OPTION_OCULUS})
		include_directories(../External/Oculus/include/	../External/Oculus/include/)
		target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/OculusDK2/lib/win32/VS2010/libovrd.lib)
	endif()

endmacro(add_opifex_oculus)
