macro(add_opifex_assets APPLICATION_TARGET )

	unset(OPIFEX_ASSETS)

	if(${OPIFEX_OPTION_RELEASE})
		SET(OPIFEX_ASSETS NULL)
		add_definitions(-DOPIFEX_ASSETS=NULL)
	else()

		add_definitions(-D_DEBUG)
		SET(OPIFEX_ASSETS "${PROJECT_SOURCE_DIR}/Assets/")
		add_definitions(-DOPIFEX_ASSETS="${OPIFEX_ASSETS}")

		message(STATUS "OPIFEX ASSETS SET TO: ${OPIFEX_ASSETS}")

	endif()

endmacro(add_opifex_assets)

macro(add_opifex_physx APPLICATION_TARGET )

	if(${OPIFEX_OPTION_PHYSX})

		message(STATUS "Link Physics")
		include_directories(../External/PhysX/include/)

        if(${OPIFEX_OS_WINDOWS})
			if(${OPIFEX_OS_WIN64})
				if(${OPIFEX_OPTION_RELEASE})
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win64/PhysX3_x64.lib)
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win64/PhysXProfileSDK.lib)
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win64/LowLevel.lib)
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win64/LowLevelCloth.lib)
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win64/PhysX3CharacterKinematic_x64.lib)
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win64/PhysX3Common_x64.lib)
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win64/PhysX3Cooking_x64.lib)
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win64/PhysX3Extensions.lib)
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win64/PhysX3Vehicle.lib)
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win64/PhysXVisualDebuggerSDK.lib)
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win64/PxTask.lib)
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win64/PvdRuntime.lib)
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win64/SceneQuery.lib)
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win64/SimulationController.lib)

					copy_file_to_binaries(/External/PhysX/lib/win64/PhysX3_x64.lib)
					copy_file_to_binaries(/External/PhysX/lib/win64/PhysXProfileSDK.lib)
					copy_file_to_binaries(/External/PhysX/lib/win64/LowLevel.lib)
					copy_file_to_binaries(/External/PhysX/lib/win64/LowLevelCloth.lib)
					copy_file_to_binaries(/External/PhysX/lib/win64/PhysX3CharacterKinematic_x64.lib)
					copy_file_to_binaries(/External/PhysX/lib/win64/PhysX3Common_x64.lib)
					copy_file_to_binaries(/External/PhysX/lib/win64/PhysX3Cooking_x64.lib)
					copy_file_to_binaries(/External/PhysX/lib/win64/PhysX3Extensions.lib)
					copy_file_to_binaries(/External/PhysX/lib/win64/PhysX3Vehicle.lib)
					copy_file_to_binaries(/External/PhysX/lib/win64/PhysXVisualDebuggerSDK.lib)
					copy_file_to_binaries(/External/PhysX/lib/win64/PxTask.lib)
					copy_file_to_binaries(/External/PhysX/lib/win64/PvdRuntime.lib)
					copy_file_to_binaries(/External/PhysX/lib/win64/SceneQuery.lib)
					copy_file_to_binaries(/External/PhysX/lib/win64/SimulationController.lib)

					copy_to_folder(${APPLICATION_TARGET} "/External/PhysX/lib/" "PhysX3CharacterKinematic_x64.dll" "/Application/Release" ${OPIFEX_OS_WINDOWS})
					copy_to_folder(${APPLICATION_TARGET} "/External/PhysX/lib/" "PhysX3Common_x64.dll" "/Application/Release" ${OPIFEX_OS_WINDOWS})
					copy_to_folder(${APPLICATION_TARGET} "/External/PhysX/lib/" "PhysX3Cooking_x64.dll" "/Application/Release" ${OPIFEX_OS_WINDOWS})
					copy_to_folder(${APPLICATION_TARGET} "/External/PhysX/lib/" "PhysX3_x64.dll" "/Application/Release" ${OPIFEX_OS_WINDOWS})
					copy_to_folder(${APPLICATION_TARGET} "/External/PhysX/lib/" "PhysX3Gpu_x64.dll" "/Application/Release" ${OPIFEX_OS_WINDOWS})
					copy_to_folder(${APPLICATION_TARGET} "/External/PhysX/lib/" "PhysXDevice64.dll" "/Application/Release" ${OPIFEX_OS_WINDOWS})
					copy_to_folder(${APPLICATION_TARGET} "/External/PhysX/lib/" "nvToolsExt64_1.dll" "/Application/Release" ${OPIFEX_OS_WINDOWS})
				else()

					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win64/PhysX3DEBUG_x64.lib)
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win64/PhysXProfileSDKDEBUG.lib)
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win64/LowLevelDEBUG.lib)
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win64/LowLevelClothDEBUG.lib)
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win64/PhysX3CharacterKinematicDEBUG_x64.lib)
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win64/PhysX3CommonDEBUG_x64.lib)
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win64/PhysX3CookingDEBUG_x64.lib)
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win64/PhysX3ExtensionsDEBUG.lib)
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win64/PhysX3VehicleDEBUG.lib)
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win64/PhysXVisualDebuggerSDKDEBUG.lib)
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win64/PxTaskDEBUG.lib)
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win64/PvdRuntimeDEBUG.lib)
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win64/SceneQueryDEBUG.lib)
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win64/SimulationControllerDEBUG.lib)

					copy_file_to_binaries(/External/PhysX/lib/win64/PhysX3DEBUG_x64.lib)
					copy_file_to_binaries(/External/PhysX/lib/win64/PhysXProfileSDKDEBUG.lib)
					copy_file_to_binaries(/External/PhysX/lib/win64/LowLevelDEBUG.lib)
					copy_file_to_binaries(/External/PhysX/lib/win64/LowLevelClothDEBUG.lib)
					copy_file_to_binaries(/External/PhysX/lib/win64/PhysX3CharacterKinematicDEBUG_x64.lib)
					copy_file_to_binaries(/External/PhysX/lib/win64/PhysX3CommonDEBUG_x64.lib)
					copy_file_to_binaries(/External/PhysX/lib/win64/PhysX3CookingDEBUG_x64.lib)
					copy_file_to_binaries(/External/PhysX/lib/win64/PhysX3ExtensionsDEBUG.lib)
					copy_file_to_binaries(/External/PhysX/lib/win64/PhysX3VehicleDEBUG.lib)
					copy_file_to_binaries(/External/PhysX/lib/win64/PhysXVisualDebuggerSDKDEBUG.lib)
					copy_file_to_binaries(/External/PhysX/lib/win64/PxTaskDEBUG.lib)
					copy_file_to_binaries(/External/PhysX/lib/win64/PvdRuntimeDEBUG.lib)
					copy_file_to_binaries(/External/PhysX/lib/win64/SceneQueryDEBUG.lib)
					copy_file_to_binaries(/External/PhysX/lib/win64/SimulationControllerDEBUG.lib)

					copy_to_folder(${APPLICATION_TARGET} "/External/PhysX/lib/" "PhysX3CharacterKinematicDEBUG_x64.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
					copy_to_folder(${APPLICATION_TARGET} "/External/PhysX/lib/" "PhysX3CommonDEBUG_x64.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
					copy_to_folder(${APPLICATION_TARGET} "/External/PhysX/lib/" "PhysX3CookingDEBUG_x64.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
					copy_to_folder(${APPLICATION_TARGET} "/External/PhysX/lib/" "PhysX3DEBUG_x64.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
					copy_to_folder(${APPLICATION_TARGET} "/External/PhysX/lib/" "PhysX3GpuDEBUG_x64.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
					copy_to_folder(${APPLICATION_TARGET} "/External/PhysX/lib/" "PhysXDevice64.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
					copy_to_folder(${APPLICATION_TARGET} "/External/PhysX/lib/" "nvToolsExt64_1.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
				endif()
			else()
				target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win32/PhysX3DEBUG_x86.lib)
				target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win32/PhysXProfileSDKDEBUG.lib)
				target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win32/LowLevelDEBUG.lib)
				target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win32/LowLevelClothDEBUG.lib)
				target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win32/PhysX3CharacterKinematicDEBUG_x86.lib)
				target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win32/PhysX3CommonDEBUG_x86.lib)
				target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win32/PhysX3CookingDEBUG_x86.lib)
				target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win32/PhysX3ExtensionsDEBUG.lib)
				target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win32/PhysX3VehicleDEBUG.lib)
				target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win32/PhysXVisualDebuggerSDKDEBUG.lib)
				target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win32/PxTaskDEBUG.lib)
				target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win32/PvdRuntimeDEBUG.lib)
				target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win32/SceneQueryDEBUG.lib)
				target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win32/SimulationControllerDEBUG.lib)

				copy_file_to_binaries(/External/PhysX/lib/win32/PhysX3DEBUG_x86.lib)
				copy_file_to_binaries(/External/PhysX/lib/win32/PhysXProfileSDKDEBUG.lib)
				copy_file_to_binaries(/External/PhysX/lib/win32/LowLevelDEBUG.lib)
				copy_file_to_binaries(/External/PhysX/lib/win32/LowLevelClothDEBUG.lib)
				copy_file_to_binaries(/External/PhysX/lib/win32/PhysX3CharacterKinematicDEBUG_x86.lib)
				copy_file_to_binaries(/External/PhysX/lib/win32/PhysX3CommonDEBUG_x86.lib)
				copy_file_to_binaries(/External/PhysX/lib/win32/PhysX3CookingDEBUG_x86.lib)
				copy_file_to_binaries(/External/PhysX/lib/win32/PhysX3ExtensionsDEBUG.lib)
				copy_file_to_binaries(/External/PhysX/lib/win32/PhysX3VehicleDEBUG.lib)
				copy_file_to_binaries(/External/PhysX/lib/win32/PhysXVisualDebuggerSDKDEBUG.lib)
				copy_file_to_binaries(/External/PhysX/lib/win32/PxTaskDEBUG.lib)
				copy_file_to_binaries(/External/PhysX/lib/win32/PvdRuntimeDEBUG.lib)
				copy_file_to_binaries(/External/PhysX/lib/win32/SceneQueryDEBUG.lib)
				copy_file_to_binaries(/External/PhysX/lib/win32/SimulationControllerDEBUG.lib)

				copy_to_folder(${APPLICATION_TARGET} "/External/PhysX/lib/" "PhysX3CharacterKinematicDEBUG_x86.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
				copy_to_folder(${APPLICATION_TARGET} "/External/PhysX/lib/" "PhysX3CommonDEBUG_x86.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
				copy_to_folder(${APPLICATION_TARGET} "/External/PhysX/lib/" "PhysX3CookingDEBUG_x86.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
				copy_to_folder(${APPLICATION_TARGET} "/External/PhysX/lib/" "PhysX3DEBUG_x86.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
				copy_to_folder(${APPLICATION_TARGET} "/External/PhysX/lib/" "PhysX3GpuDEBUG_x86.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
				copy_to_folder(${APPLICATION_TARGET} "/External/PhysX/lib/" "PhysXDevice32.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
				copy_to_folder(${APPLICATION_TARGET} "/External/PhysX/lib/" "nvToolsExt32_1.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
            endif()
        elseif(${OPIFEX_OS_OSX64})
            target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/osx64/libPhysX3DEBUG.a)
            target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/osx64/libPhysXProfileSDKDEBUG.a)
            target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/osx64/libLowLevelDEBUG.a)
            target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/osx64/libLowLevelClothDEBUG.a)
            target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/osx64/libPhysX3CharacterKinematicDEBUG.a)
            target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/osx64/libPhysX3CommonDEBUG.a)
            target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/osx64/libPhysX3CookingDEBUG.a)
            target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/osx64/libPhysX3ExtensionsDEBUG.a)
            target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/osx64/libPhysX3VehicleDEBUG.a)
            target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/osx64/libPhysXVisualDebuggerSDKDEBUG.a)
            target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/osx64/libPxTaskDEBUG.a)
            target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/osx64/libPvdRuntimeDEBUG.a)
            target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/osx64/libSceneQueryDEBUG.a)
            target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/osx64/libSimulationControllerDEBUG.a)

            copy_file_to_binaries(/External/PhysX/lib/osx64/libPhysX3DEBUG.a)
            copy_file_to_binaries(/External/PhysX/lib/osx64/libPhysXProfileSDKDEBUG.a)
            copy_file_to_binaries(/External/PhysX/lib/osx64/libLowLevelDEBUG.a)
            copy_file_to_binaries(/External/PhysX/lib/osx64/libLowLevelClothDEBUG.a)
            copy_file_to_binaries(/External/PhysX/lib/osx64/libPhysX3CharacterKinematicDEBUG.a)
            copy_file_to_binaries(/External/PhysX/lib/osx64/libPhysX3CommonDEBUG.a)
            copy_file_to_binaries(/External/PhysX/lib/osx64/libPhysX3CookingDEBUG.a)
            copy_file_to_binaries(/External/PhysX/lib/osx64/libPhysX3ExtensionsDEBUG.a)
            copy_file_to_binaries(/External/PhysX/lib/osx64/libPhysX3VehicleDEBUG.a)
            copy_file_to_binaries(/External/PhysX/lib/osx64/libPhysXVisualDebuggerSDKDEBUG.a)
            copy_file_to_binaries(/External/PhysX/lib/osx64/libPxTaskDEBUG.a)
            copy_file_to_binaries(/External/PhysX/lib/osx64/libPvdRuntimeDEBUG.a)
            copy_file_to_binaries(/External/PhysX/lib/osx64/libSceneQueryDEBUG.a)
            copy_file_to_binaries(/External/PhysX/lib/osx64/libSimulationControllerDEBUG.a)

        elseif(${OPIFEX_OS_OSX32})
            target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/osx32/libPhysX3DEBUG.a)
            target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/osx32/libPhysXProfileSDKDEBUG.a)
            target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/osx32/libLowLevelDEBUG.a)
            target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/osx32/libLowLevelClothDEBUG.a)
            target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/osx32/libPhysX3CharacterKinematicDEBUG.a)
            target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/osx32/libPhysX3CommonDEBUG.a)
            target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/osx32/libPhysX3CookingDEBUG.a)
            target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/osx32/libPhysX3ExtensionsDEBUG.a)
            target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/osx32/libPhysX3VehicleDEBUG.a)
            target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/osx32/libPhysXVisualDebuggerSDKDEBUG.a)
            target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/osx32/libPxTaskDEBUG.a)
            target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/osx32/libPvdRuntimeDEBUG.a)
            target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/osx32/libSceneQueryDEBUG.a)
            target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/PhysX/lib/osx32/libSimulationControllerDEBUG.a)

            copy_file_to_binaries(/External/PhysX/lib/osx32/libPhysX3DEBUG.a)
            copy_file_to_binaries(/External/PhysX/lib/osx32/libPhysXProfileSDKDEBUG.a)
            copy_file_to_binaries(/External/PhysX/lib/osx32/libLowLevelDEBUG.a)
            copy_file_to_binaries(/External/PhysX/lib/osx32/libLowLevelClothDEBUG.a)
            copy_file_to_binaries(/External/PhysX/lib/osx32/libPhysX3CharacterKinematicDEBUG.a)
            copy_file_to_binaries(/External/PhysX/lib/osx32/libPhysX3CommonDEBUG.a)
            copy_file_to_binaries(/External/PhysX/lib/osx32/libPhysX3CookingDEBUG.a)
            copy_file_to_binaries(/External/PhysX/lib/osx32/libPhysX3ExtensionsDEBUG.a)
            copy_file_to_binaries(/External/PhysX/lib/osx32/libPhysX3VehicleDEBUG.a)
            copy_file_to_binaries(/External/PhysX/lib/osx32/libPhysXVisualDebuggerSDKDEBUG.a)
            copy_file_to_binaries(/External/PhysX/lib/osx32/libPxTaskDEBUG.a)
            copy_file_to_binaries(/External/PhysX/lib/osx32/libPvdRuntimeDEBUG.a)
            copy_file_to_binaries(/External/PhysX/lib/osx32/libSceneQueryDEBUG.a)
            copy_file_to_binaries(/External/PhysX/lib/osx32/libSimulationControllerDEBUG.a)
		endif()
	endif()

