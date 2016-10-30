cmake_minimum_required(VERSION 2.8)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(OPifexEngine)


SET(OPIFEX_ENGINE_REPOSITORY ${PROJECT_SOURCE_DIR})

include(CMake/application.cmake)

add_marketplace_addons_projects()

string(REPLACE "\\" "/" _PHYSX_PATH "${PHYSX_PATH}")
string(REPLACE "\\" "/" _V8_PATH "${V8_PATH}")
string(REPLACE "\\" "/" _FMOD_PATH "${FMOD_PATH}")
#configure_file("OPengineDefines.h.in" "${BINARY_FOLDER}/OPengineDefines.h")
configure_file("OPengineDefines.cmake.in" "${BINARY_FOLDER}/OPengineDefines.cmake")

### Project Setup
#########################################
# External
#########################################
add_subdirectory (External)


#########################################
# Tools
#########################################
add_subdirectory (Tools)


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
# Math - Level 3
#########################################

set(MATH_LIBRARY_PATH "${PROJECT_SOURCE_DIR}/Math")
add_subdirectory("${MATH_LIBRARY_PATH}" ${PROJECT_BINARY_DIR}/Math)


#########################################
# Performance - Level 4
#########################################

set(PERFORMANCE_LIBRARY_PATH "${PROJECT_SOURCE_DIR}/Performance")
add_subdirectory("${PERFORMANCE_LIBRARY_PATH}" ${PROJECT_BINARY_DIR}/Performance)


#########################################
# Human - Level 5
#########################################

set(HUMAN_LIBRARY_PATH "${PROJECT_SOURCE_DIR}/Human")
add_subdirectory("${HUMAN_LIBRARY_PATH}" ${PROJECT_BINARY_DIR}/Human)


#########################################
# Communication - Level 6
#########################################

set(COMMUNICATION_LIBRARY_PATH "${PROJECT_SOURCE_DIR}/Communication")
add_subdirectory("${COMMUNICATION_LIBRARY_PATH}" ${PROJECT_BINARY_DIR}/Communication)


#########################################
# Scripting - Level 7
#########################################

set(SCRIPTING_LIBRARY_PATH "${PROJECT_SOURCE_DIR}/Scripting")
add_subdirectory("${SCRIPTING_LIBRARY_PATH}" ${PROJECT_BINARY_DIR}/Scripting)


#########################################
# Pipeline - Level 8
#########################################

set(PIPELINE_LIBRARY_PATH "${PROJECT_SOURCE_DIR}/Pipeline")
add_subdirectory("${PIPELINE_LIBRARY_PATH}" ${PROJECT_BINARY_DIR}/Pipeline)


#########################################
# Application - Level 9
#########################################
set(APPLICATION_LIBRARY_PATH "${PROJECT_SOURCE_DIR}/Application")
add_subdirectory("${APPLICATION_LIBRARY_PATH}" ${PROJECT_BINARY_DIR}/Application)


#########################################
# Editor - Level 9
#########################################
# Will come back to this later
set(EDITOR_LIBRARY_PATH "${PROJECT_SOURCE_DIR}/Editor")
add_subdirectory("${EDITOR_LIBRARY_PATH}" ${PROJECT_BINARY_DIR}/Editor)


#########################################
# Tests - Level
#########################################
if(OPIFEX_OS_IOS OR OPIFEX_OS_ANDROID)
else()
	set(TESTS_PATH "${PROJECT_SOURCE_DIR}/Tests")
	add_subdirectory("${TESTS_PATH}" ${PROJECT_BINARY_DIR}/Tests)
endif()

### Logging
# message(STATUS "OPIFEX_OS: ${OPIFEX_OS}")
# message(STATUS "RENDER_TARGET: ${RENDER_TARGET}")
# message(STATUS "Ogg Library: ${OGG_LIBRARY}")
# message(STATUS "Vorbis Library: ${VORBIS_LIBRARY}")
# message(STATUS "VorbisFile Library: ${VORBISFILE_LIBRARY}")
# message(STATUS "OpenGL (${OpenGL})")
# message(STATUS "GLFW_LIBRARIES (${GLFW_LIBRARIES})")

opifex_engine_status_messages()

# message(STATUS "Variable: ${CMAKE_SOURCE_DIR}")




# To Build for iOS
# cmake ../OPengine -DCMAKE_TOOLCHAIN_FILE=./CMake/engine/toolchains/iOS.cmake -DIOS_PLATFORM=SIMULATOR -DOPIFEX_OS=OPIFEX_IOS -DRENDER_TARGET=OPENGL_ES_2
