
set(OP_ASSETS_TARGET "${PROJECT_BINARY_DIR}/assets")

if( ${OPIFEX_OS_WINDOWS})
	set(OP_ASSETS_TARGET "${PROJECT_BINARY_DIR}/Application/assets")
endif()

if( ${OPIFEX_OS_ANDROID} )

	set(OP_ASSETS_TARGET "${PROJECT_BINARY_DIR}/assets")

	file(COPY ${PROJECT_SOURCE_DIR}/Android/Make/AndroidManifest.xml DESTINATION ${PROJECT_BINARY_DIR}/)
	file(COPY ${PROJECT_SOURCE_DIR}/Android/Make/build.xml DESTINATION ${PROJECT_BINARY_DIR}/)
	file(COPY ${PROJECT_SOURCE_DIR}/Android/Make/Install.bat DESTINATION ${PROJECT_BINARY_DIR}/)

	file(COPY ${PROJECT_SOURCE_DIR}/Android/Libs/ouya-sdk.jar DESTINATION ${PROJECT_BINARY_DIR}/libs/)

	configure_file(		${PROJECT_SOURCE_DIR}/Android/Make/Android.mk		${PROJECT_BINARY_DIR}/jni/Android.mk	)
	configure_file(		${PROJECT_SOURCE_DIR}/Android/Make/Application.mk		${PROJECT_BINARY_DIR}/jni/Application.mk	)
	configure_file(		${PROJECT_SOURCE_DIR}/Android/Make/strings.xml		${PROJECT_BINARY_DIR}/res/values/strings.xml	)


	file(COPY ${PROJECT_SOURCE_DIR}/External/Ogg/lib/debug/android/libogg.a DESTINATION ${PROJECT_BINARY_DIR}/Binaries/android)
	file(COPY ${PROJECT_SOURCE_DIR}/External/Vorbis/lib/debug/android/libvorbis.a DESTINATION ${PROJECT_BINARY_DIR}/Binaries/android)

	file(COPY ${PROJECT_SOURCE_DIR}/External/PhysX/Lib/release/android/libPhysX3Common.a 	DESTINATION 	${PROJECT_BINARY_DIR}/Binaries/android)
	file(COPY ${PROJECT_SOURCE_DIR}/External/PhysX/Lib/release/android/libPvdRuntime.a 	DESTINATION 	${PROJECT_BINARY_DIR}/Binaries/android)
	file(COPY ${PROJECT_SOURCE_DIR}/External/PhysX/Lib/release/android/libSimulationController.a 	DESTINATION 	${PROJECT_BINARY_DIR}/Binaries/android)
	file(COPY ${PROJECT_SOURCE_DIR}/External/PhysX/Lib/release/android/libSceneQuery.a 	DESTINATION 	${PROJECT_BINARY_DIR}/Binaries/android)
	file(COPY ${PROJECT_SOURCE_DIR}/External/PhysX/Lib/release/android/libLowLevel.a 	DESTINATION 	${PROJECT_BINARY_DIR}/Binaries/android)
	file(COPY ${PROJECT_SOURCE_DIR}/External/PhysX/Lib/release/android/libLowLevelCloth.a 	DESTINATION 	${PROJECT_BINARY_DIR}/Binaries/android)
	file(COPY ${PROJECT_SOURCE_DIR}/External/PhysX/Lib/release/android/libPhysX3.a 	DESTINATION 	${PROJECT_BINARY_DIR}/Binaries/android)
	file(COPY ${PROJECT_SOURCE_DIR}/External/PhysX/Lib/release/android/libPhysX3Vehicle.a 	DESTINATION 	${PROJECT_BINARY_DIR}/Binaries/android)
	file(COPY ${PROJECT_SOURCE_DIR}/External/PhysX/Lib/release/android/libPhysX3Cooking.a 	DESTINATION 	${PROJECT_BINARY_DIR}/Binaries/android)
	file(COPY ${PROJECT_SOURCE_DIR}/External/PhysX/Lib/release/android/libPhysX3Extensions.a 	DESTINATION 	${PROJECT_BINARY_DIR}/Binaries/android)
	file(COPY ${PROJECT_SOURCE_DIR}/External/PhysX/Lib/release/android/libPhysX3CharacterKinematic.a 	DESTINATION 	${PROJECT_BINARY_DIR}/Binaries/android)
	file(COPY ${PROJECT_SOURCE_DIR}/External/PhysX/Lib/release/android/libPhysXProfileSDK.a 	DESTINATION 	${PROJECT_BINARY_DIR}/Binaries/android)
	file(COPY ${PROJECT_SOURCE_DIR}/External/PhysX/Lib/release/android/libPxTask.a 	DESTINATION 	${PROJECT_BINARY_DIR}/Binaries/android)
	file(COPY ${PROJECT_SOURCE_DIR}/External/PhysX/Lib/release/android/libPhysXVisualDebuggerSDK.a 	DESTINATION 	${PROJECT_BINARY_DIR}/Binaries/android)

