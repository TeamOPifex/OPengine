#########################################
# PERFORMANCE - LAYER 4
#########################################
cmake_minimum_required(VERSION 2.8)


#########################################
# Include Directories
#########################################
include_directories(
	.
	../
)


#########################################
# Include Files
#########################################
add_source_group(include/ "${PROJECT_SOURCE_DIR}/Performance/include" PERFORMANCE_SRC)


#########################################
# Source Files
#########################################
add_source_group(src/ "${PROJECT_SOURCE_DIR}/Performance/src" PERFORMANCE_SRC)


#########################################
# DEPENDENT LIBRARIES
#########################################
set(ALL_LIBS
	1_Core
	2_Data
	3_Math
)


#########################################
# BUILD
#########################################
add_library(4_Performance STATIC ${PERFORMANCE_SRC})
add_definitions(-D_CRT_SECURE_NO_WARNINGS)
add_definitions(-D${OPIFEX_OS})

if(${OPIFEX_RELEASE})
	add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)
endif()
if( ${OPIFEX_OS_ANDROID} )
	add_definitions(-DOPIFEX_OPENGL_ES)
	add_definitions(-DOPIFEX_OPENGL_ES_2)
endif()

target_link_libraries(4_Performance	${ALL_LIBS})

set_target_properties(4_Performance PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(4_Performance PROPERTIES OUTPUT_NAME "Performance")


#########################################
# ADDITIONAL FEATURES
#########################################
if(${OPIFEX_PHYSICS}) 
	add_definitions(-DOPIFEX_PHYSICS)
	include_directories(../External/PhysX/include/)
	target_link_libraries(4_Performance ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win32/PhysX3DEBUG_x86.lib)
	target_link_libraries(4_Performance ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win32/PhysX3CommonDEBUG_x86.lib)
	target_link_libraries(4_Performance ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win32/PhysX3ExtensionsDEBUG.lib)
	target_link_libraries(4_Performance ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win32/PxTaskDEBUG.lib)
endif()

output_library(4_Performance "Performance")