
### Sets up variables and defaults for the build


# If the CMAKE option for OPIFEX_BINARIES has been set
if(OPIFEX_BINARIES)
	set_binary_output(OPIFEX_BINARIES_ABS)
	#get_filename_component(OPIFEX_BINARIES_ABS ${OPIFEX_BINARIES_ABS} REALPATH)
	if(NOT ${OPIFEX_ANDROID_SETUP})
		include(${OPIFEX_BINARIES_ABS}/OPengineDefines.cmake)
	endif()
endif()


### Setup the architecture

include(${CMAKE_CURRENT_LIST_DIR}/architecture.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/architecture/default_os.cmake)


### Setup default options

set(OPIFEX_OS ${OPIFEX_DEFAULT_OS} CACHE STRING "Operating system to build for.")
set(RENDER_TARGET "OPENGL_3_3" CACHE STRING "The OpenGL Version to Target")
set(OPIFEX_NODE_VERSION "0_10" CACHE STRING "The NodeJS Version to Build for")

set_property(CACHE OPIFEX_OS PROPERTY STRINGS OPIFEX_WIN32 OPIFEX_WIN64 OPIFEX_LINUX32 OPIFEX_LINUX64 OPIFEX_IOS OPIFEX_ANDROID OPIFEX_ANDROID32 OPIFEX_ANDROID64 OPIFEX_OSX32 OPIFEX_OSX64)
set_property(CACHE RENDER_TARGET PROPERTY STRINGS OPENGL_2_0 OPENGL_3_3 OPENGL_ES_2 DIRECTX_11)
set_property(CACHE OPIFEX_NODE_VERSION PROPERTY STRINGS 0_10 0_12)

SET(OPIFEX_PROJECT_BINARY_DIR ${PROJECT_BINARY_DIR})
SET(OPIFEX_PROJECT_SOURCE_DIR ${PROJECT_SOURCE_DIR})


### Setup helper variables

SET(OPIFEX_OS_WIN32 "${OPIFEX_OS}" STREQUAL "OPIFEX_WIN32")
SET(OPIFEX_OS_WIN64 "${OPIFEX_OS}" STREQUAL "OPIFEX_WIN64")
SET(OPIFEX_OS_OSX32 "${OPIFEX_OS}" STREQUAL "OPIFEX_OSX32")
SET(OPIFEX_OS_OSX64 "${OPIFEX_OS}" STREQUAL "OPIFEX_OSX64")
SET(OPIFEX_OS_LINUX32 "${OPIFEX_OS}" STREQUAL "OPIFEX_LINUX32")
SET(OPIFEX_OS_LINUX64 "${OPIFEX_OS}" STREQUAL "OPIFEX_LINUX64")
SET(OPIFEX_OS_ANDROID "${OPIFEX_OS}" STREQUAL "OPIFEX_ANDROID")
SET(OPIFEX_OS_ANDROID32 "${OPIFEX_OS}" STREQUAL "OPIFEX_ANDROID32")
SET(OPIFEX_OS_ANDROID64 "${OPIFEX_OS}" STREQUAL "OPIFEX_ANDROID64")
SET(OPIFEX_OS_IOS "${OPIFEX_OS}" STREQUAL "OPIFEX_IOS")

if(${OPIFEX_OS_ANDROID32} OR ${OPIFEX_OS_ANDROID32})
		SET(OPIFEX_OS_ANDROID 1)
endif()

if(${OPIFEX_OS_ANDROID})
	if("${ANDROID_ABI}" STREQUAL "x86_64")
		SET(OPIFEX_OS_ANDROID64 1)
	else()
		SET(OPIFEX_OS_ANDROID32 1)
	endif()
	SET(OPIFEX_OS "OPIFEX_ANDROID64")
endif()

OPIFEX_EVAL(OPIFEX_OS_WINDOWS ${OPIFEX_OS_WIN32} OR ${OPIFEX_OS_WIN64})
OPIFEX_EVAL(OPIFEX_OS_UNIX ${OPIFEX_OS_OSX32} OR ${OPIFEX_OS_OSX64} OR ${OPIFEX_OS_LINUX32} OR ${OPIFEX_OS_LINUX64})
OPIFEX_EVAL(OPIFEX_OS_LINUX ${OPIFEX_OS_LINUX32} OR ${OPIFEX_OS_LINUX64})
OPIFEX_EVAL(OPIFEX_OS_OSX ${OPIFEX_OS_OSX32} OR ${OPIFEX_OS_OSX64})

SET(OPIFEX_OS_32 ${OPIFEX_OS_WIN32} OR ${OPIFEX_OS_OSX32} OR ${OPIFEX_OS_LINUX32} OR ${OPIFEX_OS_ANDROID32})
SET(OPIFEX_OS_64 ${OPIFEX_OS_WIN64} OR ${OPIFEX_OS_OSX64} OR ${OPIFEX_OS_LINUX64} OR ${OPIFEX_OS_ANDROID64})


OPIFEX_EVAL(OPIFEX_OS_MOBILE ${OPIFEX_OS_ANDROID} OR ${OPIFEX_OS_IOS})
OPIFEX_EVAL(OPIFEX_OS_DESKTOP ${OPIFEX_OS_WINDOWS} OR ${OPIFEX_OS_UNIX})

# Make sure the XInput library is linked correctly (Regardless of which Visual Studio is used)
# Windows 7 - http://en.wikipedia.org/wiki/Windows_NT#Releases
# MSVC10 - Visual Studio 2010
if( ${CMAKE_SYSTEM} STREQUAL "Windows-6.1" )
	if(${MSVC_VERSION} GREATER 1600)
		SET(MSVC_WIN7_FIX 1)
	endif()
endif()


if(${OPIFEX_OS_DESKTOP})
	if(MSVC)
		set(CMAKE_C_FLAGS_DEBUG "/D_DEBUG /MTd /Zi /Ob0 /Od /RTC1" CACHE STRING "" FORCE)
		set(CMAKE_C_FLAGS_RELEASE "/MT /O2 /Ob2 /D" CACHE STRING "" FORCE)

		set(CMAKE_CXX_FLAGS_DEBUG "/D_DEBUG /MTd /Zi /Ob0 /Od /RTC1" CACHE STRING "" FORCE)
		set(CMAKE_CXX_FLAGS_RELEASE "/MT /O2 /Ob2 /D" CACHE STRING "" FORCE)
	else()
		set(CMAKE_CXX_FLAGS "-stdlib=libstdc++ -std=c++11 -O3" CACHE STRING "" FORCE)
	endif()
endif()


if(${OPIFEX_OS_ANDROID})
	message(STATUS "ANDROID ABI::: ${ANDROID_ABI}")
	if("${ANDROID_ABI}" STREQUAL "armeabi-v7a")
		include_directories(${ANDROID_SYSROOT}/usr/include/arm-linux-androideabi)
	elseif("${ANDROID_ABI}" STREQUAL "arm64-v8a")
		include_directories(${ANDROID_SYSROOT}/usr/include/aarch64-linux-android)
	elseif("${ANDROID_ABI}" STREQUAL "x86_64")
		include_directories(${ANDROID_SYSROOT}/usr/include/x86_64-linux-android)
	else()
		include_directories(${ANDROID_SYSROOT}/usr/include/arm-linux-androideabi)
	endif()

	if(NOT ${OPIFEX_ANDROID_SETUP})
		SET(OPIFEX_PROJECT_BINARY_DIR ${OPIFEX_ANDROID_PROJECT_BINARY_DIR})
	endif()
endif()