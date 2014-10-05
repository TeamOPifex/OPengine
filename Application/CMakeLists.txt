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
	../External/Spine/include/
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

if(${OPIFEX_SPINE})
	target_link_libraries(9_Application SPINE)
	add_definitions(-DOPIFEX_SPINE)
endif()

if(${OPIFEX_PHYSICS}) 
	message(STATUS "Link Physics")
	add_definitions(-DOPIFEX_PHYSICS)
	include_directories(../External/PhysX/include/)
	if(${OPIFEX_RELEASE})
		message(STATUS "Physics RELEASE")
		target_link_libraries(9_Application ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win32/PhysX3_x86.lib)
		target_link_libraries(9_Application ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win32/PhysX3Common_x86.lib)
		target_link_libraries(9_Application ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win32/PhysX3Extensions.lib)
		target_link_libraries(9_Application ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win32/PxTask.lib)
	else()
		message(STATUS "Physics DEBUG")
		target_link_libraries(9_Application ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win32/PhysX3DEBUG_x86.lib)
		target_link_libraries(9_Application ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win32/PhysX3CommonDEBUG_x86.lib)
		target_link_libraries(9_Application ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win32/PhysX3ExtensionsDEBUG.lib)
		target_link_libraries(9_Application ${PROJECT_SOURCE_DIR}/External/PhysX/lib/win32/PxTaskDEBUG.lib)
	endif()
endif()

if(${OPIFEX_OCULUS})
	include_directories(../External/Oculus/include/	../External/Oculus/include/)
	add_definitions(-DOPIFEX_OCULUS)
	target_link_libraries(9_Application ${PROJECT_SOURCE_DIR}/External/OculusDK2/lib/win32/VS2010/libovrd.lib)
endif()

if(${OPIFEX_MYO})
	include_directories(../External/Myo/include/)
	add_definitions(-DOPIFEX_MYO)
	if(${OPIFEX_OS_WIN64})
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

unset(OPIFEX_REPO)
if(${OPIFEX_RELEASE})

else()
	add_definitions(-D_DEBUG)
	if(${OPIFEX_OS_WINDOWS})
		SET(OPIFEX_REPO "${OPIFEX_ENGINE_REPOSITORY}/Assets/")
		add_definitions(-DOPIFEX_REPO="${OPIFEX_REPO}")
	endif()
	
endif()

if(${OPIFEX_RELEASE})
	add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)
	target_link_libraries(9_Application	libcpmtd0.lib)
	target_link_libraries(9_Application	${OGG_LIBRARY_RELEASE} ${VORBIS_LIBRARY_RELEASE} ${VORBISFILE_LIBRARY_RELEASE})
else()
	target_link_libraries(9_Application	${OGG_LIBRARY} ${VORBIS_LIBRARY} ${VORBISFILE_LIBRARY})
endif()

if(${OPIFEX_RELEASE})
	copy_to_folder(9_Application "/External/Ogg/lib/release/" "libogg.dll" "/Application/Release" ${OPIFEX_OS_WINDOWS})
	copy_to_folder(9_Application "/External/Vorbis/lib/release/" "libvorbis.dll" "/Application/Release" ${OPIFEX_OS_WINDOWS})
	copy_to_folder(9_Application "/External/Vorbis/lib/release/" "libvorbisfile.dll" "/Application/Release" ${OPIFEX_OS_WINDOWS})
else()
	copy_to_folder(9_Application "/External/Ogg/lib/debug/" "libogg.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
	copy_to_folder(9_Application "/External/Vorbis/lib/debug/" "libvorbis.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
	copy_to_folder(9_Application "/External/Vorbis/lib/debug/" "libvorbisfile.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
endif()

#########################################
# OUTPUT BINARIES
#########################################

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


if(${OPIFEX_PHYSICS}) 
	if(${OPIFEX_RELEASE})
		copy_to_folder(9_Application "/External/PhysX/lib/release/" "PhysX3_x86.dll" "/Application/Release" ${OPIFEX_OS_WINDOWS})
		copy_to_folder(9_Application "/External/PhysX/lib/release/" "PhysX3Common_x86.dll" "/Application/Release" ${OPIFEX_OS_WINDOWS})
	else()
		copy_to_folder(9_Application "/External/PhysX/lib/debug/" "PhysX3CHECKED_x86.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
		copy_to_folder(9_Application "/External/PhysX/lib/debug/" "PhysX3CommonCHECKED_x86.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
		copy_to_folder(9_Application "/External/PhysX/lib/debug/" "nvToolsExt32_1.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
	endif()
endif()