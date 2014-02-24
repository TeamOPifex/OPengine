
if( ${OPIFEX_OS_ANDROID} )
	
	file(COPY ${PROJECT_SOURCE_DIR}/Android/Make/AndroidManifest.xml DESTINATION ${PROJECT_BINARY_DIR}/)
	file(COPY ${PROJECT_SOURCE_DIR}/Android/Make/build.xml DESTINATION ${PROJECT_BINARY_DIR}/)
	file(COPY ${PROJECT_SOURCE_DIR}/Android/Make/Install.bat DESTINATION ${PROJECT_BINARY_DIR}/)
	file(COPY ${PROJECT_SOURCE_DIR}/Android/Java/GamePadButtonState.java DESTINATION ${PROJECT_BINARY_DIR}/src/com/opifex/SMRF/)	
	file(COPY ${PROJECT_SOURCE_DIR}/Android/Java/GamePadAxes.java DESTINATION ${PROJECT_BINARY_DIR}/src/com/opifex/SMRF/)
	file(COPY ${PROJECT_SOURCE_DIR}/Android/Java/GamePadButtons.java DESTINATION ${PROJECT_BINARY_DIR}/src/com/opifex/SMRF/)
	file(COPY ${PROJECT_SOURCE_DIR}/Android/Java/ControllerState.java DESTINATION ${PROJECT_BINARY_DIR}/src/com/opifex/SMRF/)
	file(COPY ${PROJECT_SOURCE_DIR}/Android/Java/GL2JNILib.java DESTINATION ${PROJECT_BINARY_DIR}/src/com/opifex/SMRF/)
	file(COPY ${PROJECT_SOURCE_DIR}/Android/Java/GL2JNIView.java DESTINATION ${PROJECT_BINARY_DIR}/src/com/opifex/SMRF/)
	file(COPY ${PROJECT_SOURCE_DIR}/Android/Java/OPEngine.java DESTINATION ${PROJECT_BINARY_DIR}/src/com/opifex/SMRF/)
		
	file(COPY ${PROJECT_SOURCE_DIR}/Android/Libs/ouya-sdk.jar DESTINATION ${PROJECT_BINARY_DIR}/libs/)
	
	configure_file(		${PROJECT_SOURCE_DIR}/Android/Make/Android.mk		${PROJECT_BINARY_DIR}/jni/Android.mk	)
	configure_file(		${PROJECT_SOURCE_DIR}/Android/Make/Application.mk		${PROJECT_BINARY_DIR}/jni/Application.mk	)
	configure_file(		${PROJECT_SOURCE_DIR}/Android/Make/strings.xml		${PROJECT_BINARY_DIR}/res/values/strings.xml	)
	
	copy_source_group("${PROJECT_SOURCE_DIR}/Assets/Audio" ${PROJECT_BINARY_DIR}/assets/Audio)
	copy_source_group("${PROJECT_SOURCE_DIR}/Assets/Models" ${PROJECT_BINARY_DIR}/assets/Models)
	copy_source_group("${PROJECT_SOURCE_DIR}/Assets/Textures" ${PROJECT_BINARY_DIR}/assets/Textures)
	copy_source_group("${PROJECT_SOURCE_DIR}/Assets/Fonts" ${PROJECT_BINARY_DIR}/assets/Fonts)
	copy_source_group("${PROJECT_SOURCE_DIR}/Assets/Shaders/OPENGL_ES_2_0" ${PROJECT_BINARY_DIR}/assets/Shaders)	
	copy_source_group("${PROJECT_SOURCE_DIR}/Assets/Shaders/OPENGL_ES_2_0/Common" ${PROJECT_BINARY_DIR}/assets/Shaders/Common)	

else()

	copy_source_group("${PROJECT_SOURCE_DIR}/Assets/Audio" ${PROJECT_BINARY_DIR}/Application/assets/Audio)	
	copy_source_group("${PROJECT_SOURCE_DIR}/Assets/Models" ${PROJECT_BINARY_DIR}/Application/assets/Models)	
	copy_source_group("${PROJECT_SOURCE_DIR}/Assets/Textures" ${PROJECT_BINARY_DIR}/Application/assets/Textures)	
	copy_source_group("${PROJECT_SOURCE_DIR}/Assets/Fonts" ${PROJECT_BINARY_DIR}/Application/assets/Fonts)	
	copy_source_group("${PROJECT_SOURCE_DIR}/Assets/Scripts" ${PROJECT_BINARY_DIR}/Application/assets/Scripts)	
	copy_source_group("${PROJECT_SOURCE_DIR}/Assets/Shaders/${OPENGL_DESKTOP_TARGET}" ${PROJECT_BINARY_DIR}/Application/assets/Shaders)	
	copy_source_group("${PROJECT_SOURCE_DIR}/Assets/Shaders/${OPENGL_DESKTOP_TARGET}/Common" ${PROJECT_BINARY_DIR}/Application/assets/Shaders/Common)

endif()