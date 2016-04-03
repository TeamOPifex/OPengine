
macro(add_opifex_raknet_set_source)

	# Look for an External Path to the PhysX folder
	SET(_RAKNET_SOURCE 0)
	if(NOT "${RAKNET_PATH}" STREQUAL "")
		# message(STATUS "FMOD PATH WAS SET ${RAKNET_PATH}")
		SET(_RAKNET_SOURCE 1)
		SET(_RAKNET_PATH "${RAKNET_PATH}")
	else()
		# message(STATUS "FMOD PATH WAS NOT SET ${RAKNET_PATH}")
		SET(_RAKNET_PATH "${PROJECT_SOURCE_DIR}/External/Raknet/")
	endif()

endmacro(add_opifex_raknet_set_source)

macro(add_opifex_raknet_include)

	add_opifex_raknet_set_source()
	include_directories(${_RAKNET_PATH}/Source/)
	message(STATUS "   ###   RAKNET INCLUDE PATH ${_RAKNET_PATH}/Source/")

endmacro(add_opifex_raknet_include)


function(add_opifex_raknet APPLICATION_TARGET )

	if(NOT ${OPIFEX_OPTION_RAKNET})
		message(STATUS "   RAKNET NOT SELECTED")
		RETURN()
	endif()

	add_opifex_raknet_set_source()

	message(STATUS "Link RAKNET")

  if(${OPIFEX_OS_WINDOWS})
		message(STATUS "Adding Windows RakNet")
		add_opifex_raknet_windows(${APPLICATION_TARGET})
  else()
		message(STATUS "Adding OSX RakNet")
		add_opifex_raknet_osx(${APPLICATION_TARGET})
	endif()

endfunction(add_opifex_raknet)


macro(add_opifex_raknet_osx APPLICATION_TARGET)

	if(${OPIFEX_OS_OSX64})

		# message(STATUS "Adding OSX 64 FMOD")

		SET(_FMOD_BINARY_LOCATION "${OPIFEX_ENGINE_REPOSITORY}/External/Raknet/lib/debug/osx64/")
		if(_RAKNET_SOURCE)
			SET(_FMOD_BINARY_LOCATION "${RAKNET_PATH}/DLL/")
		endif()

		message(STATUS "Looking for FMOD libs in ${_FMOD_BINARY_LOCATION}")

		copy_to_binaries(${_FMOD_BINARY_LOCATION}libRakNetDLL.dylib)
		link_from_binaries(${APPLICATION_TARGET}
			libRakNetDLL.dylib
			)

	endif()

endmacro(add_opifex_raknet_osx)


macro(add_opifex_raknet_windows APPLICATION_TARGET)


	if(${OPIFEX_OS_WIN64})

		SET(_RAKNET_BINARY_LOCATION "${OPIFEX_ENGINE_REPOSITORY}/External/Raknet/lib/debug/win64/")
		if(_RAKNET_SOURCE)
			SET(_RAKNET_BINARY_LOCATION "${_RAKNET_PATH}\\Lib\\")
		endif()

		copy_to_binaries(${_RAKNET_BINARY_LOCATION}RakNet_VS2008_LibStatic_Debug_x64.lib)
		#copy_to_binaries(${_RAKNET_BINARY_LOCATION}RakNet_VS2008_DLL_Debug_Win32.dll)


		link_from_binaries(${APPLICATION_TARGET}
			RakNet_VS2008_LibStatic_Debug_x64.lib
			)

		#copy_from_binaries(${APPLICATION_TARGET} "RakNet_VS2008_DLL_Debug_Win32.dll" "/Application/Debug")

	elseif(${OPIFEX_OS_WIN32})


		if(${OPIFEX_OPTION_RELEASE})
			SET(_RAKNET_BINARY_LOCATION "${OPIFEX_ENGINE_REPOSITORY}/External/Raknet/lib/release/win32/")
		else()
			SET(_RAKNET_BINARY_LOCATION "${OPIFEX_ENGINE_REPOSITORY}/External/Raknet/lib/debug/win32/")
		endif()

		if(_RAKNET_SOURCE)
			SET(_RAKNET_BINARY_LOCATION "${_RAKNET_PATH}\\lib\\")
		endif()

		copy_to_binaries(${_RAKNET_BINARY_LOCATION}libRakNetDLL.lib)

		copy_to_binaries(${_RAKNET_BINARY_LOCATION}libRakNetDLL.dll)

		link_from_binaries(${APPLICATION_TARGET}
			libRakNetDLL.lib
			)

		if(${OPIFEX_OPTION_RELEASE})
			copy_from_binaries(${APPLICATION_TARGET} "libRakNetDLL.dll" "/Application/Release")
		else()
			copy_from_binaries(${APPLICATION_TARGET} "libRakNetDLL.dll" "/Application/Debug")
		endif()

	endif()


endmacro(add_opifex_raknet_windows)
