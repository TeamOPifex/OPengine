macro(add_opifex_oculus_set_source)

	# Look for an External Path to the V8 folder
	SET(_OCULUS_SOURCE 0)
	if(NOT "${OCULUS_PATH}" STREQUAL "")
		# message(STATUS "OCULUS PATH WAS SET ${OCULUS_PATH}")
		SET(_OCULUS_SOURCE 1)
		SET(_OCULUS_PATH "${OCULUS_PATH}")
	else()
		# message(STATUS "OCULUS PATH WAS NOT SET ${OCULUS_PATH}")
		SET(_OCULUS_PATH "${PROJECT_SOURCE_DIR}/External/Oculus")
	endif()

endmacro(add_opifex_oculus_set_source)

macro(add_opifex_oculus_include)

	add_opifex_oculus_set_source()
	include_directories(${_OCULUS_PATH}/Include/)

endmacro(add_opifex_oculus_include)

function(add_opifex_oculus APPLICATION_TARGET )
		if(NOT ${OPIFEX_OPTION_OCULUS})
			RETURN()
		endif()

		add_opifex_oculus_set_source()

		if(${OPIFEX_OS_WINDOWS})
			add_opifex_oculus_windows(${APPLICATION_TARGET})
		else()
			add_opifex_oculus_osx(${APPLICATION_TARGET})
		endif()

endfunction(add_opifex_oculus)



macro(add_opifex_oculus_osx APPLICATION_TARGET)

endmacro(add_opifex_oculus_osx)


macro(add_opifex_oculus_windows APPLICATION_TARGET)
	if(${OPIFEX_OS_64})
		SET(_OCULUS_BINARY_LOCATION "${OPIFEX_ENGINE_REPOSITORY}/External/Oculus/lib/debug/win64/")
		if(_OCULUS_SOURCE)
			SET(_OCULUS_BINARY_LOCATION "${OCULUS_PATH}\\Lib\\Windows\\x64\\Release\\VS2105\\")
		endif()


		copy_to_binaries(${_OCULUS_BINARY_LOCATION}LibOVR.lib)
		link_from_binaries(${APPLICATION_TARGET}
			LibOVR.lib
			)

	else()
	endif()

endmacro(add_opifex_oculus_windows)
