
macro(add_opifex_v8_set_source)

	# Look for an External Path to the V8 folder
	SET(_V8_SOURCE 0)
	if(NOT "${V8_PATH}" STREQUAL "")
		# message(STATUS "V8 PATH WAS SET ${V8_PATH}")
		SET(_V8_SOURCE 1)
		SET(_V8_PATH "${V8_PATH}")
	else()
		# message(STATUS "V8 PATH WAS NOT SET ${V8_PATH}")
		SET(_V8_PATH "${PROJECT_SOURCE_DIR}/External/V8")
	endif()

endmacro(add_opifex_v8_set_source)


macro(add_opifex_v8_include)

	add_opifex_v8_set_source()
	# message(STATUS "@@@@@@@@@:::::::: ${_V8_PATH}/include/")
	include_directories(${_V8_PATH}/)
	include_directories(${_V8_PATH}/include/)

endmacro(add_opifex_v8_include)


function(add_opifex_v8 APPLICATION_TARGET )

	if(NOT ${OPIFEX_OPTION_V8})
		RETURN()
	endif()

	add_opifex_v8_set_source()

	# if _V8_SOURCE then look for libs in out/(arch).(mode)/

	if(${OPIFEX_OS_WINDOWS})
		add_opifex_v8_windows(${APPLICATION_TARGET})
	else()
		add_opifex_v8_osx(${APPLICATION_TARGET})
	endif()

endfunction(add_opifex_v8)


macro(add_opifex_v8_osx APPLICATION_TARGET)

	# Need to make sure we're compiling with c++11 since that's what the
	# V8 engine is built with now

	SET(CMAKE_C_FLAGS "-stdlib=libstdc++ -std=c++11"	)
	SET(CMAKE_CXX_FLAGS "-stdlib=libstdc++ -std=c++11"	)


	if(${OPIFEX_OPTION_RELEASE})

		SET(_V8_BINARY_LOCATION "${OPIFEX_ENGINE_REPOSITORY}/External/V8/lib/release/osx64/")
		if(_V8_SOURCE)
			SET(_V8_BINARY_LOCATION "${V8_PATH}/out/x64.release/")
		endif()

		message(STATUS "RELEASE V8: ${V8_PATH} ${_V8_SOURCE}")



		copy_to_binaries(${_V8_BINARY_LOCATION}libv8_base.a)
		copy_to_binaries(${_V8_BINARY_LOCATION}libv8_libbase.a)
		copy_to_binaries(${_V8_BINARY_LOCATION}libv8_libplatform.a)
		copy_to_binaries(${_V8_BINARY_LOCATION}libv8_nosnapshot.a)
		copy_to_binaries(${_V8_BINARY_LOCATION}libv8_libsampler.a)
		#copy_to_binaries(${_V8_BINARY_LOCATION}libicudata.a)
		copy_to_binaries(${_V8_BINARY_LOCATION}libicuuc.a)
		copy_to_binaries(${_V8_BINARY_LOCATION}libicui18n.a)
		copy_to_binaries(${_V8_BINARY_LOCATION}libv8_external_snapshot.a)

		if(${OPIFEX_OS_64})
			link_from_binaries(${APPLICATION_TARGET}
				libv8_nosnapshot.a
				libv8_base.a
				libv8_libbase.a
				libv8_libplatform.a
                libv8_libsampler.a
				#libicudata.a
				libicuuc.a
				libicui18n.a
                libv8_external_snapshot.a)
		else()

		endif()

	else()

		SET(_V8_BINARY_LOCATION "${OPIFEX_ENGINE_REPOSITORY}/External/V8/lib/debug/osx64/")
		if(_V8_SOURCE)
			SET(_V8_BINARY_LOCATION "${V8_PATH}/out/x64.debug/")
		endif()

		message(STATUS "DEBUG V8: ${V8_PATH} ${_V8_SOURCE}")

        copy_to_binaries(${_V8_BINARY_LOCATION}libv8_base.a)
		copy_to_binaries(${_V8_BINARY_LOCATION}libv8_libbase.a)
		copy_to_binaries(${_V8_BINARY_LOCATION}libv8_libplatform.a)
		copy_to_binaries(${_V8_BINARY_LOCATION}libv8_nosnapshot.a)
		copy_to_binaries(${_V8_BINARY_LOCATION}libv8_libsampler.a)
		#copy_to_binaries(${_V8_BINARY_LOCATION}libicudata.a)
		copy_to_binaries(${_V8_BINARY_LOCATION}libicuuc.a)
		copy_to_binaries(${_V8_BINARY_LOCATION}libicui18n.a)
		copy_to_binaries(${_V8_BINARY_LOCATION}libv8_external_snapshot.a)

		if(${OPIFEX_OS_64})
			link_from_binaries(${APPLICATION_TARGET}
				libv8_nosnapshot.a
				libv8_base.a
				libv8_libbase.a
				libv8_libplatform.a
                libv8_libsampler.a
				#libicudata.a
				libicuuc.a
				libicui18n.a
                libv8_external_snapshot.a)
		else()

		endif()

	endif()

