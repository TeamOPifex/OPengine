#########################################
# DATA - LAYER 2
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
add_source_group(\\ "${PROJECT_SOURCE_DIR}/Data" DATA_SRC)
add_source_group(include "${PROJECT_SOURCE_DIR}/Data/include" DATA_SRC)


#########################################
# Source Files
#########################################
add_source_group(src "${PROJECT_SOURCE_DIR}/Data/src" DATA_SRC)


#########################################
# DEPENDENT LIBRARIES
#########################################
set(ALL_LIBS
	1_Core
)


#########################################
# BUILD
#########################################
add_library(2_Data STATIC ${DATA_SRC})
add_definitions(-D${OPIFEX_OS})

if(${OPIFEX_RELEASE})
	add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)
else()
	add_definitions(-D_DEBUG)
	if(${OPIFEX_OS_WINDOWS})
		SET(OPIFEX_REPO "../${OPIFEX_REPOSITORY}/Assets/")
		add_definitions(-DOPIFEX_REPO="${OPIFEX_REPO}")
	ELSE()
		SET(OPIFEX_REPO "${OPIFEX_REPOSITORY}/Assets/")
		add_definitions(-DOPIFEX_REPO="${OPIFEX_REPO}")
	ENDIF()
	
endif()

target_link_libraries(2_Data ${ALL_LIBS})

set_target_properties(2_Data PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(2_Data PROPERTIES OUTPUT_NAME "Data")

output_library(2_Data "Data")