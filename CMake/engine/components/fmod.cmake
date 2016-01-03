
macro(add_opifex_fmod_set_source)

	# Look for an External Path to the PhysX folder
	SET(_FMOD_SOURCE 0)
	if(NOT "${FMOD_PATH}" STREQUAL "")
		# message(STATUS "FMOD PATH WAS SET ${FMOD_PATH}")
		SET(_FMOD_SOURCE 1)
		SET(_FMOD_PATH "${FMOD_PATH}")
	else()
		# message(STATUS "FMOD PATH WAS NOT SET ${FMOD_PATH}")
		SET(_FMOD_PATH "${PROJECT_SOURCE_DIR}/External/FMOD/")
	endif()

endmacro(add_opifex_fmod_set_source)

macro(add_opifex_fmod_include)

	add_opifex_fmod_set_source()
	include_directories(${_FMOD_PATH}inc/)
	message(STATUS "   ###   FMOD INCLUDE PATH ${_FMOD_PATH}inc/")

endmacro(add_opifex_fmod_include)


function(add_opifex_fmod APPLICATION_TARGET )

	# message("   CHOOSE FMOD")

	if(NOT ${OPIFEX_OPTION_FMOD})
		# message(STATUS "   FMOD NOT SELECTED")
		RETURN()
	endif()

	add_opifex_fmod_set_source()

	message(STATUS "Link FMOD")

    if(${OPIFEX_OS_WINDOWS})
		# message(STATUS "Adding Windows FMOD")
		add_opifex_fmod_windows(${APPLICATION_TARGET})
    else()
		# message(STATUS "Adding OSX FMOD")
		add_opifex_fmod_osx(${APPLICATION_TARGET})
	endif()

endfunction(add_opifex_fmod)


macro(add_opifex_fmod_osx APPLICATION_TARGET)

	if(${OPIFEX_OS_OSX64})

		# message(STATUS "Adding OSX 64 FMOD")

		SET(_FMOD_BINARY_LOCATION "${OPIFEX_ENGINE_REPOSITORY}/External/FMOD/lib/debug/osx64/")
		if(_FMOD_SOURCE)
			SET(_FMOD_BINARY_LOCATION "${FMOD_PATH}/lib/")
		endif()

		message(STATUS "Looking for FMOD libs in ${_FMOD_BINARY_LOCATION}")

		copy_to_binaries(${_FMOD_BINARY_LOCATION}libfmod.dylib)
		copy_to_binaries(${_FMOD_BINARY_LOCATION}libfmodL.dylib)
		link_from_binaries(${APPLICATION_TARGET}
			libfmod.dylib
			libfmodL.dylib
			)

	endif()

endmacro(add_opifex_fmod_osx)


macro(add_opifex_fmod_windows APPLICATION_TARGET)


	if(${OPIFEX_OS_WIN64})

		SET(_FMOD_BINARY_LOCATION "${OPIFEX_ENGINE_REPOSITORY}/External/FMOD/lib/debug/win64/")
		if(_FMOD_SOURCE)
			SET(_FMOD_BINARY_LOCATION "${_FMOD_PATH}\\lib\\")
		endif()

		copy_to_binaries(${_FMOD_BINARY_LOCATION}fmod64_vc.lib)
		copy_to_binaries(${_FMOD_BINARY_LOCATION}fmodL64_vc.lib)

		copy_to_binaries(${_FMOD_BINARY_LOCATION}fmod64.dll)
		copy_to_binaries(${_FMOD_BINARY_LOCATION}fmodL64.dll)


		link_from_binaries(${APPLICATION_TARGET}
			fmod64_vc.lib
			fmodL64_vc.lib
			)

		copy_from_binaries(${APPLICATION_TARGET} "fmod64.dll" "/Application/Debug")
		copy_from_binaries(${APPLICATION_TARGET} "fmodL64.dll" "/Application/Debug")

	elseif(${OPIFEX_OS_WIN32})


		if(${OPIFEX_OPTION_RELEASE})
			SET(_FMOD_BINARY_LOCATION "${OPIFEX_ENGINE_REPOSITORY}/External/FMOD/lib/release/win32/")
		else()
			SET(_FMOD_BINARY_LOCATION "${OPIFEX_ENGINE_REPOSITORY}/External/FMOD/lib/debug/win32/")
		endif()

		if(_FMOD_SOURCE)
			SET(_FMOD_BINARY_LOCATION "${_FMOD_PATH}\\lib\\")
		endif()

		copy_to_binaries(${_FMOD_BINARY_LOCATION}fmod_vc.lib)
		copy_to_binaries(${_FMOD_BINARY_LOCATION}fmodL_vc.lib)

		copy_to_binaries(${_FMOD_BINARY_LOCATION}fmod.dll)
		copy_to_binaries(${_FMOD_BINARY_LOCATION}fmodL.dll)

		link_from_binaries(${APPLICATION_TARGET}
			fmod_vc.lib
			fmodL_vc.lib
			)

		if(${OPIFEX_OPTION_RELEASE})
			copy_from_binaries(${APPLICATION_TARGET} "fmod.dll" "/Application/Release")
			copy_from_binaries(${APPLICATION_TARGET} "fmodL.dll" "/Application/Release")
		else()
			copy_from_binaries(${APPLICATION_TARGET} "fmod.dll" "/Application/Debug")
			copy_from_binaries(${APPLICATION_TARGET} "fmodL.dll" "/Application/Debug")
		endif()

	endif()


endmacro(add_opifex_fmod_windows)
