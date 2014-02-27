#########################################
# PIPELINE - LAYER 8
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


#########################################
# BUILD
#########################################
add_library(8_Pipeline STATIC ${PIPELINE_SRC})
set_target_properties(8_Pipeline PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(8_Pipeline PROPERTIES OUTPUT_NAME "Pipeline")

if(${OPIFEX_V8})
	if(${OPIFEX_OS_WINDOWS})
		target_link_libraries(8_Pipeline 
			ws2_32.lib
			advapi32.lib
			winmm.lib)
		if(${OPIFEX_OS_64})
			target_link_libraries(8_Pipeline ${PROJECT_SOURCE_DIR}/External/V8/lib/win64/v8.lib)
		else()
			target_link_libraries(8_Pipeline ${PROJECT_SOURCE_DIR}/External/V8/lib/win32/v8.lib)
		endif()
	endif()
	add_definitions(-DOPIFEX_V8)
endif()

#########################################
# OUTPUT BINARIES
#########################################
output_library(8_Pipeline "Pipeline")

if(${OPIFEX_V8})
	output_binary(8_Pipeline "/External/V8/lib/" "icui18n.dll" ${OPIFEX_OS_WINDOWS})
	output_binary(8_Pipeline "/External/V8/lib/" "icuuc.dll" ${OPIFEX_OS_WINDOWS})
	output_binary(8_Pipeline "/External/V8/lib/" "v8.dll" ${OPIFEX_OS_WINDOWS})
endif()