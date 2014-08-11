#########################################
# APPLICATION - LAYER 9
#########################################
cmake_minimum_required(VERSION 2.8)


#########################################
# Include Directories
#########################################
include_directories(
	../External/glfw-3.0.4/include/
	../External/glm-0.9.1/
	../External/glew-1.9.0/include/
	../External/Phsyx.3.2.1/include/
	../External/GLES2/
	../External/Ogg/include/
	../External/Vorbis/include/
	../External/OpenAL/
	../External/Oculus/include/
	../External/Oculus/Src/
	../External/Myo/include/
	../External/V8/include/
	../External/V8/
	../External/VP8/include/
	.
	../
)


#########################################
# Include Files
#########################################


#########################################
# Source Files
#########################################
add_source_group(\\ "${PROJECT_SOURCE_DIR}/Application" APPLICATION_SRC)
add_source_group(Examples "${PROJECT_SOURCE_DIR}/Application/Examples" APPLICATION_SRC)


#########################################
# DEPENDENT LIBRARIES
#########################################
set(ALL_LIBS
	${OPENGL_LIBRARY}
	${OPENAL_LIBRARY}
	1_Core
	2_Data
	3_Math
	4_Performance
	5_Human
	6_Communication
	7_Scripting
	8_Pipeline
	${OGG_LIBRARY}
	${VORBIS_LIBRARY}
	${VORBISFILE_LIBRARY}
	${ZLIB_LIBRARY}
	${LIBPNG_LIBRARY}
	${CMAKE_THREAD_LIBS_INIT}
	glfw ${GLFW_LIBRARIES}
)


#########################################
# BUILD
#########################################
add_executable(9_Application ${APPLICATION_SRC})
add_definitions(-D${OPIFEX_OS})

target_link_libraries(9_Application	${ALL_LIBS})

if( ${OPIFEX_OS_WINDOWS} )
	target_link_libraries(9_Application	Winmm.lib)
endif()

if( ${MSVC_WIN7_FIX} )
	target_link_libraries(9_Application	Xinput9_1_0.lib)
endif()

set_target_properties(9_Application PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(9_Application PROPERTIES OUTPUT_NAME "Application")


#########################################
# ADDITIONAL FEATURES
#########################################
if(${OPIFEX_OCULUS})
	include_directories(../External/Oculus/include/	../External/Oculus/include/)
	add_definitions(-DOPIFEX_OCULUS)
endif()

if(${OPIFEX_MYO})
	include_directories(../External/Myo/include/)
	add_definitions(-DOPIFEX_MYO)
	if(${OPIFEX_WIN64})
		target_link_libraries(9_Application ${PROJECT_SOURCE_DIR}/External/Myo/lib/win64/myo64.lib)
		copy_to_folder(9_Application "/External/Myo/lib/" "myo64.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
		copy_to_folder(9_Application "/External/Myo/lib/" "ble64.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
	else()
		target_link_libraries(9_Application ${PROJECT_SOURCE_DIR}/External/Myo/lib/win32/myo32.lib)
		copy_to_folder(9_Application "/External/Myo/lib/" "myo32.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
		copy_to_folder(9_Application "/External/Myo/lib/" "ble32.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
	endif()
endif()

if(${OPIFEX_V8})
	if(${OPIFEX_OS_WINDOWS})
		target_link_libraries(9_Application 
			ws2_32.lib
			advapi32.lib
			winmm.lib)
		if(${OPIFEX_RELEASE})
			if(${OPIFEX_OS_64})
				target_link_libraries(9_Application ${PROJECT_SOURCE_DIR}/External/V8/lib/release/win64/v8.lib)
			else()
				target_link_libraries(9_Application ${PROJECT_SOURCE_DIR}/External/V8/lib/release/win32/v8.lib)
				target_link_libraries(9_Application ${PROJECT_SOURCE_DIR}/External/V8/lib/release/win32/v8_libbase.lib)
				target_link_libraries(9_Application ${PROJECT_SOURCE_DIR}/External/V8/lib/release/win32/v8_libplatform.lib)
			endif()

		else()
		
			if(${OPIFEX_OS_64})
				target_link_libraries(9_Application ${PROJECT_SOURCE_DIR}/External/V8/lib/debug/win64/v8.lib)
			else()
				target_link_libraries(9_Application ${PROJECT_SOURCE_DIR}/External/V8/lib/debug/win32/v8.lib)
				target_link_libraries(9_Application ${PROJECT_SOURCE_DIR}/External/V8/lib/debug/win32/v8_libbase.lib)
				target_link_libraries(9_Application ${PROJECT_SOURCE_DIR}/External/V8/lib/debug/win32/v8_libplatform.lib)
			endif()
		
		endif()
	endif()
	add_definitions(-DOPIFEX_V8)
endif()

if(1)
	add_definitions(-D_DEBUG)
endif()

if(${OPIFEX_RELEASE})
	add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)
endif()

#########################################
# OUTPUT BINARIES
#########################################
if(${OPIFEX_RELEASE})
	copy_to_folder(9_Application "/External/Ogg/lib/" "ogg.dll" "/Application/Release" ${OPIFEX_OS_WINDOWS})
	copy_to_folder(9_Application "/External/Ogg/lib/" "libogg.dll" "/Application/Release" ${OPIFEX_OS_WINDOWS})
	copy_to_folder(9_Application "/External/Vorbis/lib/" "vorbisfile.dll" "/Application/Release" ${OPIFEX_OS_WINDOWS})
	copy_to_folder(9_Application "/External/Vorbis/lib/" "libvorbis.dll" "/Application/Release" ${OPIFEX_OS_WINDOWS})
	copy_to_folder(9_Application "/External/Vorbis/lib/" "libvorbisfile.dll" "/Application/Release" ${OPIFEX_OS_WINDOWS})
else()	
	copy_to_folder(9_Application "/External/Ogg/lib/" "ogg.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
	copy_to_folder(9_Application "/External/Ogg/lib/" "libogg.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
	copy_to_folder(9_Application "/External/Vorbis/lib/" "vorbisfile.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
	copy_to_folder(9_Application "/External/Vorbis/lib/" "libvorbis.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
	copy_to_folder(9_Application "/External/Vorbis/lib/" "libvorbisfile.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
endif()

if(${OPIFEX_V8})
	if(${OPIFEX_RELEASE})
		copy_to_folder(9_Application "/External/V8/lib/release/" "icui18n.dll" "/Application/Release" ${OPIFEX_OS_WINDOWS})
		copy_to_folder(9_Application "/External/V8/lib/release/" "icuuc.dll" "/Application/Release" ${OPIFEX_OS_WINDOWS})
		copy_to_folder(9_Application "/External/V8/lib/release/" "v8.dll" "/Application/Release" ${OPIFEX_OS_WINDOWS})
	else()	
		copy_to_folder(9_Application "/External/V8/lib/debug/" "icui18n.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
		copy_to_folder(9_Application "/External/V8/lib/debug/" "icuuc.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
		copy_to_folder(9_Application "/External/V8/lib/debug/" "v8.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
	endif()
endif()