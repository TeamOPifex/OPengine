cmake_minimum_required(VERSION 2.8)
project(OPifexEngine)

macro(add_source_group FILTER_NAME SOURCE_PATH TARGET_LIST)
	
	file(TO_NATIVE_PATH ${FILTER_NAME} NEW_FILTER_NAME)
	
	file(GLOB TEMP_SRC
		"${SOURCE_PATH}/*.h"
		"${SOURCE_PATH}/*.cpp"
		"${SOURCE_PATH}/*.mm"
	)
	source_group("${NEW_FILTER_NAME}" FILES ${TEMP_SRC})
	
	list(APPEND ${TARGET_LIST} "${TEMP_SRC}")

endmacro(add_source_group)

#########################################
# Core - Level 1
#########################################

set(CORE_LIBRARY_PATH "${PROJECT_SOURCE_DIR}/Core")
add_subdirectory("${CORE_LIBRARY_PATH}" ${PROJECT_BINARY_DIR}/Core)


#########################################
# Data - Level 2
#########################################

set(DATA_LIBRARY_PATH "${PROJECT_SOURCE_DIR}/Data")
add_subdirectory("${DATA_LIBRARY_PATH}" ${PROJECT_BINARY_DIR}/Data)


#########################################
# Human - Level 3
#########################################

set(HUMAN_LIBRARY_PATH "${PROJECT_SOURCE_DIR}/Human")
add_subdirectory("${HUMAN_LIBRARY_PATH}" ${PROJECT_BINARY_DIR}/Human)


#########################################
# Performance - Level 4
#########################################

set(PERFORMANCE_LIBRARY_PATH "${PROJECT_SOURCE_DIR}/Performance")
add_subdirectory("${PERFORMANCE_LIBRARY_PATH}" ${PROJECT_BINARY_DIR}/Performance)


#########################################
# Communication - Level 5
#########################################

set(COMMUNICATION_LIBRARY_PATH "${PROJECT_SOURCE_DIR}/Communication")
add_subdirectory("${COMMUNICATION_LIBRARY_PATH}" ${PROJECT_BINARY_DIR}/Communication)


#########################################
# Scripting - Level 6
#########################################

set(SCRIPTING_LIBRARY_PATH "${PROJECT_SOURCE_DIR}/Scripting")
add_subdirectory("${SCRIPTING_LIBRARY_PATH}" ${PROJECT_BINARY_DIR}/Scripting)


#########################################
# Application - Level 7
#########################################

set(APPLICATION_LIBRARY_PATH "${PROJECT_SOURCE_DIR}/Application")
add_subdirectory("${APPLICATION_LIBRARY_PATH}" ${PROJECT_BINARY_DIR}/Application)

