cmake_minimum_required(VERSION 2.8)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
project(OPifexEngine)

# c_flag_overrides.cmake
    if(MSVC)
        set(CMAKE_C_FLAGS_DEBUG_INIT "/D_DEBUG /MTd /Zi /Ob0 /Od /RTC1")
        set(CMAKE_C_FLAGS_MINSIZEREL_INIT     "/MT /O1 /Ob1 /D NDEBUG")
        set(CMAKE_C_FLAGS_RELEASE_INIT        "/MT /O2 /Ob2 /D NDEBUG")
        set(CMAKE_C_FLAGS_RELWITHDEBINFO_INIT "/MT /Zi /O2 /Ob1 /D NDEBUG")
    endif()
# cxx_flag_overrides.cmake
    if(MSVC)
        set(CMAKE_CXX_FLAGS_DEBUG_INIT "/D_DEBUG /MTd /Zi /Ob0 /Od /RTC1")
        set(CMAKE_CXX_FLAGS_MINSIZEREL_INIT     "/MT /O1 /Ob1 /D NDEBUG")
        set(CMAKE_CXX_FLAGS_RELEASE_INIT        "/MT /O2 /Ob2 /D NDEBUG")
        set(CMAKE_CXX_FLAGS_RELWITHDEBINFO_INIT "/MT /Zi /O2 /Ob1 /D NDEBUG")
    endif()
	

### CMake Setup
include(CMake/macros.cmake) 
### Build Setup
include(CMake/build.cmake)
include(CMake/global.cmake)
include(CMake/packages.cmake)
include(CMake/assets.cmake)
include(CMake/binaries.cmake)


### Project Setup
#########################################
# External
#########################################
add_subdirectory (External)


#########################################
# Tools
#########################################
if( ${OPIFEX_OS_WINDOWS} )
	add_subdirectory (Tools)
endif()


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
# Tests - Level
#########################################
##set(TESTS_PATH "${PROJECT_SOURCE_DIR}/Tests")
##add_subdirectory("${TESTS_PATH}" ${PROJECT_BINARY_DIR}/Tests)

### Logging
message(STATUS "OPIFEX_OS: ${OPIFEX_OS}")
message(STATUS "OPENGL_DESKTOP_TARGET: ${OPENGL_DESKTOP_TARGET}")
message(STATUS "Ogg Library: ${OGG_LIBRARY}")
message(STATUS "Vorbis Library: ${VORBIS_LIBRARY}")
message(STATUS "VorbisFile Library: ${VORBISFILE_LIBRARY}")
message(STATUS "OpenGL (${OpenGL})")
message(STATUS "GLFW_LIBRARIES (${GLFW_LIBRARIES})")