endmacro(add_opifex_physx)


macro(add_opifex_v8 APPLICATION_TARGET )

	if(${OPIFEX_OPTION_V8})

		if(${OPIFEX_OS_WINDOWS})
			target_link_libraries(${APPLICATION_TARGET}
				ws2_32.lib
				advapi32.lib
				winmm.lib)
			if(${OPIFEX_OPTION_RELEASE})
				if(${OPIFEX_OS_64})
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/V8/lib/release/win64/v8.lib)
				else()
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/V8/lib/release/win32/v8.lib)
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/V8/lib/release/win32/v8_libbase.lib)
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/V8/lib/release/win32/v8_libplatform.lib)
				endif()

			else()

				if(${OPIFEX_OS_64})
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/V8/lib/debug/win64/v8.lib)
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/V8/lib/debug/win64/v8_libbase.lib)
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/V8/lib/debug/win64/v8_libplatform.lib)
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/V8/lib/debug/win64/icuuc.lib)
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/V8/lib/debug/win64/icui18n.lib)
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/V8/lib/debug/win64/v8_base_0.lib)
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/V8/lib/debug/win64/v8_base_1.lib)
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/V8/lib/debug/win64/v8_base_2.lib)
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/V8/lib/debug/win64/v8_base_3.lib)
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/V8/lib/debug/win64/v8_nosnapshot.lib)
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/V8/lib/debug/win64/v8_external_snapshot.lib)
				else()
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/V8/lib/debug/win32/v8.lib)
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/V8/lib/debug/win32/v8_libbase.lib)
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/V8/lib/debug/win32/v8_libplatform.lib)
				endif()

			endif()


			if(${OPIFEX_OPTION_RELEASE})
				copy_to_folder(${APPLICATION_TARGET} "/External/V8/lib/release/" "icui18n.dll" "/Application/Release" ${OPIFEX_OS_WINDOWS})
				copy_to_folder(${APPLICATION_TARGET} "/External/V8/lib/release/" "icuuc.dll" "/Application/Release" ${OPIFEX_OS_WINDOWS})
				copy_to_folder(${APPLICATION_TARGET} "/External/V8/lib/release/" "v8.dll" "/Application/Release" ${OPIFEX_OS_WINDOWS})
			else()
				#copy_to_folder(${APPLICATION_TARGET} "/External/V8/lib/debug/" "icui18n.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
				#copy_to_folder(${APPLICATION_TARGET} "/External/V8/lib/debug/" "icuuc.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
				#copy_to_folder(${APPLICATION_TARGET} "/External/V8/lib/debug/" "v8.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
			endif()
		else()

			SET(CMAKE_C_FLAGS "-stdlib=libstdc++"	)
			SET(CMAKE_CXX_FLAGS "-stdlib=libstdc++"	)

			if(${OPIFEX_OPTION_RELEASE})

				if(${OPIFEX_OS_64})
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/V8/lib/release/osx64/v8.lib)
				else()
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/V8/lib/release/win32/v8.lib)
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/V8/lib/release/win32/v8_libbase.lib)
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/V8/lib/release/win32/v8_libplatform.lib)
				endif()

			else()

				copy_file_to_binaries(/External/V8/lib/debug/osx64/libv8_base.a)
				copy_file_to_binaries(/External/V8/lib/debug/osx64/libv8_libbase.a)
				copy_file_to_binaries(/External/V8/lib/debug/osx64/libv8_libplatform.a)
				copy_file_to_binaries(/External/V8/lib/debug/osx64/libv8_nosnapshot.a)
				copy_file_to_binaries(/External/V8/lib/debug/osx64/libicudata.a)
				copy_file_to_binaries(/External/V8/lib/debug/osx64/libicuuc.a)
				copy_file_to_binaries(/External/V8/lib/debug/osx64/libicui18n.a)

				if(${OPIFEX_OS_64})
					target_link_libraries(${APPLICATION_TARGET}
						${PROJECT_SOURCE_DIR}/External/V8/lib/debug/osx64/libv8_nosnapshot.a)

					target_link_libraries(${APPLICATION_TARGET}
						${PROJECT_SOURCE_DIR}/External/V8/lib/debug/osx64/libv8_base.a)

					target_link_libraries(${APPLICATION_TARGET}
						${PROJECT_SOURCE_DIR}/External/V8/lib/debug/osx64/libv8_libbase.a)

					target_link_libraries(${APPLICATION_TARGET}
						${PROJECT_SOURCE_DIR}/External/V8/lib/debug/osx64/libv8_libplatform.a)

					target_link_libraries(${APPLICATION_TARGET}
						${PROJECT_SOURCE_DIR}/External/V8/lib/debug/osx64/libicudata.a)

					target_link_libraries(${APPLICATION_TARGET}
						${PROJECT_SOURCE_DIR}/External/V8/lib/debug/osx64/libicuuc.a)

					target_link_libraries(${APPLICATION_TARGET}
						${PROJECT_SOURCE_DIR}/External/V8/lib/debug/osx64/libicui18n.a)

					message(STATUS "LIBV8")
				else()
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/V8/lib/debug/win32/v8.lib)
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/V8/lib/debug/win32/v8_libbase.lib)
					target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/V8/lib/debug/win32/v8_libplatform.lib)
				endif()

			endif()
		endif()

	endif()

