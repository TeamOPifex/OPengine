
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
	# message(STATUS "${_FMOD_PATH}inc/")

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
		target_link_libraries(${APPLICATION_TARGET}  ${PROJECT_SOURCE_DIR}/External/FMod/lib/win64/fmod64_vc.lib)
		target_link_libraries(${APPLICATION_TARGET}  ${PROJECT_SOURCE_DIR}/External/FMod/lib/win64/fmodL64_vc.lib)
		copy_file_to_binaries(/External/FMod/lib/win64/fmod64.dll)
		copy_file_to_binaries(/External/FMod/lib/win64/fmodL64.dll)
	elseif(${OPIFEX_OS_WIN32})
		target_link_libraries(${APPLICATION_TARGET}  ${PROJECT_SOURCE_DIR}/External/FMod/lib/win32/fmod_vc.lib)
		target_link_libraries(${APPLICATION_TARGET}  ${PROJECT_SOURCE_DIR}/External/FMod/lib/win32/fmodL_vc.lib)
		copy_file_to_binaries(/External/FMod/lib/win32/fmod.dll)
		copy_file_to_binaries(/External/FMod/lib/win32/fmodL.dll)
	endif()

	if(${OPIFEX_OPTION_RELEASE})
		if(${OPIFEX_OS_WIN64})
			copy_to_folder(${APPLICATION_TARGET} "/External/FMod/lib/" "fmod64.dll" "/Application/Release" ${OPIFEX_OS_WINDOWS})
			copy_to_folder(${APPLICATION_TARGET} "/External/FMod/lib/" "fmodL64.dll" "/Application/Release" ${OPIFEX_OS_WINDOWS})
		else()
			copy_to_folder(${APPLICATION_TARGET} "/External/FMod/lib/" "fmod.dll" "/Application/Release" ${OPIFEX_OS_WINDOWS})
			copy_to_folder(${APPLICATION_TARGET} "/External/FMod/lib/" "fmodL.dll" "/Application/Release" ${OPIFEX_OS_WINDOWS})
		endif()
	else()
		if(${OPIFEX_OS_WIN64})
			copy_to_folder_msvc(${APPLICATION_TARGET} "/External/FMod/lib/" "fmod64.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
			copy_to_folder_msvc(${APPLICATION_TARGET} "/External/FMod/lib/" "fmodL64.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
		else()
			copy_to_folder_msvc(${APPLICATION_TARGET} "/External/FMod/lib/" "fmod.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
			copy_to_folder_msvc(${APPLICATION_TARGET} "/External/FMod/lib/" "fmodL.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
		endif()
	endif()


endmacro(add_opifex_fmod_windows)
