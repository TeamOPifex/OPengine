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

add_external_opifex_includes()

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
if(${OPIFEX_OPTION_SHARED})
	add_library(4_Performance SHARED ${PERFORMANCE_SRC})
else()
	add_library(4_Performance STATIC ${PERFORMANCE_SRC})
endif()

if(${OPIFEX_OS_IOS})
	set_xcode_property(4_Performance GCC_GENERATE_DEBUGGING_SYMBOLS YES)
endif()
add_opifex_defines()
add_definitions(-DGLEW_STATIC)


if(${OPIFEX_OPTION_RELEASE})
	add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)
endif()

target_link_libraries(4_Performance	${ALL_LIBS})

set_target_properties(4_Performance PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(4_Performance PROPERTIES OUTPUT_NAME "Performance")


#########################################
# ADDITIONAL FEATURES
#########################################
add_opifex_physx(4_Performance)

output_library(4_Performance "Performance")
