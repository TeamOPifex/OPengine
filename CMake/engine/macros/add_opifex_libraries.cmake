
macro(add_opifex_libraries APPLICATION_TARGET )

	unset(LIBLODEPNG CACHE)
	unset(LIBCORE CACHE)
	unset(LIBDATA CACHE)
	unset(LIBMATH CACHE)
	unset(LIBPERFORMANCE CACHE)
	unset(LIBSCRIPTING CACHE)
	unset(LIBPIPELINE CACHE)
	unset(LIBHUMAN CACHE)
	unset(LIBGLEW_158 CACHE)
	unset(LIBGLFW CACHE)
	unset(LIBJANSSON CACHE)
	unset(LIBOGG CACHE)
	unset(LIBVORBIS CACHE)
	unset(LIBVORBISFILE CACHE)
	unset(LIBV8 CACHE)
	unset(LIBV8_BASE_0 CACHE)
	unset(LIBV8_BASE_1 CACHE)
	unset(LIBV8_BASE_2 CACHE)
	unset(LIBV8_BASE_3 CACHE)
	unset(LIBV8_LIBBASE CACHE)
	unset(LIBV8_LIBPLATFORM CACHE)
	unset(LIBV8_EXTERNAL_SNAPSHOT CACHE)
	unset(LIBV8_NOSHNAPSHOT CACHE)
	unset(LIBV8_ICUDATA CACHE)
	unset(LIBV8_ICUUC CACHE)
	unset(LIBV8_ICUI18N CACHE)
	unset(LIBV8_MKSNAPSHOT CACHE)
	unset(LIBFMOD CACHE)
	unset(LIBRAKNET CACHE)
	unset(LIBLOW_LEVEL_DEBUG)
	unset(LIBLOW_LEVEL_CLOTH_DEBUG)
	unset(LIBPHYSX3_CHARACTER_KINEMATIC_DEBUG)
	unset(LIBPHYSX3_COMMON_DEBUG)
	unset(LIBPHYSX3_COOKING_DEBUG)
	unset(LIBPHYSX3_DEBUG)
	unset(LIBPHYSX3_EXTENSIONS_DEBUG)
	unset(LIBPHYSX3_VEHICLE_DEBUG)
	unset(LIBPHYSX_PROFILE_SDK_DEBUG)
	unset(LIBPHYSX_VISUAL_DEBUGGER_SDK_DEBUG)
	unset(LIBPVD_RUNTIME_DEBUG)
	unset(LIBPX_TASK_DEBUG)
	unset(LIBSCENE_QUERY_DEBUG)
	unset(LIBSIMULATION_CONTROLLER_DEBUG)


	mark_as_advanced(LIBLODEPNG)
	mark_as_advanced(LIBCORE)
	mark_as_advanced(LIBDATA)
	mark_as_advanced(LIBMATH)
	mark_as_advanced(LIBPERFORMANCE)
	mark_as_advanced(LIBSCRIPTING)
	mark_as_advanced(LIBCOMMUNICATION)
	mark_as_advanced(LIBPIPELINE)
	mark_as_advanced(LIBHUMAN)
	mark_as_advanced(LIBGLEW_158)
	mark_as_advanced(LIBGLFW)
	mark_as_advanced(LIBJANSSON)
	mark_as_advanced(LIBOGG)
	mark_as_advanced(LIBVORBIS)
	mark_as_advanced(LIBVORBISFILE)
	mark_as_advanced(LIBV8)
	mark_as_advanced(LIBV8_LIBBASE)
	mark_as_advanced(LIBV8_LIBPLATFORM)
	mark_as_advanced(LIBV8_NOSHNAPSHOT)
	mark_as_advanced(LIBV8_ICUDATA)
	mark_as_advanced(LIBV8_ICUUC)
	mark_as_advanced(LIBV8_ICUI18N)
	mark_as_advanced(LIBV8_NOSNAPSHOT)
	mark_as_advanced(LIBFMOD)
	mark_as_advanced(LIBRAKNET)
	mark_as_advanced(LIBLOW_LEVEL_DEBUG)
	mark_as_advanced(LIBLOW_LEVEL_CLOTH_DEBUG)
	mark_as_advanced(LIBPHYSX3_CHARACTER_KINEMATIC_DEBUG)
	mark_as_advanced(LIBPHYSX3_COMMON_DEBUG)
	mark_as_advanced(LIBPHYSX3_COOKING_DEBUG)
	mark_as_advanced(LIBPHYSX3_DEBUG)
	mark_as_advanced(LIBPHYSX3_EXTENSIONS_DEBUG)
	mark_as_advanced(LIBPHYSX3_VEHICLE_DEBUG)
	mark_as_advanced(LIBPHYSX_PROFILE_SDK_DEBUG)
	mark_as_advanced(LIBPHYSX_VISUAL_DEBUGGER_SDK_DEBUG)
	mark_as_advanced(LIBPVD_RUNTIME_DEBUG)
	mark_as_advanced(LIBPX_TASK_DEBUG)
	mark_as_advanced(LIBSCENE_QUERY_DEBUG)
	mark_as_advanced(LIBSIMULATION_CONTROLLER_DEBUG)

	if( ${OPIFEX_OS_ANDROID} )
		find_binary(LIBLODEPNG "LodePNG")
		find_binary(LIBCORE "opifex-core")
		find_binary(LIBDATA "opifex-data")
		find_binary(LIBMATH "Math")
		find_binary(LIBPERFORMANCE "Performance")
		find_binary(LIBSCRIPTING "Scripting")
		find_binary(LIBCOMMUNICATION "Communication" true)
		find_binary(LIBPIPELINE "Pipeline")
		find_binary(LIBHUMAN "Human")
	elseif("${OPIFEX_OS}" STREQUAL "OPIFEX_IOS")
		# message(STATUS "HEY WE'RE IOS, SHOULD BE GOOD")
		find_binary(LIBLODEPNG "LodePNG" false)
		find_binary(LIBCORE "Core" true)
		find_binary(LIBDATA "Data" true)
		find_binary(LIBMATH "Math" true)
		find_binary(LIBPERFORMANCE "Performance" true)
		find_binary(LIBSCRIPTING "Scripting" true)
		find_binary(LIBCOMMUNICATION "Communication" true)
		find_binary(LIBPIPELINE "Pipeline" true)
		find_binary(LIBHUMAN "Human" true)
	else()
		# message(STATUS "!!!!!!!!!!!! NOT IOS OR ANDROID")
		find_binary(LIBLODEPNG "LodePNG" false)
		find_binary(LIBCORE "Core" true)
		find_binary(LIBDATA "Data" true)
		find_binary(LIBMATH "Math" true)
		find_binary(LIBPERFORMANCE "Performance" true)
		find_binary(LIBSCRIPTING "Scripting" true)
		find_binary(LIBCOMMUNICATION "Communication" true)
		find_binary(LIBPIPELINE "Pipeline" true)
		find_binary(LIBHUMAN "Human" true)
		find_binary(LIBGLEW_158 "GLEW_158" false)
		find_binary(LIBGLFW "glfw3" false)
		if(${OPIFEX_OPTION_RELEASE})
				find_binary(LIBJANSSON "jansson" false)
		else()
            if(${OPIFEX_OS_WINDOWS})
				find_binary(LIBJANSSON "jansson_d" false)
            else()
				find_binary(LIBJANSSON "jansson" false)
            endif()
		endif()

		target_link_libraries(${APPLICATION_TARGET}
			${LIBGLFW}
			${LIBGLEW_158}
			${LIBJANSSON}
		)
	endif()

	target_link_libraries(${APPLICATION_TARGET}
		${OPENGL_LIBRARY}
		${LIBLODEPNG}
		${LIBCORE}
		${LIBDATA}
		${LIBMATH}
		${LIBPERFORMANCE}
		${LIBHUMAN}
		${LIBSCRIPTING}
		${LIBCOMMUNICATION}
		${LIBPIPELINE}
		${LIBGLFW}
		${LIBGLEW_158}
		${OPENAL_LIBRARY}
		${OpenGL}
	)

	add_opifex_xinput(${APPLICATION_TARGET})

	if(${OPIFEX_OPTION_FMOD})

		if(${OPIFEX_OS_WINDOWS})
			if(${OPIFEX_OS_WIN64})
				find_dynamic(LIBFMOD "fmod64_vc")
				copy_from_binaries_on_build(${APPLICATION_TARGET} "fmod64.dll" ${OPIFEX_OS_WINDOWS})
				copy_from_binaries_on_build(${APPLICATION_TARGET} "fmodL64.dll" ${OPIFEX_OS_WINDOWS})
			else()
				find_dynamic(LIBFMOD "fmod_vc")
				copy_from_binaries_on_build(${APPLICATION_TARGET} "fmod.dll" ${OPIFEX_OS_WINDOWS})
				copy_from_binaries_on_build(${APPLICATION_TARGET} "fmodL.dll" ${OPIFEX_OS_WINDOWS})
			endif()
		else()
			find_dynamic(LIBFMOD "fmod")
		endif()
		target_link_libraries(${APPLICATION_TARGET}
				${LIBFMOD}
			)
	endif()

	if(${OPIFEX_OPTION_RAKNET})

		if(${OPIFEX_OS_WINDOWS})
			if(${OPIFEX_OS_WIN64})
				if(${OPIFEX_OPTION_RELEASE})
					find_dynamic(LIBRAKNET "RakNet_VS2008_LibStatic_Release_x64")
				else()
					find_dynamic(LIBRAKNET "RakNet_VS2008_LibStatic_Debug_x64")
				endif()
			else()
				find_dynamic(LIBRAKNET "RakNet_VS2008_LibStatic_Debug_x86")
			endif()
		else()
			find_dynamic(LIBFMOD "fmod")
		endif()
		target_link_libraries(${APPLICATION_TARGET}
				${LIBRAKNET}
			)
	endif()

	if(${OPIFEX_OPTION_V8})

		if(${OPIFEX_OS_WINDOWS})

			find_binary(LIBV8_BASE_0 "v8_base_0" false)
			find_binary(LIBV8_BASE_1 "v8_base_1" false)
			find_binary(LIBV8_BASE_2 "v8_base_2" false)
			find_binary(LIBV8_BASE_3 "v8_base_3" false)
			find_binary(LIBV8_LIBBASE "v8_libbase" false)
			find_binary(LIBV8_LIBPLATFORM "v8_libplatform" false)
			find_binary(LIBV8_EXTERNAL_SNAPSHOT "v8_external_snapshot" false)
			find_binary(LIBV8_NOSNAPSHOT "v8_nosnapshot" false)
			find_binary(LIBV8_ICUI18N "icui18n" false)
			find_binary(LIBV8_ICUUC "icuuc" false)
			#find_binary(LIBV8_MKSNAPSHOT "mksnapshot" false)
			copy_from_binaries_on_build(${APPLICATION_TARGET} "icudt.dll" ${OPIFEX_OS_WINDOWS})
		else()
			find_binary(LIBV8 "v8_base" false)
			find_binary(LIBV8_LIBBASE "v8_libbase" false)
			find_binary(LIBV8_LIBPLATFORM "v8_libplatform" false)
			find_binary(LIBV8_NOSHNAPSHOT "v8_nosnapshot" false)
			find_binary(LIBV8_SAMPLER "v8_libsampler" false)
			find_binary(LIBV8_ICUUC "icuuc" false)
			find_binary(LIBV8_ICUI18N "icui18n" false)
			#find_binary(LIBV8_EXTERNAL_SNAPSHOT "v8_external_snapshot" false)

		endif()


		add_definitions(-DOPIFEX_V8)


		if( ${OPIFEX_OS_WINDOWS} )
			target_link_libraries(${APPLICATION_TARGET}
					ws2_32.lib
					advapi32.lib
					winmm.lib
					${LIBV8_BASE_0}
					${LIBV8_BASE_1}
					${LIBV8_BASE_2}
					${LIBV8_BASE_3}
					${LIBV8_LIBBASE}
					${LIBV8_LIBPLATFORM}
					#${LIBV8_EXTERNAL_SNAPSHOT}
					${LIBV8_ICUUC}
					${LIBV8_ICUI18N}
					#${LIBV8_MKSNAPSHOT}
					${LIBV8_NOSNAPSHOT}
					)
		else()
            message(STATUS "Linking V8")
            message(STATUS "${LIBV8}")
            message(STATUS "${LIBV8_LIBBASE}")
            message(STATUS "${LIBV8_LIBPLATFORM}")
            message(STATUS "${LIBV8_NOSHNAPSHOT}")
            message(STATUS "${LIBV8_SAMPLER}")
            message(STATUS "${LIBV8_ICUUC}")
            message(STATUS "${LIBV8_ICUI18N}")
            message(STATUS "${LIBV8_EXTERNAL_SNAPSHOT}")
			target_link_libraries(${APPLICATION_TARGET}
					${LIBV8}
					${LIBV8_LIBBASE}
					${LIBV8_LIBPLATFORM}
					${LIBV8_NOSHNAPSHOT}
                    ${LIBV8_SAMPLER}
					${LIBV8_ICUUC}
					${LIBV8_ICUI18N}
                    #${LIBV8_EXTERNAL_SNAPSHOT}
                )
		endif()
	endif()

	if(${OPIFEX_OPTION_PHYSX})
			if(${OPIFEX_OS_WINDOWS})
				if(${OPIFEX_OS_WIN64})
						if(${OPIFEX_OPTION_RELEASE})

								# message(STATUS "USING RELEASE VERSION OF PHYSX")

								find_binary(LIBLOW_LEVEL_DEBUG "LowLevel" false)
								find_binary(LIBLOW_LEVEL_CLOTH_DEBUG "LowLevelCloth" false)
								find_binary(LIBPHYSX3_CHARACTER_KINEMATIC_DEBUG "PhysX3CharacterKinematic_x64" false)
								find_binary(LIBPHYSX3_COMMON_DEBUG "PhysX3Common_x64" false)
								find_binary(LIBPHYSX3_COOKING_DEBUG "PhysX3Cooking_x64" false)
								find_binary(LIBPHYSX3_DEBUG "PhysX3_x64" false)
								find_binary(LIBPHYSX3_EXTENSIONS_DEBUG "PhysX3Extensions" false)
								find_binary(LIBPHYSX3_VEHICLE_DEBUG "PhysX3Vehicle" false)
								find_binary(LIBPHYSX_PROFILE_SDK_DEBUG "PhysXProfileSDK" false)
								find_binary(LIBPHYSX_VISUAL_DEBUGGER_SDK_DEBUG "PhysXVisualDebuggerSDK" false)
								find_binary(LIBPVD_RUNTIME_DEBUG "PvdRuntime" false)
								find_binary(LIBPX_TASK_DEBUG "PxTask" false)
								find_binary(LIBSCENE_QUERY_DEBUG "SceneQuery" false)
								find_binary(LIBSIMULATION_CONTROLLER_DEBUG "SimulationController" false)

								copy_from_binaries_on_build(${APPLICATION_TARGET} "PhysX3CharacterKinematic_x64.dll" ${OPIFEX_OS_WINDOWS})
								copy_from_binaries_on_build(${APPLICATION_TARGET} "PhysX3Common_x64.dll" ${OPIFEX_OS_WINDOWS})
								copy_from_binaries_on_build(${APPLICATION_TARGET} "PhysX3Cooking_x64.dll" ${OPIFEX_OS_WINDOWS})
								copy_from_binaries_on_build(${APPLICATION_TARGET} "PhysX3_x64.dll" ${OPIFEX_OS_WINDOWS})
								#copy_from_binaries_on_build(${APPLICATION_TARGET} "PhysX3Gpu_x64.dll" ${OPIFEX_OS_WINDOWS})
								#copy_from_binaries_on_build(${APPLICATION_TARGET} "PhysXDevice64.dll" ${OPIFEX_OS_WINDOWS})
								copy_from_binaries_on_build(${APPLICATION_TARGET} "nvToolsExt64_1.dll" ${OPIFEX_OS_WINDOWS})
						else()
								find_binary(LIBLOW_LEVEL_DEBUG "LowLevelDEBUG" false)
								find_binary(LIBLOW_LEVEL_CLOTH_DEBUG "LowLevelClothDEBUG" false)
								find_binary(LIBPHYSX3_CHARACTER_KINEMATIC_DEBUG "PhysX3CharacterKinematicDEBUG_x64" false)
								find_binary(LIBPHYSX3_COMMON_DEBUG "PhysX3CommonDEBUG_x64" false)
								find_binary(LIBPHYSX3_COOKING_DEBUG "PhysX3CookingDEBUG_x64" false)
								find_binary(LIBPHYSX3_DEBUG "PhysX3DEBUG_x64" false)
								find_binary(LIBPHYSX3_EXTENSIONS_DEBUG "PhysX3ExtensionsDEBUG" false)
								find_binary(LIBPHYSX3_VEHICLE_DEBUG "PhysX3VehicleDEBUG" false)
								find_binary(LIBPHYSX_PROFILE_SDK_DEBUG "PhysXProfileSDKDEBUG" false)
								find_binary(LIBPHYSX_VISUAL_DEBUGGER_SDK_DEBUG "PhysXVisualDebuggerSDKDEBUG" false)
								find_binary(LIBPVD_RUNTIME_DEBUG "PvdRuntimeDEBUG" false)
								find_binary(LIBPX_TASK_DEBUG "PxTaskDEBUG" false)
								find_binary(LIBSCENE_QUERY_DEBUG "SceneQueryDEBUG" false)
								find_binary(LIBSIMULATION_CONTROLLER_DEBUG "SimulationControllerDEBUG" false)

								copy_from_binaries_on_build(${APPLICATION_TARGET} "PhysX3CharacterKinematicDEBUG_x64.dll" ${OPIFEX_OS_WINDOWS})
								copy_from_binaries_on_build(${APPLICATION_TARGET} "PhysX3CommonDEBUG_x64.dll" ${OPIFEX_OS_WINDOWS})
								copy_from_binaries_on_build(${APPLICATION_TARGET} "PhysX3CookingDEBUG_x64.dll" ${OPIFEX_OS_WINDOWS})
								copy_from_binaries_on_build(${APPLICATION_TARGET} "PhysX3DEBUG_x64.dll" ${OPIFEX_OS_WINDOWS})
								#copy_from_binaries_on_build(${APPLICATION_TARGET} "PhysX3GpuDEBUG_x64.dll" ${OPIFEX_OS_WINDOWS})
								#copy_from_binaries_on_build(${APPLICATION_TARGET} "PhysXDevice64.dll" ${OPIFEX_OS_WINDOWS})
								copy_from_binaries_on_build(${APPLICATION_TARGET} "nvToolsExt64_1.dll" ${OPIFEX_OS_WINDOWS})
						endif()

				else()

					if(${OPIFEX_OPTION_RELEASE})
						find_binary(LIBLOW_LEVEL_DEBUG "LowLevel" false)
						find_binary(LIBLOW_LEVEL_CLOTH_DEBUG "LowLevelCloth" false)
						find_binary(LIBPHYSX3_CHARACTER_KINEMATIC_DEBUG "PhysX3CharacterKinematic_x86" false)
						find_binary(LIBPHYSX3_COMMON_DEBUG "PhysX3Common_x86" false)
						find_binary(LIBPHYSX3_COOKING_DEBUG "PhysX3Cooking_x86" false)
						find_binary(LIBPHYSX3_DEBUG "PhysX3_x86" false)
						find_binary(LIBPHYSX3_EXTENSIONS_DEBUG "PhysX3Extensions" false)
						find_binary(LIBPHYSX3_VEHICLE_DEBUG "PhysX3Vehicle" false)
						find_binary(LIBPHYSX_PROFILE_SDK_DEBUG "PhysXProfileSDK" false)
						find_binary(LIBPHYSX_VISUAL_DEBUGGER_SDK_DEBUG "PhysXVisualDebuggerSDK" false)
						find_binary(LIBPVD_RUNTIME_DEBUG "PvdRuntime" false)
						find_binary(LIBPX_TASK_DEBUG "PxTask" false)
						find_binary(LIBSCENE_QUERY_DEBUG "SceneQuery" false)
						find_binary(LIBSIMULATION_CONTROLLER_DEBUG "SimulationController" false)

						copy_from_binaries_on_build(${APPLICATION_TARGET} "PhysX3CharacterKinematic_x86.dll" ${OPIFEX_OS_WINDOWS})
						copy_from_binaries_on_build(${APPLICATION_TARGET} "PhysX3Common_x86.dll" ${OPIFEX_OS_WINDOWS})
						copy_from_binaries_on_build(${APPLICATION_TARGET} "PhysX3Cooking_x86.dll" ${OPIFEX_OS_WINDOWS})
						copy_from_binaries_on_build(${APPLICATION_TARGET} "PhysX3_x86.dll" ${OPIFEX_OS_WINDOWS})
						copy_from_binaries_on_build(${APPLICATION_TARGET} "PhysX3Gpu_x86.dll" ${OPIFEX_OS_WINDOWS})
						copy_from_binaries_on_build(${APPLICATION_TARGET} "PhysXDevice.dll" ${OPIFEX_OS_WINDOWS})
						copy_from_binaries_on_build(${APPLICATION_TARGET} "nvToolsExt32_1.dll" ${OPIFEX_OS_WINDOWS})
					else()
						find_binary(LIBLOW_LEVEL_DEBUG "LowLevelDEBUG" false)
						find_binary(LIBLOW_LEVEL_CLOTH_DEBUG "LowLevelClothDEBUG" false)
						find_binary(LIBPHYSX3_CHARACTER_KINEMATIC_DEBUG "PhysX3CharacterKinematicDEBUG_x86" false)
						find_binary(LIBPHYSX3_COMMON_DEBUG "PhysX3CommonDEBUG_x86" false)
						find_binary(LIBPHYSX3_COOKING_DEBUG "PhysX3CookingDEBUG_x86" false)
						find_binary(LIBPHYSX3_DEBUG "PhysX3DEBUG_x86" false)
						find_binary(LIBPHYSX3_EXTENSIONS_DEBUG "PhysX3ExtensionsDEBUG" false)
						find_binary(LIBPHYSX3_VEHICLE_DEBUG "PhysX3VehicleDEBUG" false)
						find_binary(LIBPHYSX_PROFILE_SDK_DEBUG "PhysXProfileSDKDEBUG" false)
						find_binary(LIBPHYSX_VISUAL_DEBUGGER_SDK_DEBUG "PhysXVisualDebuggerSDKDEBUG" false)
						find_binary(LIBPVD_RUNTIME_DEBUG "PvdRuntimeDEBUG" false)
						find_binary(LIBPX_TASK_DEBUG "PxTaskDEBUG" false)
						find_binary(LIBSCENE_QUERY_DEBUG "SceneQueryDEBUG" false)
						find_binary(LIBSIMULATION_CONTROLLER_DEBUG "SimulationControllerDEBUG" false)

						copy_from_binaries_on_build(${APPLICATION_TARGET} "PhysX3CharacterKinematicDEBUG_x86.dll" ${OPIFEX_OS_WINDOWS})
						copy_from_binaries_on_build(${APPLICATION_TARGET} "PhysX3CommonDEBUG_x86.dll" ${OPIFEX_OS_WINDOWS})
						copy_from_binaries_on_build(${APPLICATION_TARGET} "PhysX3CookingDEBUG_x86.dll" ${OPIFEX_OS_WINDOWS})
						copy_from_binaries_on_build(${APPLICATION_TARGET} "PhysX3DEBUG_x86.dll" ${OPIFEX_OS_WINDOWS})
						copy_from_binaries_on_build(${APPLICATION_TARGET} "PhysX3GpuDEBUG_x86.dll" ${OPIFEX_OS_WINDOWS})
						copy_from_binaries_on_build(${APPLICATION_TARGET} "PhysXDevice.dll" ${OPIFEX_OS_WINDOWS})
						copy_from_binaries_on_build(${APPLICATION_TARGET} "nvToolsExt32_1.dll" ${OPIFEX_OS_WINDOWS})
					endif()

				endif()
			else()
				find_binary(LIBLOW_LEVEL_DEBUG "LowLevelDEBUG" false)
				find_binary(LIBLOW_LEVEL_CLOTH_DEBUG "LowLevelClothDEBUG" false)
				find_binary(LIBPHYSX3_CHARACTER_KINEMATIC_DEBUG "PhysX3CharacterKinematicDEBUG" false)
				find_binary(LIBPHYSX3_COMMON_DEBUG "PhysX3CommonDEBUG" false)
				find_binary(LIBPHYSX3_COOKING_DEBUG "PhysX3CookingDEBUG" false)
				find_binary(LIBPHYSX3_DEBUG "PhysX3DEBUG" false)
				find_binary(LIBPHYSX3_EXTENSIONS_DEBUG "PhysX3ExtensionsDEBUG" false)
				find_binary(LIBPHYSX3_VEHICLE_DEBUG "PhysX3VehicleDEBUG" false)
				find_binary(LIBPHYSX_PROFILE_SDK_DEBUG "PhysXProfileSDKDEBUG" false)
				find_binary(LIBPHYSX_VISUAL_DEBUGGER_SDK_DEBUG "PhysXVisualDebuggerSDKDEBUG" false)
				find_binary(LIBPVD_RUNTIME_DEBUG "PvdRuntimeDEBUG" false)
				find_binary(LIBPX_TASK_DEBUG "PxTaskDEBUG" false)
				find_binary(LIBSCENE_QUERY_DEBUG "SceneQueryDEBUG" false)
				find_binary(LIBSIMULATION_CONTROLLER_DEBUG "SimulationControllerDEBUG" false)
			endif()

			target_link_libraries(${APPLICATION_TARGET}
					${LIBLOW_LEVEL_DEBUG}
					${LIBLOW_LEVEL_CLOTH_DEBUG}
					${LIBPHYSX3_CHARACTER_KINEMATIC_DEBUG}
					${LIBPHYSX3_COMMON_DEBUG}
					${LIBPHYSX3_COOKING_DEBUG}
					${LIBPHYSX3_DEBUG}
					${LIBPHYSX3_EXTENSIONS_DEBUG}
					${LIBPHYSX3_VEHICLE_DEBUG}
					${LIBPHYSX_PROFILE_SDK_DEBUG}
					${LIBPHYSX_VISUAL_DEBUGGER_SDK_DEBUG}
					${LIBPVD_RUNTIME_DEBUG}
					${LIBPX_TASK_DEBUG}
					${LIBSCENE_QUERY_DEBUG}
					${LIBSIMULATION_CONTROLLER_DEBUG}
				)
	endif()

	if( ${OPIFEX_OS_WINDOWS} )
		target_link_libraries(${APPLICATION_TARGET}	Winmm.lib)
		if( ${OPIFEX_OPTION_AUDIO} )
			if(${OPIFEX_OPTION_RELEASE})
				find_binary(LIBOGG "libogg_static" false)
				find_binary(LIBVORBIS "libvorbis_static" false)
				find_binary(LIBVORBISFILE "libvorbisfile_static" false)
			else()
				find_binary(LIBOGG "libogg" false)
				find_binary(LIBVORBIS "libvorbis" false)
				find_binary(LIBVORBISFILE "libvorbisfile" false)
			endif()
			target_link_libraries(${APPLICATION_TARGET}
				Winmm.lib
				${LIBOGG}
				${LIBVORBIS}
				${LIBVORBISFILE}
			)
		endif()
	endif()

	if( ${MSVC_WIN7_FIX} )
		target_link_libraries(${APPLICATION_TARGET}	Xinput9_1_0.lib)
	endif()

	if(${OPIFEX_OS_LINUX32} OR ${OPIFEX_OS_LINUX64})
		find_package( X11 REQUIRED )
		find_package( Threads )


		set(GL_LIBRARY GL GLU X11 Xi Xxf86vm Xrandr)

		target_link_libraries(${APPLICATION_TARGET}
			${X11_LIBRARIES}
			${CMAKE_THREAD_LIBS_INIT}
			${CMAKE_DL_LIBS}
			${GL_LIBRARY}
			${GLFW_LIBRARIES}
		)

		if( ${OPIFEX_OPTION_AUDIO} )
			target_link_libraries(${APPLICATION_TARGET}
				${OGG_LIBRARY}
				${VORBIS_LIBRARY}
				${VORBISFILE_LIBRARY}
				-logg
			)
		endif()
		SET(CMAKE_EXE_LINKER_FLAGS, "-ldl")
	endif()

	if(${OPIFEX_OS_OSX32} OR ${OPIFEX_OS_OSX64})
		SET(GLFW_LIBRARIES "/System/Library/Frameworks/Cocoa.framework;/System/Library/Frameworks/OpenGL.framework;/System/Library/Frameworks/IOKit.framework;/System/Library/Frameworks/CoreFoundation.framework;/System/Library/Frameworks/CoreVideo.framework")
		SET(OPENAL_LIBRARY "/System/Library/Frameworks/OpenAL.framework")
		target_link_libraries(${APPLICATION_TARGET}
			${CMAKE_THREAD_LIBS_INIT}
			${GLFW_LIBRARIES}
		)
		if( ${OPIFEX_OPTION_AUDIO} )
			target_link_libraries(${APPLICATION_TARGET}
				${OPENAL_LIBRARY}
				${OGG_LIBRARY}
				${VORBIS_LIBRARY}
				${VORBISFILE_LIBRARY}
			)
		endif()
	endif()


	if( ${OPIFEX_OPTION_AUDIO} )
		copy_from_binaries_on_build(${APPLICATION_TARGET} "ogg.dll" ${OPIFEX_OS_WINDOWS})
		copy_from_binaries_on_build(${APPLICATION_TARGET} "libogg.dll" ${OPIFEX_OS_WINDOWS})
		copy_from_binaries_on_build(${APPLICATION_TARGET} "vorbisfile.dll" ${OPIFEX_OS_WINDOWS})
		copy_from_binaries_on_build(${APPLICATION_TARGET} "libvorbis.dll" ${OPIFEX_OS_WINDOWS})
		copy_from_binaries_on_build(${APPLICATION_TARGET} "libvorbisfile.dll" ${OPIFEX_OS_WINDOWS})
	endif()
	#copy_from_binaries_on_build(${APPLICATION_TARGET} "myo32.dll" ${OPIFEX_OS_WINDOWS})
	#copy_from_binaries_on_build(${APPLICATION_TARGET} "ble32.dll" ${OPIFEX_OS_WINDOWS})

endmacro(add_opifex_libraries)
