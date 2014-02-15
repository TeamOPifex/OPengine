#########################################
# MATH - LAYER 3
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
add_source_group(include/ "${PROJECT_SOURCE_DIR}/Math/include" MATH_SRC)


#########################################
# Source Files
#########################################
add_source_group(src/ "${PROJECT_SOURCE_DIR}/Math/src" MATH_SRC)


#########################################
# DEPENDENT LIBRARIES
#########################################
set(ALL_LIBS
	1_Core
	2_Data
)


#########################################
# BUILD
#########################################
add_library(3_Math STATIC ${MATH_SRC})
add_definitions(-D_CRT_SECURE_NO_WARNINGS)
add_definitions(-D${OPIFEX_OS})
	
target_link_libraries(3_Math ${ALL_LIBS})

set_target_properties(3_Math PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(3_Math PROPERTIES OUTPUT_NAME "Math")

output_library(3_Math "Math")