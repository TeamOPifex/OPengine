#########################################
# APPLICATION - LAYER 8
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
	${OGG_LIBRARY}
	${VORBIS_LIBRARY}
	${VORBISFILE_LIBRARY}
	${ZLIB_LIBRARY}
	${LIBPNG_LIBRARY}
	${CMAKE_THREAD_LIBS_INIT}
	glfw ${GLFW_LIBRARIES}
	Winmm.lib
)


#########################################
# BUILD
#########################################
add_executable(8_Application ${APPLICATION_SRC})
add_definitions(-D${OPIFEX_OS})

target_link_libraries(8_Application	${ALL_LIBS})

if( ${OPIFEX_OS_WINDOWS} )
	target_link_libraries(8_Application	Winmm.lib)
endif()

if( ${MSVC_WIN7_FIX} )
	target_link_libraries(8_Application	Xinput9_1_0.lib)
endif()

set_target_properties(8_Application PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(8_Application PROPERTIES OUTPUT_NAME "Application")


#########################################
# ADDITIONAL FEATURES
#########################################
if(${OPIFEX_OCULUS})
	include_directories(../External/Oculus/include/	../External/Oculus/Src/)
	target_link_libraries(8_Application ${PROJECT_SOURCE_DIR}/External/Oculus/lib/win32/libovr.lib)
	add_definitions(-DOPIFEX_OCULUS)
endif()

if(${OPIFEX_MYO})
	include_directories(../External/Myo/include/)
	target_link_libraries(8_Application ${PROJECT_SOURCE_DIR}/External/Myo/lib/win64/myo.lib)
	add_definitions(-DOPIFEX_MYO)
	copy_to_folder(8_Application "/External/Myo/lib/" "myo.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
	copy_to_folder(8_Application "/External/Myo/lib/" "ble.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
endif()


#########################################
# OUTPUT BINARIES
#########################################
copy_to_folder(8_Application "/External/Ogg/lib/" "ogg.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
copy_to_folder(8_Application "/External/Ogg/lib/" "libogg.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
copy_to_folder(8_Application "/External/Vorbis/lib/" "vorbisfile.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
copy_to_folder(8_Application "/External/Vorbis/lib/" "libvorbis.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
copy_to_folder(8_Application "/External/Vorbis/lib/" "libvorbisfile.dll" "/Application/Debug" ${OPIFEX_OS_WINDOWS})
