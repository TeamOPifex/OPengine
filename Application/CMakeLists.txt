#########################################
# APPLICATION - LAYER 9
#########################################
cmake_minimum_required(VERSION 2.8)


#########################################
# Include Directories
#########################################
include_directories(
	../External/glfw-3.0.4/include/
	../External/glm-0.9.5/
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

add_opifex_spine(9_Application)
add_opifex_physics(9_Application)
add_opifex_v8(9_Application)
add_opifex_oculus(9_Application)
add_opifex_myo(9_Application)
add_opifex_assets(9_Application)
add_opifex_oggvorbis(9_Application)