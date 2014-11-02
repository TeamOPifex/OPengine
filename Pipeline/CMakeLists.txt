#########################################
# PIPELINE - LAYER 8
#########################################
cmake_minimum_required(VERSION 2.8)


#########################################
# Include Directories
#########################################
include_directories(
	../External/glfw-3.0.4/include/
	../External/glm-0.9.5/
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
	../External/Spine/include/
	.
	../
)

#########################################
# Include Files
#########################################
add_source_group(include/ "${PROJECT_SOURCE_DIR}/Pipeline/include" PIPELINE_SRC)


#########################################
# Source Files
#########################################
add_source_group(\\ "${PROJECT_SOURCE_DIR}/Pipeline" PIPELINE_SRC)
add_source_group(src/ "${PROJECT_SOURCE_DIR}/Pipeline/src" PIPELINE_SRC)


#########################################
# DEPENDENT LIBRARIES
#########################################
set(ALL_LIBS
	${OPENGL_LIBRARY}
	${OPENAL_LIBRARY}
	GLEW_158
	1_Core
	2_Data
	3_Math
	4_Performance
	5_Human
	7_Scripting
	glfw 
	${GLFW_LIBRARIES}
)

#########################################
# BUILD
#########################################
add_library(8_Pipeline STATIC ${PIPELINE_SRC})
set_target_properties(8_Pipeline PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(8_Pipeline PROPERTIES OUTPUT_NAME "Pipeline")
add_definitions(-D${OPIFEX_OS})

if(${OPIFEX_RELEASE})
	add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)
endif()

target_link_libraries(8_Pipeline ${ALL_LIBS})

if(${OPIFEX_RELEASE})

else()
	add_definitions(-D_DEBUG)
endif()

if(${OPIFEX_SPINE})
	target_link_libraries(8_Pipeline SPINE)
	add_definitions(-DOPIFEX_SPINE)
endif()

if(${OPIFEX_V8})
	if(${OPIFEX_OS_WINDOWS})
		target_link_libraries(8_Pipeline 
			ws2_32.lib
			advapi32.lib
			winmm.lib)
		if(${OPIFEX_RELEASE})
			if(${OPIFEX_OS_64})
				target_link_libraries(8_Pipeline ${PROJECT_SOURCE_DIR}/External/V8/lib/release/win64/v8.lib)
			else()
				target_link_libraries(8_Pipeline ${PROJECT_SOURCE_DIR}/External/V8/lib/release/win32/v8.lib)
				target_link_libraries(8_Pipeline ${PROJECT_SOURCE_DIR}/External/V8/lib/release/win32/v8_libbase.lib)
				target_link_libraries(8_Pipeline ${PROJECT_SOURCE_DIR}/External/V8/lib/release/win32/v8_libplatform.lib)
			endif()

		else()
		
			if(${OPIFEX_OS_64})
				target_link_libraries(8_Pipeline ${PROJECT_SOURCE_DIR}/External/V8/lib/debug/win64/v8.lib)
			else()
				target_link_libraries(8_Pipeline ${PROJECT_SOURCE_DIR}/External/V8/lib/debug/win32/v8.lib)
				target_link_libraries(8_Pipeline ${PROJECT_SOURCE_DIR}/External/V8/lib/debug/win32/v8_libbase.lib)
				target_link_libraries(8_Pipeline ${PROJECT_SOURCE_DIR}/External/V8/lib/debug/win32/v8_libplatform.lib)
			endif()
		
		endif()
	endif()
	add_definitions(-DOPIFEX_V8)
endif()

#########################################
# OUTPUT BINARIES
#########################################
output_library(8_Pipeline "Pipeline")

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