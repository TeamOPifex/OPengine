#########################################
# SCRIPTING - LAYER 7
#########################################
cmake_minimum_required(VERSION 2.8)


#########################################
# Include Directories
#########################################
include_directories(
	.
	../
	../External/Node/src/
)
add_external_opifex_includes()

#########################################
# Include Files
#########################################
add_source_group(include/ "${PROJECT_SOURCE_DIR}/Scripting/include" SCRIPTING_SRC)
add_source_group(include/V8 "${PROJECT_SOURCE_DIR}/Scripting/include/V8" SCRIPTING_SRC)
add_source_group(include/V8/WrapperMath "${PROJECT_SOURCE_DIR}/Scripting/include/V8/WrapperMath" SCRIPTING_SRC)
add_source_group(include/JavaScript "${PROJECT_SOURCE_DIR}/Scripting/include/JavaScript" SCRIPTING_SRC)
add_source_group(include/JavaScript/Communication "${PROJECT_SOURCE_DIR}/Scripting/include/JavaScript/Communication" SCRIPTING_SRC)
add_source_group(include/JavaScript/Core "${PROJECT_SOURCE_DIR}/Scripting/include/JavaScript/Core" SCRIPTING_SRC)
add_source_group(include/JavaScript/Data "${PROJECT_SOURCE_DIR}/Scripting/include/JavaScript/Data" SCRIPTING_SRC)
add_source_group(include/JavaScript/Human "${PROJECT_SOURCE_DIR}/Scripting/include/JavaScript/Human" SCRIPTING_SRC)
add_source_group(include/JavaScript/Math "${PROJECT_SOURCE_DIR}/Scripting/include/JavaScript/Math" SCRIPTING_SRC)
add_source_group(include/JavaScript/Performance "${PROJECT_SOURCE_DIR}/Scripting/include/JavaScript/Performance" SCRIPTING_SRC)
add_source_group(include/JavaScript/Pipeline "${PROJECT_SOURCE_DIR}/Scripting/include/JavaScript/Pipeline" SCRIPTING_SRC)


#########################################
# Source Files
#########################################
add_source_group(/ "${PROJECT_SOURCE_DIR}/Scripting" SCRIPTING_SRC)
add_source_group(src/ "${PROJECT_SOURCE_DIR}/Scripting/src" SCRIPTING_SRC)
add_source_group(src/V8 "${PROJECT_SOURCE_DIR}/Scripting/src/V8" SCRIPTING_SRC)
add_source_group(src/V8/WrapperMath "${PROJECT_SOURCE_DIR}/Scripting/src/V8/WrapperMath" SCRIPTING_SRC)
add_source_group(src/JavaScript "${PROJECT_SOURCE_DIR}/Scripting/src/JavaScript" SCRIPTING_SRC)
add_source_group(src/JavaScript/Communication "${PROJECT_SOURCE_DIR}/Scripting/src/JavaScript/Communication" SCRIPTING_SRC)
add_source_group(src/JavaScript/Core "${PROJECT_SOURCE_DIR}/Scripting/src/JavaScript/Core" SCRIPTING_SRC)
add_source_group(src/JavaScript/Data "${PROJECT_SOURCE_DIR}/Scripting/src/JavaScript/Data" SCRIPTING_SRC)
add_source_group(src/JavaScript/Human "${PROJECT_SOURCE_DIR}/Scripting/src/JavaScript/Human" SCRIPTING_SRC)
add_source_group(src/JavaScript/Math "${PROJECT_SOURCE_DIR}/Scripting/src/JavaScript/Math" SCRIPTING_SRC)
add_source_group(src/JavaScript/Performance "${PROJECT_SOURCE_DIR}/Scripting/src/JavaScript/Performance" SCRIPTING_SRC)
add_source_group(src/JavaScript/Pipeline "${PROJECT_SOURCE_DIR}/Scripting/src/JavaScript/Pipeline" SCRIPTING_SRC)


#########################################
# DEPENDENT LIBRARIES
#########################################
set(ALL_LIBS
	${OPENGL_LIBRARY}
	${OPENAL_LIBRARY}
	#GLEW_158
	LODEPNG
	1_Core
	2_Data
	3_Math
	4_Performance
	5_Human
	7_Pipeline
	#glfw ${GLFW_LIBRARIES}
)

#########################################
# BUILD
#########################################
if(${OPIFEX_OPTION_SHARED})
	add_library(8_Scripting SHARED ${SCRIPTING_SRC})
else()
	add_library(8_Scripting STATIC ${SCRIPTING_SRC})
endif()
set_xcode_property(8_Scripting GCC_GENERATE_DEBUGGING_SYMBOLS YES)
add_opifex_defines()
add_definitions(-DGLEW_STATIC)

target_link_libraries(8_Scripting ${ALL_LIBS})
set_target_properties(8_Scripting PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(8_Scripting PROPERTIES OUTPUT_NAME "Scripting")

if(${OPIFEX_OPTION_RELEASE})
	add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)
endif()

add_opifex_v8(8_Scripting)

#########################################
# OUTPUT BINARIES
#########################################
output_library(8_Scripting "Scripting")

if(${OPIFEX_OPTION_V8})
	if(${OPIFEX_OPTION_RELEASE})
		output_binary(8_Scripting "/External/V8/lib/release/" "icui18n.dll" ${OPIFEX_OS_WINDOWS})
		output_binary(8_Scripting "/External/V8/lib/release/" "icuuc.dll" ${OPIFEX_OS_WINDOWS})
		output_binary(8_Scripting "/External/V8/lib/release/" "v8.dll" ${OPIFEX_OS_WINDOWS})
	else()
		#output_binary(8_Scripting "/External/V8/lib/debug/" "icui18n.dll" ${OPIFEX_OS_WINDOWS})
		#output_binary(8_Scripting "/External/V8/lib/debug/" "icuuc.dll" ${OPIFEX_OS_WINDOWS})
		#output_binary(8_Scripting "/External/V8/lib/debug/" "v8.dll" ${OPIFEX_OS_WINDOWS})
		#output_binary(8_Scripting "/External/V8/lib/debug/" "v8.lib" ${OPIFEX_OS_WINDOWS})
		#output_binary(8_Scripting "/External/V8/lib/debug/" "v8_libbase.lib" ${OPIFEX_OS_WINDOWS})
		#output_binary(8_Scripting "/External/V8/lib/debug/" "v8_libplatform.lib" ${OPIFEX_OS_WINDOWS})
	endif()
endif()
