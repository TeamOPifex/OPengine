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
	.
	../
)

#########################################
# Include Files
#########################################
add_source_group(include/ "${PROJECT_SOURCE_DIR}/Scripting/include" SCRIPTING_SRC)


#########################################
# Source Files
#########################################
add_source_group(\\ "${PROJECT_SOURCE_DIR}/Scripting" SCRIPTING_SRC)
add_source_group(src/ "${PROJECT_SOURCE_DIR}/Scripting/src" SCRIPTING_SRC)


#########################################
# DEPENDENT LIBRARIES
#########################################


#########################################
# BUILD
#########################################
add_library(7_Scripting STATIC ${SCRIPTING_SRC})
set_target_properties(7_Scripting PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(7_Scripting PROPERTIES OUTPUT_NAME "Scripting")

if( ${OPIFEX_V8} )
	target_link_libraries(7_Scripting 
		${PROJECT_SOURCE_DIR}/External/V8/lib/win64/icui18n.lib
		${PROJECT_SOURCE_DIR}/External/V8/lib/win64/icuuc.lib
		${PROJECT_SOURCE_DIR}/External/V8/lib/win64/v8_base.x64.lib
		${PROJECT_SOURCE_DIR}/External/V8/lib/win64/v8_snapshot.lib
		ws2_32.lib
		advapi32.lib
		winmm.lib)
	add_definitions(-DOPIFEX_V8)
endif()