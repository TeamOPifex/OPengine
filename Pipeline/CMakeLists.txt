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
add_source_group(include/Loaders/ "${PROJECT_SOURCE_DIR}/Pipeline/include/Loaders" PIPELINE_SRC)


#########################################
# Source Files
#########################################
add_source_group(\\ "${PROJECT_SOURCE_DIR}/Pipeline" PIPELINE_SRC)
add_source_group(src/ "${PROJECT_SOURCE_DIR}/Pipeline/src" PIPELINE_SRC)
add_source_group(src/Loaders "${PROJECT_SOURCE_DIR}/Pipeline/src/Loaders" PIPELINE_SRC)


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
if(${OPIFEX_SHARED})
	add_library(8_Pipeline SHARED ${PIPELINE_SRC})
else()
	add_library(8_Pipeline STATIC ${PIPELINE_SRC})
endif()

set_target_properties(8_Pipeline PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(8_Pipeline PROPERTIES OUTPUT_NAME "Pipeline")
add_definitions(-D${OPIFEX_OS})

if(${OPIFEX_RELEASE})
	add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)
endif()

target_link_libraries(8_Pipeline ${ALL_LIBS})


add_opifex_assets(8_Pipeline)
add_opifex_spine(8_Pipeline)
add_opifex_v8(8_Pipeline)

#########################################
# OUTPUT BINARIES
#########################################
output_library(8_Pipeline "Pipeline")