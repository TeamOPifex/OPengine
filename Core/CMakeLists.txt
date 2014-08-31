#########################################
# CORE - LAYER 1
#########################################
cmake_minimum_required(VERSION 2.8)


#########################################
# Include Directories
#########################################
include_directories(
	.
	../
	./Core/include/
)


#########################################
# Include Files
#########################################
add_source_group(\\ "${PROJECT_SOURCE_DIR}/Core" CORE_SRC)
add_source_group(include/ "${PROJECT_SOURCE_DIR}/Core/include" CORE_SRC)


#########################################
# Source Files
#########################################
add_source_group(src/ "${PROJECT_SOURCE_DIR}/Core/src" CORE_SRC)

	
#########################################
# BUILD
#########################################
add_library(1_Core STATIC ${CORE_SRC})
add_definitions(-D${OPIFEX_OS})

if(${OPIFEX_RELEASE})

else()
	add_definitions(-D_DEBUG)
endif()
if(${OPIFEX_RELEASE})
	add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)
endif()
set_target_properties(1_Core PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(1_Core PROPERTIES OUTPUT_NAME "Core")

if( ${OPIFEX_OS_WINDOWS} )
	target_link_libraries(1_Core Winmm.lib)
endif()	

output_library(1_Core "Core")
