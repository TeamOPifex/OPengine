#########################################
# PIPELINE - LAYER 8
#########################################
cmake_minimum_required(VERSION 2.8)


#########################################
# Include Directories
#########################################
include_directories(
	.
	../
)

add_external_opifex_includes()

#########################################
# Include Files
#########################################
add_source_group(include/ "${PROJECT_SOURCE_DIR}/Pipeline/include" PIPELINE_SRC)
add_source_group(include/Loaders/ "${PROJECT_SOURCE_DIR}/Pipeline/include/Loaders" PIPELINE_SRC)


#########################################
# Source Files
#########################################
add_source_group(/ "${PROJECT_SOURCE_DIR}/Pipeline" PIPELINE_SRC)
add_source_group(src/ "${PROJECT_SOURCE_DIR}/Pipeline/src" PIPELINE_SRC)
add_source_group(src/Loaders "${PROJECT_SOURCE_DIR}/Pipeline/src/Loaders" PIPELINE_SRC)


#########################################
# DEPENDENT LIBRARIES
#########################################
set(ALL_LIBS
	${OPENGL_LIBRARY}
	${OPENAL_LIBRARY}
	LODEPNG
	1_Core
	2_Data
	3_Math
	4_Performance
	5_Human
	#GLEW_158
	#glfw ${GLFW_LIBRARIES}
)

#########################################
# BUILD
#########################################
if(${OPIFEX_OPTION_SHARED})
	add_library(7_Pipeline SHARED ${PIPELINE_SRC})
else()
	add_library(7_Pipeline STATIC ${PIPELINE_SRC})
endif()
set_xcode_property(7_Pipeline GCC_GENERATE_DEBUGGING_SYMBOLS YES)
add_opifex_defines()
add_definitions(-DGLEW_STATIC)

set_target_properties(7_Pipeline PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(7_Pipeline PROPERTIES OUTPUT_NAME "Pipeline")

if(${OPIFEX_OPTION_RELEASE})
	add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)
endif()

target_link_libraries(7_Pipeline ${ALL_LIBS})


add_opifex_assets(7_Pipeline)
add_opifex_spine(7_Pipeline)
add_opifex_v8(7_Pipeline)
add_opifex_fmod(7_Pipeline)

#########################################
# OUTPUT BINARIES
#########################################
output_library(7_Pipeline "Pipeline")
