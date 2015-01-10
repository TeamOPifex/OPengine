#########################################
# Communication - LAYER 6
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
	../External/Mongoose/include/
	.
	../
)

#########################################
# Include Files
#########################################
add_source_group(include/ "${PROJECT_SOURCE_DIR}/Communication/include" COMMUNICATION_SRC)


#########################################
# Source Files
#########################################
add_source_group(\\ "${PROJECT_SOURCE_DIR}/Communication" COMMUNICATION_SRC)
add_source_group(src/ "${PROJECT_SOURCE_DIR}/Communication/src" COMMUNICATION_SRC)


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
	${OGG_LIBRARY}
	${VORBIS_LIBRARY}
	${VORBISFILE_LIBRARY}
	glfw 
	${GLFW_LIBRARIES}
	MONGOOSE
)

#########################################
# BUILD
#########################################
if(${OPIFEX_OPTION_SHARED})
	add_library(6_Communication SHARED ${COMMUNICATION_SRC})
	add_definitions(-DOPIFEX_SHARED)
else()
	add_library(6_Communication STATIC ${COMMUNICATION_SRC})
endif()

set_target_properties(6_Communication PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(6_Communication PROPERTIES OUTPUT_NAME "Communication")
add_definitions(-D${OPIFEX_OS})

if(${OPIFEX_RELEASE})
	add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)
endif()
target_link_libraries(6_Communication ${ALL_LIBS})

#########################################
# OUTPUT BINARIES
#########################################
output_library(6_Communication "Communication")