cmake_minimum_required(VERSION 2.8)

option(OPIFEX_TOOLS_FONTMAKER "Add the Font Maker Tool" OFF)
option(OPIFEX_TOOLS_PROJECTBUILDER "Add the Project Builder Tool" OFF)
option(OPIFEX_TOOLS_FBXTOOPM "Add the FBX to OPM Tool" OFF)

SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

#################################
## Font Maker
#################################

if(${OPIFEX_TOOLS_FONTMAKER})

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

	add_source_group(\\ "${PROJECT_SOURCE_DIR}/Tools/FontMaker" FONTMAKER_SRC)
	set(ALL_LIBS
		1_Core
		2_Data
		3_Math
		4_Performance
		5_Human
	)

	add_executable(Tools_FontMaker ${FONTMAKER_SRC})

	target_link_libraries(Tools_FontMaker ${ALL_LIBS})	

	if(${OPIFEX_RELEASE})
		target_link_libraries( Tools_FontMaker ${PROJECT_SOURCE_DIR}/External/freetype/libs/release/win32/freetype253.lib )	
	else()
		target_link_libraries( Tools_FontMaker ${PROJECT_SOURCE_DIR}/External/freetype/libs/debug/win32/freetype253_D.lib )	
	endif()

	add_definitions(-D${OPIFEX_OS})

	SET_PROPERTY(TARGET Tools_FontMaker                PROPERTY FOLDER "Tools")

	set_target_properties(Tools_FontMaker PROPERTIES LINKER_LANGUAGE CXX)
	set_target_properties(Tools_FontMaker PROPERTIES 
		OUTPUT_NAME "FontMaker"
	)
endif()


#################################
## Project Builder
#################################

if(${OPIFEX_TOOLS_PROJECTBUILDER})
	add_source_group(\\ "${PROJECT_SOURCE_DIR}/Tools/ProjectBuilder" PROJECTBUILDER_SRC)
	add_executable(Tools_ProjectBuilder ${PROJECTBUILDER_SRC})
	target_link_libraries(Tools_ProjectBuilder ${ALL_LIBS})	
	SET_PROPERTY(TARGET Tools_ProjectBuilder                PROPERTY FOLDER "Tools")
	set_target_properties(Tools_ProjectBuilder PROPERTIES LINKER_LANGUAGE CXX)
	set_target_properties(Tools_ProjectBuilder PROPERTIES 
		OUTPUT_NAME "ProjectBuilder"
	)
endif()


#################################
## FBX to OPM
#################################

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
		"C:/Program Files/Autodesk/FBX/FBX SDK/2015.1/include"
	)

	add_source_group(\\ "${PROJECT_SOURCE_DIR}/Tools/FBXtoOPM" FBXTOOPM_SRC)
	set(ALL_LIBS
		1_Core
		2_Data
		3_Math
		4_Performance
		5_Human
		"C:/Program Files/Autodesk/FBX/FBX SDK/2015.1/lib/vs2013/x86/debug/libfbxsdk-mt.lib"
	)

	add_executable(Tools_FBXtoOPM ${FBXTOOPM_SRC})

	target_link_libraries(Tools_FBXtoOPM ${ALL_LIBS})	

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