#########################################
# SCRIPTING - LAYER 7
#########################################
cmake_minimum_required(VERSION 2.8)


#########################################
# Include Directories
#########################################
include_directories(
	../External/glfw-3.0.4/include/
	../External/glm-0.9.1/
	../External/glew-1.9.0/include/
	../External/Phsyx.3.2.1/include/
	../External/GLES2/
	../External/Ogg/include/
	../External/Vorbis/include/
	../External/OpenAL/
	../External/Oculus/include/
	../External/Oculus/Src/
	../External/Myo/include/
	../External/V8/include/
	../External/V8/
	.
	../
)

#########################################
# Include Files
#########################################
add_source_group(include/ "${PROJECT_SOURCE_DIR}/Scripting/include" SCRIPTING_SRC)
add_source_group(include/wrappers "${PROJECT_SOURCE_DIR}/Scripting/include/wrappers" SCRIPTING_SRC)


#########################################
# Source Files
#########################################
add_source_group(\\ "${PROJECT_SOURCE_DIR}/Scripting" SCRIPTING_SRC)
add_source_group(src/ "${PROJECT_SOURCE_DIR}/Scripting/src" SCRIPTING_SRC)
add_source_group(src/wrappers "${PROJECT_SOURCE_DIR}/Scripting/src/wrappers" SCRIPTING_SRC)


#########################################
# DEPENDENT LIBRARIES
#########################################
set(ALL_LIBS
	${OPENGL_LIBRARY}
	${OPENAL_LIBRARY}
	GLEW_158
	LODEPNG
	1_Core
	2_Data
	3_Math
	4_Performance
	5_Human
	${OGG_LIBRARY}
	${VORBIS_LIBRARY}
	${VORBISFILE_LIBRARY}
	glfw 
	${GLFW_LIBRARIES}
)

#########################################
# BUILD
#########################################
add_library(7_Scripting STATIC ${SCRIPTING_SRC})
target_link_libraries(7_Scripting ${ALL_LIBS})
set_target_properties(7_Scripting PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(7_Scripting PROPERTIES OUTPUT_NAME "Scripting")

add_definitions(-D${OPIFEX_OS})
if(${OPIFEX_RELEASE})
	add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)
endif()

if(${OPIFEX_V8})
	if(${OPIFEX_OS_WINDOWS})
		target_link_libraries(7_Scripting 
			ws2_32.lib
			advapi32.lib
			winmm.lib)
		if(${OPIFEX_RELEASE})
			if(${OPIFEX_OS_64})
				target_link_libraries(7_Scripting ${PROJECT_SOURCE_DIR}/External/V8/lib/release/win64/v8.lib)
			else()
				target_link_libraries(7_Scripting ${PROJECT_SOURCE_DIR}/External/V8/lib/release/win32/v8.lib)
				target_link_libraries(7_Scripting ${PROJECT_SOURCE_DIR}/External/V8/lib/release/win32/v8_libbase.lib)
				target_link_libraries(7_Scripting ${PROJECT_SOURCE_DIR}/External/V8/lib/release/win32/v8_libplatform.lib)
			endif()

		else()
		
			if(${OPIFEX_OS_64})
				target_link_libraries(7_Scripting ${PROJECT_SOURCE_DIR}/External/V8/lib/debug/win64/v8.lib)
			else()
				target_link_libraries(7_Scripting ${PROJECT_SOURCE_DIR}/External/V8/lib/debug/win32/v8.lib)
				target_link_libraries(7_Scripting ${PROJECT_SOURCE_DIR}/External/V8/lib/debug/win32/v8_libbase.lib)
				target_link_libraries(7_Scripting ${PROJECT_SOURCE_DIR}/External/V8/lib/debug/win32/v8_libplatform.lib)
			endif()
		
		endif()
	endif()
	add_definitions(-DOPIFEX_V8)
endif()

#########################################
# OUTPUT BINARIES
#########################################
output_library(7_Scripting "Scripting")

if(${OPIFEX_V8})
	if(${OPIFEX_RELEASE})
		output_binary(7_Scripting "/External/V8/lib/release/" "icui18n.dll" ${OPIFEX_OS_WINDOWS})
		output_binary(7_Scripting "/External/V8/lib/release/" "icuuc.dll" ${OPIFEX_OS_WINDOWS})
		output_binary(7_Scripting "/External/V8/lib/release/" "v8.dll" ${OPIFEX_OS_WINDOWS})
	else()	
		output_binary(7_Scripting "/External/V8/lib/debug/" "icui18n.dll" ${OPIFEX_OS_WINDOWS})
		output_binary(7_Scripting "/External/V8/lib/debug/" "icuuc.dll" ${OPIFEX_OS_WINDOWS})
		output_binary(7_Scripting "/External/V8/lib/debug/" "v8.dll" ${OPIFEX_OS_WINDOWS})
	endif()
endif()