else()
	if(${OPIFEX_OPTION_NODEJS})
		message(STATUS "BINDING.GYP")
		if(${OPIFEX_OPTION_RELEASE})
			message(STATUS "RELEASE MODE BINDING.GYP")
			configure_file(		${PROJECT_SOURCE_DIR}/NodeJs/bindingRelease.gyp		${PROJECT_BINARY_DIR}/binding.gyp	)
		else()
			message(STATUS "DEBUG MODE BINDING.GYP")
			configure_file(		${PROJECT_SOURCE_DIR}/NodeJs/binding.gyp		${PROJECT_BINARY_DIR}/binding.gyp	)
		endif()
	endif()

endif()




# Only copy assets if we're in release mode
# It pulls from the Repository otherwise
if(${OPIFEX_OPTION_RELEASE} OR ${OPIFEX_OS_ANDROID})

	if( ${OPIFEX_OS_WINDOWS})
		set(OP_ASSETS_TARGET "${PROJECT_BINARY_DIR}/Application/Release/assets")
	endif()
	copy_source_group("${PROJECT_SOURCE_DIR}/Assets/Audio" ${OP_ASSETS_TARGET}/Audio)
	copy_source_group("${PROJECT_SOURCE_DIR}/Assets/Models" ${OP_ASSETS_TARGET}/Models)
	copy_source_group("${PROJECT_SOURCE_DIR}/Assets/Textures" ${OP_ASSETS_TARGET}/Textures)
	copy_source_group("${PROJECT_SOURCE_DIR}/Assets/Fonts" ${OP_ASSETS_TARGET}/Fonts)
	copy_source_group("${PROJECT_SOURCE_DIR}/Assets/Sprites" ${OP_ASSETS_TARGET}/Sprites)
	copy_source_group("${PROJECT_SOURCE_DIR}/Assets/Scripts" ${OP_ASSETS_TARGET}/Scripts)
	copy_source_group("${PROJECT_SOURCE_DIR}/Assets/Shaders/${OPENGL_DESKTOP_TARGET}" ${OP_ASSETS_TARGET}/Shaders)
	copy_source_group("${PROJECT_SOURCE_DIR}/Assets/Shaders/${OPENGL_DESKTOP_TARGET}/Common" ${OP_ASSETS_TARGET}/Shaders/Common)
	copy_source_group("${PROJECT_SOURCE_DIR}/Assets/Web" ${OP_ASSETS_TARGET}/Web)
	copy_source_group("${PROJECT_SOURCE_DIR}/Assets/Web/css" ${OP_ASSETS_TARGET}/Web/css)
	copy_source_group("${PROJECT_SOURCE_DIR}/Assets/Web/fonts" ${OP_ASSETS_TARGET}/Web/fonts)
	copy_source_group("${PROJECT_SOURCE_DIR}/Assets/Web/images" ${OP_ASSETS_TARGET}/Web/images)
	copy_source_group("${PROJECT_SOURCE_DIR}/Assets/Web/js" ${OP_ASSETS_TARGET}/Web/js)
	copy_source_group("${PROJECT_SOURCE_DIR}/Assets/Web/js/angular" ${OP_ASSETS_TARGET}/Web/angular)
	copy_source_group("${PROJECT_SOURCE_DIR}/Assets/Web/js/angular/controllers" ${OP_ASSETS_TARGET}/Web/angular/controllers)
	copy_source_group("${PROJECT_SOURCE_DIR}/Assets/Web/js/angular/directives" ${OP_ASSETS_TARGET}/Web/angular/directives)
	copy_source_group("${PROJECT_SOURCE_DIR}/Assets/Web/js/angular/filters" ${OP_ASSETS_TARGET}/Web/angular/filters)
	copy_source_group("${PROJECT_SOURCE_DIR}/Assets/Web/js/angular/services" ${OP_ASSETS_TARGET}/Web/angular/services)

	# message(STATUS "COPYING FILES")

endif()


#file(GLOB FILES_RSC "${PROJECT_SOURCE_DIR}/Assets/Audio")
#set_source_files_properties(${FILES_RSC} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