endmacro(add_opifex_v8_osx)


macro(add_opifex_v8_windows APPLICATION_TARGET)

	target_link_libraries(${APPLICATION_TARGET}
		ws2_32.lib
		advapi32.lib
		winmm.lib)


	if(${OPIFEX_OS_64})
		if(${OPIFEX_OPTION_RELEASE})

			SET(_V8_BINARY_LOCATION "${OPIFEX_ENGINE_REPOSITORY}/External/V8/lib/release/win64/")
			SET(_V8_DLL_LOCATION "${OPIFEX_ENGINE_REPOSITORY}/External/V8/lib/release/win64/")
			if(_V8_SOURCE)
				SET(_V8_BINARY_LOCATION "${V8_PATH}\\build\\Release\\lib\\")
				SET(_V8_DLL_LOCATION "${V8_PATH}\\build\\Release\\")
			endif()

			copy_to_binaries(${_V8_DLL_LOCATION}icudt.dll)

			copy_to_binaries(${_V8_BINARY_LOCATION}icuuc.lib)
			copy_to_binaries(${_V8_BINARY_LOCATION}icui18n.lib)
			copy_to_binaries(${_V8_BINARY_LOCATION}v8_libbase.lib)
			copy_to_binaries(${_V8_BINARY_LOCATION}v8_libplatform.lib)
			copy_to_binaries(${_V8_BINARY_LOCATION}v8_nosnapshot.lib)
			copy_to_binaries(${_V8_BINARY_LOCATION}v8_base_0.lib)
			copy_to_binaries(${_V8_BINARY_LOCATION}v8_base_1.lib)
			copy_to_binaries(${_V8_BINARY_LOCATION}v8_base_2.lib)
			copy_to_binaries(${_V8_BINARY_LOCATION}v8_base_3.lib)
			copy_to_binaries(${_V8_BINARY_LOCATION}v8_external_snapshot.lib)

			link_from_binaries(${APPLICATION_TARGET}
				icuuc.lib
				icui18n.lib
				v8_libbase.lib
				v8_libplatform.lib
				v8_nosnapshot.lib
				v8_base_0.lib
				v8_base_1.lib
				v8_base_2.lib
				v8_base_3.lib
				)

			copy_from_binaries(${APPLICATION_TARGET} "icudt.dll" "/Application/Release/")

		else()

			SET(_V8_BINARY_LOCATION "${OPIFEX_ENGINE_REPOSITORY}/External/V8/lib/debug/win64/")
			SET(_V8_DLL_LOCATION "${OPIFEX_ENGINE_REPOSITORY}/External/V8/lib/debug/win64/")
			if(_V8_SOURCE)
				SET(_V8_BINARY_LOCATION "${V8_PATH}\\build\\Debug\\lib\\")
				SET(_V8_DLL_LOCATION "${V8_PATH}\\build\\Debug\\")
			endif()

			copy_to_binaries(${_V8_DLL_LOCATION}icudt.dll)

			copy_to_binaries(${_V8_BINARY_LOCATION}icuuc.lib)
			copy_to_binaries(${_V8_BINARY_LOCATION}icui18n.lib)
			copy_to_binaries(${_V8_BINARY_LOCATION}v8_libbase.lib)
			copy_to_binaries(${_V8_BINARY_LOCATION}v8_libplatform.lib)
			copy_to_binaries(${_V8_BINARY_LOCATION}v8_nosnapshot.lib)
			copy_to_binaries(${_V8_BINARY_LOCATION}v8_base_0.lib)
			copy_to_binaries(${_V8_BINARY_LOCATION}v8_base_1.lib)
			copy_to_binaries(${_V8_BINARY_LOCATION}v8_base_2.lib)
			copy_to_binaries(${_V8_BINARY_LOCATION}v8_base_3.lib)
			copy_to_binaries(${_V8_BINARY_LOCATION}v8_external_snapshot.lib)

			link_from_binaries(${APPLICATION_TARGET}
				icuuc.lib
				icui18n.lib
				v8_libbase.lib
				v8_libplatform.lib
				v8_nosnapshot.lib
				v8_base_0.lib
				v8_base_1.lib
				v8_base_2.lib
				v8_base_3.lib
				)

			copy_from_binaries(${APPLICATION_TARGET} "icudt.dll" "/Application/Debug/")

		endif()
	else()

		if(${OPIFEX_OPTION_RELEASE})

			SET(_V8_BINARY_LOCATION "${OPIFEX_ENGINE_REPOSITORY}/External/V8/lib/release/win32/")
			SET(_V8_DLL_LOCATION "${OPIFEX_ENGINE_REPOSITORY}/External/V8/lib/release/win32/")
			if(_V8_SOURCE)
				SET(_V8_BINARY_LOCATION "${V8_PATH}\\build\\Release\\lib\\")
				SET(_V8_DLL_LOCATION "${V8_PATH}\\build\\Release\\")
			endif()

			copy_to_binaries(${_V8_DLL_LOCATION}icudt.dll)

			copy_to_binaries(${_V8_BINARY_LOCATION}icuuc.lib)
			copy_to_binaries(${_V8_BINARY_LOCATION}icui18n.lib)
			copy_to_binaries(${_V8_BINARY_LOCATION}v8_libbase.lib)
			copy_to_binaries(${_V8_BINARY_LOCATION}v8_libplatform.lib)
			copy_to_binaries(${_V8_BINARY_LOCATION}v8_nosnapshot.lib)
			copy_to_binaries(${_V8_BINARY_LOCATION}v8_base_0.lib)
			copy_to_binaries(${_V8_BINARY_LOCATION}v8_base_1.lib)
			copy_to_binaries(${_V8_BINARY_LOCATION}v8_base_2.lib)
			copy_to_binaries(${_V8_BINARY_LOCATION}v8_base_3.lib)
			copy_to_binaries(${_V8_BINARY_LOCATION}v8_external_snapshot.lib)

			link_from_binaries(${APPLICATION_TARGET}
				icuuc.lib
				icui18n.lib
				v8_libbase.lib
				v8_libplatform.lib
				v8_nosnapshot.lib
				v8_base_0.lib
				v8_base_1.lib
				v8_base_2.lib
				v8_base_3.lib
				)

			copy_from_binaries(${APPLICATION_TARGET} "icudt.dll" "/Application/Release/")

			# message(STATUS "SETUP WIN32 RELEASE of V8")
		else()


			SET(_V8_BINARY_LOCATION "${OPIFEX_ENGINE_REPOSITORY}/External/V8/lib/debug/win32/")
			SET(_V8_DLL_LOCATION "${OPIFEX_ENGINE_REPOSITORY}/External/V8/lib/debug/win32/")
			if(_V8_SOURCE)
				SET(_V8_BINARY_LOCATION "${V8_PATH}\\build\\Debug\\lib\\")
				SET(_V8_DLL_LOCATION "${V8_PATH}\\build\\Debug\\")
			endif()

			copy_to_binaries(${_V8_DLL_LOCATION}icudt.dll)
			#copy_to_binaries(${_V8_DLL_LOCATION}icui18n.dll)
			#copy_to_binaries(${_V8_DLL_LOCATION}icuuc.dll)

			copy_to_binaries(${_V8_BINARY_LOCATION}icuuc.lib)
			copy_to_binaries(${_V8_BINARY_LOCATION}icui18n.lib)
			copy_to_binaries(${_V8_BINARY_LOCATION}v8_libbase.lib)
			copy_to_binaries(${_V8_BINARY_LOCATION}v8_libplatform.lib)
			copy_to_binaries(${_V8_BINARY_LOCATION}v8_nosnapshot.lib)
			copy_to_binaries(${_V8_BINARY_LOCATION}v8_base_0.lib)
			copy_to_binaries(${_V8_BINARY_LOCATION}v8_base_1.lib)
			copy_to_binaries(${_V8_BINARY_LOCATION}v8_base_2.lib)
			copy_to_binaries(${_V8_BINARY_LOCATION}v8_base_3.lib)
			copy_to_binaries(${_V8_BINARY_LOCATION}v8_external_snapshot.lib)

			link_from_binaries(${APPLICATION_TARGET}
				icuuc.lib
				icui18n.lib
				v8_libbase.lib
				v8_libplatform.lib
				v8_nosnapshot.lib
				v8_base_0.lib
				v8_base_1.lib
				v8_base_2.lib
				v8_base_3.lib
				)

			copy_from_binaries(${APPLICATION_TARGET} "icudt.dll" "/Application/Debug/")

		endif()
	endif()

endmacro(add_opifex_v8_windows)