endmacro(add_opifex_v8)


macro(add_opifex_spine APPLICATION_TARGET )

	if(${OPIFEX_OPTION_SPINE})
		target_link_libraries(${APPLICATION_TARGET} SPINE)
	endif()

endmacro(add_opifex_spine)


macro(add_opifex_oculus APPLICATION_TARGET )

	if(${OPIFEX_OPTION_OCULUS})
		include_directories(../External/Oculus/include/	../External/Oculus/include/)
		target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/OculusDK2/lib/win32/VS2010/libovrd.lib)
	endif()

endmacro(add_opifex_oculus)


macro(add_opifex_myo APPLICATION_TARGET )

	if(${OPIFEX_OPTION_MYO})
		include_directories(../External/Myo/include/)
		if(${OPIFEX_OS_WIN64})
			target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/Myo/lib/win64/myo64.lib)
			copy_to_folder(${APPLICATION_TARGET}  "/External/Myo/lib/" "myo64.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
			copy_to_folder(${APPLICATION_TARGET}  "/External/Myo/lib/" "ble64.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
		elseif(${OPIFEX_OS_WIN32})
			target_link_libraries(${APPLICATION_TARGET}  ${PROJECT_SOURCE_DIR}/External/Myo/lib/win32/myo32.lib)
			copy_to_folder(${APPLICATION_TARGET}  "/External/Myo/lib/" "myo32.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
			copy_to_folder(${APPLICATION_TARGET}  "/External/Myo/lib/" "ble32.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
		elseif(${OPIFEX_OS_OSX64})
			target_link_libraries(${APPLICATION_TARGET}  ${PROJECT_SOURCE_DIR}/External/Myo/lib/osx64/myo.framework)
		endif()
	endif()

endmacro(add_opifex_myo)

macro(add_opifex_fmod APPLICATION_TARGET )

	if(${OPIFEX_OPTION_FMOD})
		include_directories(../External/FMod/include/)

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
		elseif(${OPIFEX_OS_OSX64})
			target_link_libraries(${APPLICATION_TARGET}  ${PROJECT_SOURCE_DIR}/External/FMod/lib/libfmod.dylib)
			target_link_libraries(${APPLICATION_TARGET}  ${PROJECT_SOURCE_DIR}/External/FMod/lib/libfmodL.dylib)
			copy_file_to_binaries(/External/FMod/lib/libfmod.dylib)
			copy_file_to_binaries(/External/FMod/lib/libfmodL.dylib)
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
	endif()

endmacro(add_opifex_fmod)


macro(add_opifex_oggvorbis APPLICATION_TARGET )

	if(${OPIFEX_OPTION_AUDIO})
		if(${OPIFEX_OPTION_RELEASE})
			if(${OPIFEX_WINDOWS})
				add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)
				target_link_libraries(${APPLICATION_TARGET}	libcpmtd0.lib)
				target_link_libraries(${APPLICATION_TARGET}	${OGG_LIBRARY_RELEASE} ${VORBIS_LIBRARY_RELEASE} ${VORBISFILE_LIBRARY_RELEASE})
			endif()
		else()
			target_link_libraries(${APPLICATION_TARGET}	${OGG_LIBRARY} ${VORBIS_LIBRARY} ${VORBISFILE_LIBRARY})
		endif()

		if(${OPIFEX_OPTION_RELEASE})
			copy_to_folder(${APPLICATION_TARGET} "/External/Ogg/lib/release/" "libogg.dll" "/Application/Release" ${OPIFEX_OS_WINDOWS})
			copy_to_folder(${APPLICATION_TARGET} "/External/Vorbis/lib/release/" "libvorbis.dll" "/Application/Release" ${OPIFEX_OS_WINDOWS})
			copy_to_folder(${APPLICATION_TARGET} "/External/Vorbis/lib/release/" "libvorbisfile.dll" "/Application/Release" ${OPIFEX_OS_WINDOWS})
		else()
			copy_to_folder_msvc_vs(${APPLICATION_TARGET} "/External/Ogg/lib/debug/" "libogg.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
			copy_to_folder_msvc_vs(${APPLICATION_TARGET} "/External/Vorbis/lib/debug/" "libvorbis.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
			copy_to_folder_msvc_vs(${APPLICATION_TARGET} "/External/Vorbis/lib/debug/" "libvorbisfile.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
		endif()


		if(${OPIFEX_OS_WIN64})
			if(${OPIFEX_OPTION_RELEASE})
				message(STATUS "Copying... OggVorbis Win64 Release")
				copy_file_to_binaries(/External/Ogg/lib/release/win64/ogg.dll)
				copy_file_to_binaries(/External/Ogg/lib/release/win64/libogg.dll)
				copy_file_to_binaries(/External/Vorbis/lib/release/win64/vorbisfile.dll)
				copy_file_to_binaries(/External/Vorbis/lib/release/win64/libvorbis.dll)
				copy_file_to_binaries(/External/Vorbis/lib/release/win64/libvorbisfile.dll)

				copy_file_to_binaries(/External/Ogg/lib/release/win64/libogg_static.lib)
				copy_file_to_binaries(/External/Vorbis/lib/release/win64/libvorbis_static.lib)
				copy_file_to_binaries(/External/Vorbis/lib/release/win64/libvorbisfile_static.lib)

			else()

				message(STATUS "Copying... OggVorbis Win64 Debug")
				copy_file_to_binaries(/External/Ogg/lib/debug/win64/ogg.dll)
				copy_file_to_binaries(/External/Ogg/lib/debug/win64/libogg.dll)
				copy_file_to_binaries(/External/Vorbis/lib/debug/win64/vorbisfile.dll)
				copy_file_to_binaries(/External/Vorbis/lib/debug/win64/libvorbis.dll)
				copy_file_to_binaries(/External/Vorbis/lib/debug/win64/libvorbisfile.dll)

				copy_file_to_binaries(/External/Ogg/lib/debug/win64/libogg.lib)
				copy_file_to_binaries(/External/Vorbis/lib/debug/win64/libvorbis.lib)
				copy_file_to_binaries(/External/Vorbis/lib/debug/win64/libvorbisfile.lib)

			endif()
		elseif(${OPIFEX_OS_WIN32})
			if(${OPIFEX_OPTION_RELEASE})

				message(STATUS "Copying... OggVorbis Win32 Release")
				copy_file_to_binaries(/External/Ogg/lib/release/win32/ogg.dll)
				copy_file_to_binaries(/External/Ogg/lib/release/win32/libogg.dll)
				copy_file_to_binaries(/External/Vorbis/lib/release/win32/vorbisfile.dll)
				copy_file_to_binaries(/External/Vorbis/lib/release/win32/libvorbis.dll)
				copy_file_to_binaries(/External/Vorbis/lib/release/win32/libvorbisfile.dll)

				copy_file_to_binaries(/External/Ogg/lib/release/win32/libogg_static.lib)
				copy_file_to_binaries(/External/Vorbis/lib/release/win32/libvorbis_static.lib)
				copy_file_to_binaries(/External/Vorbis/lib/release/win32/libvorbisfile_static.lib)

			else()


				if(${MSVC_VERSION} GREATER 1700)

					message(STATUS "Copying... OggVorbis Win32 Debug for ${MSVC_VERSION}")

					copy_file_to_binaries(/External/Ogg/lib/debug/win32/vs2013/ogg.dll)
					copy_file_to_binaries(/External/Ogg/lib/debug/win32/vs2013/libogg.dll)
					copy_file_to_binaries(/External/Vorbis/lib/debug/win32/vs2013/vorbisfile.dll)
					copy_file_to_binaries(/External/Vorbis/lib/debug/win32/vs2013/libvorbis.dll)
					copy_file_to_binaries(/External/Vorbis/lib/debug/win32/vs2013/libvorbisfile.dll)

					copy_file_to_binaries(/External/Ogg/lib/debug/win32/vs2013/libogg.lib)
					copy_file_to_binaries(/External/Vorbis/lib/debug/win32/vs2013/libvorbis.lib)
					copy_file_to_binaries(/External/Vorbis/lib/debug/win32/vs2013/libvorbisfile.lib)


				else()

					copy_file_to_binaries(/External/Ogg/lib/debug/win32/ogg.dll)
					copy_file_to_binaries(/External/Ogg/lib/debug/win32/libogg.dll)
					copy_file_to_binaries(/External/Vorbis/lib/debug/win32/vorbisfile.dll)
					copy_file_to_binaries(/External/Vorbis/lib/debug/win32/libvorbis.dll)
					copy_file_to_binaries(/External/Vorbis/lib/debug/win32/libvorbisfile.dll)

					copy_file_to_binaries(/External/Ogg/lib/debug/win32/libogg.lib)
					copy_file_to_binaries(/External/Vorbis/lib/debug/win32/libvorbis.lib)
					copy_file_to_binaries(/External/Vorbis/lib/debug/win32/libvorbisfile.lib)

				endif()

			endif()
		endif()
	endif()
endmacro(add_opifex_oggvorbis)


macro(add_opifex_steam APPLICATION_TARGET )

	if(${OPIFEX_OPTION_STEAM})

		message(STATUS "Link Steam")
		include_directories(../External/Steam/include/)

		message(STATUS "Steam")
		target_link_libraries(${APPLICATION_TARGET} ${PROJECT_SOURCE_DIR}/External/Steam/lib/win32/steam_api.lib)

		copy_to_folder(${APPLICATION_TARGET} "/External/Steam/lib/" "steam_api.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})

	endif()

endmacro(add_opifex_steam)

macro(add_opifex_xinput APPLICATION_TARGET )

	if(${OPIFEX_OS_WIN32} OR ${OPIFEX_OS_WIN64})
		message(STATUS "link xinput")
		target_link_libraries(${APPLICATION_TARGET} XInput9_1_0.lib)
	endif()

endmacro(add_opifex_xinput)
