cmake_minimum_required(VERSION 2.8)

include_directories(
	../External/glfw-3.0.4/include/
	../External/glm-0.9.1/
	../External/glew-1.5.8/include/
	../External/Phsyx.3.2.1/include/
	../External/GLES2/
	../External/Ogg/include/
	../External/Vorbis/include/
	../External/OpenAL/
	../External/Oculus/include/
	../External/Oculus/Src/
	.
	../
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	${OPENAL_LIBRARY}
	1_Core
	2_Data
	3_Math
	4_Performance
	5_Human
	
)


#if( "${OPIFEX_OS}" STREQUAL "OPIFEX_ANDROID" )
	#add_source_group(\\ "${PROJECT_SOURCE_DIR}/Android" APPLICATION_SRC)
#elseif(1)
	add_source_group(\\ "${PROJECT_SOURCE_DIR}/Application" APPLICATION_SRC)
#endif()

if("${OPIFEX_OS}" STREQUAL "OPIFEX_OSX32")
	message(STATUS "GLFW_LIBRARIES (${GLFW_LIBRARIES})")
endif()

add_executable(8_Application ${APPLICATION_SRC})

if( "${OPIFEX_OS}" STREQUAL "OPIFEX_WIN32" )

	add_custom_command(TARGET 8_Application POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${PROJECT_SOURCE_DIR}/External/Ogg/lib/win32/ogg.dll"
		${PROJECT_BINARY_DIR}/Application/Debug)

	add_custom_command(TARGET 8_Application POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${PROJECT_SOURCE_DIR}/External/Ogg/lib/win32/libogg.dll"
		${PROJECT_BINARY_DIR}/Application/Debug)

	add_custom_command(TARGET 8_Application POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${PROJECT_SOURCE_DIR}/External/Vorbis/lib/win32/vorbisfile.dll"
		${PROJECT_BINARY_DIR}/Application/Debug)

	add_custom_command(TARGET 8_Application POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${PROJECT_SOURCE_DIR}/External/Vorbis/lib/win32/libvorbis.dll"
		${PROJECT_BINARY_DIR}/Application/Debug)
		
	add_custom_command(TARGET 8_Application POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${PROJECT_SOURCE_DIR}/External/Vorbis/lib/win32/libvorbisfile.dll"
		${PROJECT_BINARY_DIR}/Application/Debug)
		
elseif("${OPIFEX_OS}" STREQUAL "OPIFEX_LINUX32")

endif()

target_link_libraries(8_Application
	${ALL_LIBS}
	${OGG_LIBRARY}
	${VORBIS_LIBRARY}
	${VORBISFILE_LIBRARY}
	${ZLIB_LIBRARY}
	${LIBPNG_LIBRARY}
	${CMAKE_THREAD_LIBS_INIT}
	glfw ${GLFW_LIBRARIES}
)	


# Make sure the XInput library is linked correctly (Regardless of which Visual Studio is used)
# Windows 7 - http://en.wikipedia.org/wiki/Windows_NT#Releases
# MSVC10 - Visual Studio 2010
if( ${CMAKE_SYSTEM} STREQUAL "Windows-6.1" AND ${MSVC_VERSION} GREATER 1600 )
	target_link_libraries(8_Application
		Xinput9_1_0.lib
	)
endif()

add_definitions(-D${OPIFEX_OS})

SET_TARGET_PROPERTIES(8_Application PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(8_Application PROPERTIES 
	OUTPUT_NAME "Application"
)

message(STATUS "LINKED THINGS")