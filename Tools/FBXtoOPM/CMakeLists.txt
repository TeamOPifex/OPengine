
if(${OPIFEX_TOOLS_FBXTOOPM})

	include_directories(
		../External/freetype/win32/
		../External/glfw-3.0.4/include/
		../External/glm-0.9.1/
		../External/glew-1.5.8/include/
		../External/Phsyx.3.2.1/include/
		../External/GLES2/
		../External/Ogg/include/
		../External/Vorbis/include/
		../External/OpenAL/
		.
		../
		
	)

	if(${OPIFEX_OS_WINDOWS})
		include_directories("C:/Program Files/Autodesk/FBX/FBX SDK/2015.1/include")
	endif()	
	if(${OPIFEX_OS_OSX})
		include_directories("/Applications/Autodesk/FBX SDK/2015.1/include")
	endif()


	add_source_group(\\ "${PROJECT_SOURCE_DIR}/Tools/FBXtoOPM" FBXTOOPM_SRC)
	
	set(ALL_LIBS
		1_Core
		2_Data
		3_Math
		4_Performance
		5_Human
	)


	add_executable(Tools_FBXtoOPM ${FBXTOOPM_SRC})


	target_link_libraries(Tools_FBXtoOPM ${ALL_LIBS})	


	if(${OPIFEX_OS_WINDOWS})
		target_link_libraries(Tools_FBXtoOPM "C:/Program Files/Autodesk/FBX/FBX SDK/2015.1/lib/vs2013/x86/debug/libfbxsdk-mt.lib")
	endif()	
	if(${OPIFEX_OS_OSX})
		target_link_libraries(Tools_FBXtoOPM "/Applications/Autodesk/FBX\ SDK/2015.1/lib/clang/debug/libfbxsdk.a")
	endif()


	add_definitions(-D${OPIFEX_OS})


	SET_PROPERTY(TARGET Tools_FBXtoOPM                PROPERTY FOLDER "Tools")

	set_target_properties(Tools_FBXtoOPM PROPERTIES LINKER_LANGUAGE CXX)
	set_target_properties(Tools_FBXtoOPM PROPERTIES 
		OUTPUT_NAME "FBXtoOPM"
	)

	if(${OPIFEX_RELEASE})		
		#add_custom_command(TARGET ${Tools_FBXtoOPM} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
		#		"C:/Program Files/Autodesk/FBX/FBX SDK/2015.1/lib/vs2013/x86/release/libfbxsdk.dll"
		#		"${PROJECT_BINARY_DIR}/Release")
	else()	
		#add_custom_command(TARGET ${Tools_FBXtoOPM} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
		#		"C:/Program Files/Autodesk/FBX/FBX SDK/2015.1/lib/vs2013/x86/debug/libfbxsdk.dll"
		#		"${PROJECT_BINARY_DIR}/Release")
	endif()

endif()