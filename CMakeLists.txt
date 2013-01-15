cmake_minimum_required(VERSION 2.8)
project(OPifexEngine)

macro(add_source_group FILTER_NAME SOURCE_PATH TARGET_LIST)

	file(TO_NATIVE_PATH ${FILTER_NAME} NEW_FILTER_NAME)
	file(GLOB TEMP_SRC
		"${SOURCE_PATH}/*.h"
		"${SOURCE_PATH}/*.cpp"
		"${SOURCE_PATH}/*.c"
		"${SOURCE_PATH}/*.mm"
	)
	source_group("${NEW_FILTER_NAME}" FILES ${TEMP_SRC})
	list(APPEND ${TARGET_LIST} "${TEMP_SRC}")

endmacro(add_source_group)

set(OPIFEX_OS "OPIFEX_LINUX32" CACHE STRING
  "Operating system to build for.")

set_property(CACHE OPIFEX_OS PROPERTY STRINGS OPIFEX_WIN32 OPIFEX_WIN64 OPIFEX_LINUX32 OPIFEX_LINUX64 OPIFEX_IOS OPIFEX_ANDROID)
 
if( "${OPIFEX_OS}" STREQUAL "OPIFEX_ANDROID" )
	
	file(COPY ${PROJECT_SOURCE_DIR}/Android/Make/AndroidManifest.xml DESTINATION ${PROJECT_BINARY_DIR}/)
	file(COPY ${PROJECT_SOURCE_DIR}/Android/Make/build.xml DESTINATION ${PROJECT_BINARY_DIR}/)
	file(COPY ${PROJECT_SOURCE_DIR}/Android/Make/Install.bat DESTINATION ${PROJECT_BINARY_DIR}/)
	file(COPY ${PROJECT_SOURCE_DIR}/Android/Java/ControllerState.java DESTINATION ${PROJECT_BINARY_DIR}/src/com/opifex/SMRF/)
	file(COPY ${PROJECT_SOURCE_DIR}/Android/Java/GL2JNILib.java DESTINATION ${PROJECT_BINARY_DIR}/src/com/opifex/SMRF/)
	file(COPY ${PROJECT_SOURCE_DIR}/Android/Java/GL2JNIView.java DESTINATION ${PROJECT_BINARY_DIR}/src/com/opifex/SMRF/)
	file(COPY ${PROJECT_SOURCE_DIR}/Android/Java/SMRF.java DESTINATION ${PROJECT_BINARY_DIR}/src/com/opifex/SMRF/)
	
	file(COPY ${PROJECT_SOURCE_DIR}/Android/Assets/steamPlane.obj DESTINATION ${PROJECT_BINARY_DIR}/assets/)
	file(COPY ${PROJECT_SOURCE_DIR}/Android/Assets/bricks.dds DESTINATION ${PROJECT_BINARY_DIR}/assets/)
	file(COPY ${PROJECT_SOURCE_DIR}/Android/Assets/steamPlaneSkin.dds DESTINATION ${PROJECT_BINARY_DIR}/assets/)
	file(COPY ${PROJECT_SOURCE_DIR}/Android/Assets/AMemoryAway.ogg DESTINATION ${PROJECT_BINARY_DIR}/assets/)
	
	file(COPY ${PROJECT_SOURCE_DIR}/Android/Libs/ouya-sdk.jar DESTINATION ${PROJECT_BINARY_DIR}/libs/)
	
	configure_file(		${PROJECT_SOURCE_DIR}/Android/Make/Android.mk		${PROJECT_BINARY_DIR}/jni/Android.mk	)
	configure_file(		${PROJECT_SOURCE_DIR}/Android/Make/strings.xml		${PROJECT_BINARY_DIR}/res/values/strings.xml	)
		
	configure_file (
	  "${PROJECT_SOURCE_DIR}/Core/Target.h.in"
	  "${PROJECT_SOURCE_DIR}/Core/Target.h"
	)
	
endif()


find_package(OpenGL REQUIRED)

add_subdirectory (External)

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
	# Performance - Level 3
	#########################################

	set(PERFORMANCE_LIBRARY_PATH "${PROJECT_SOURCE_DIR}/Performance")
	add_subdirectory("${PERFORMANCE_LIBRARY_PATH}" ${PROJECT_BINARY_DIR}/Performance)
	

	#########################################
	# Human - Level 4
	#########################################

	set(HUMAN_LIBRARY_PATH "${PROJECT_SOURCE_DIR}/Human")
	add_subdirectory("${HUMAN_LIBRARY_PATH}" ${PROJECT_BINARY_DIR}/Human)
	

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


