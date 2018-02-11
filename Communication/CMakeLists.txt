#########################################
# Communication - LAYER 6
#########################################
cmake_minimum_required(VERSION 2.8)


#########################################
# Include Directories
#########################################
add_opifex_includes()


#########################################
# Source Files
#########################################
add_source_group(include/ "${PROJECT_SOURCE_DIR}/Communication/include" COMMUNICATION_SRC)

add_source_group(/ "${PROJECT_SOURCE_DIR}/Communication" COMMUNICATION_SRC)
add_source_group(src/ "${PROJECT_SOURCE_DIR}/Communication/src" COMMUNICATION_SRC)
add_source_group(src/ "${PROJECT_SOURCE_DIR}/Communication/src/Enums" COMMUNICATION_SRC)


#########################################
# DEPENDENT LIBRARIES
#########################################
set(ALL_LIBS
	${OPENGL_LIBRARY}
	1_Core
	2_Data
	3_Math
	4_Performance
	5_Human
)

#########################################
# BUILD
#########################################
if(${OPIFEX_OPTION_SHARED})
	add_library(6_Communication SHARED ${COMMUNICATION_SRC})
else()
	add_library(6_Communication STATIC ${COMMUNICATION_SRC})
endif()

if(${OPIFEX_OS_IOS})
	set_xcode_property(6_Communication GCC_GENERATE_DEBUGGING_SYMBOLS YES)
endif()
add_opifex_defines()

set_target_properties(6_Communication PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(6_Communication PROPERTIES OUTPUT_NAME "Communication")

if(${OPIFEX_OPTION_RELEASE})
	add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)
endif()
target_link_libraries(6_Communication ${ALL_LIBS})

add_opifex_oggvorbis(6_Communication)

#########################################
# OUTPUT BINARIES
#########################################
output_library(6_Communication "Communication")
