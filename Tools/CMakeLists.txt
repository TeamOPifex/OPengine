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
		.
		../
	)
	add_external_opifex_includes()

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

	if(${OPIFEX_OPTION_RELEASE})
		target_link_libraries( Tools_FontMaker ${PROJECT_SOURCE_DIR}/External/freetype/libs/release/win32/freetype253.lib )	
	else()
		target_link_libraries( Tools_FontMaker ${PROJECT_SOURCE_DIR}/External/freetype/libs/debug/win32/freetype253_D.lib )	
	endif()

	add_definitions(-D${OPIFEX_OS})
	if(${OPIFEX_OPTION_RELEASE})
		add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)
	endif()

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
		.
		../
		
	)
	add_external_opifex_includes()

	if(${OPIFEX_OS_WINDOWS})
		include_directories("C:/Program Files/Autodesk/FBX/FBX SDK/2015.1/include")
	endif()	
	
	if(${OPIFEX_OS_OSX})
		include_directories("/Applications/Autodesk/FBX SDK/2015.1/include")
	endif()


	add_source_group(/ "${PROJECT_SOURCE_DIR}/Tools/FBXtoOPM" FBXTOOPM_SRC)
	
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
		target_link_libraries(Tools_FBXtoOPM "/Applications/Autodesk/FBX\ SDK/2015.1/lib/clang/debug/libfbxsdk.dylib")
	endif()


	add_definitions(-D${OPIFEX_OS})


	SET_PROPERTY(TARGET Tools_FBXtoOPM                PROPERTY FOLDER "Tools")

	set_target_properties(Tools_FBXtoOPM PROPERTIES LINKER_LANGUAGE CXX)
	set_target_properties(Tools_FBXtoOPM PROPERTIES 
		OUTPUT_NAME "FBXtoOPM"
	)

	if(${OPIFEX_OPTION_RELEASE})

		add_definitions(-DOPIFEX_RELEASE)
		
		#add_custom_command(TARGET ${Tools_FBXtoOPM} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
		#		"C:/Program Files/Autodesk/FBX/FBX SDK/2015.1/lib/vs2013/x86/release/libfbxsdk.dll"
		#		"${PROJECT_BINARY_DIR}/Release")
	else()	
		#add_custom_command(TARGET ${Tools_FBXtoOPM} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
		#		"C:/Program Files/Autodesk/FBX/FBX SDK/2015.1/lib/vs2013/x86/debug/libfbxsdk.dll"
		#		"${PROJECT_BINARY_DIR}/Release")
	endif()

endif()