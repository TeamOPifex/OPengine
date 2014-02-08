

set(GLFW_LIBRARY_PATH "${PROJECT_SOURCE_DIR}/External/glfw-3.0.4")
add_subdirectory("${GLFW_LIBRARY_PATH}" ${PROJECT_BINARY_DIR}/GLFW)

add_definitions(
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

include_directories(
	glew-1.9.0/include/
)

set(GLEW_SOURCE
	glew-1.9.0/src/glew.c
)

set(GLEW_HEADERS
)


add_library( GLEW_158 STATIC
	${GLEW_SOURCE}
	${GLEW_INCLUDE}
)

target_link_libraries(GLEW_158
	${OPENGL_LIBRARY}
)


if( "${OPIFEX_OS}" STREQUAL "OPIFEX_WIN32" )

	add_custom_command(TARGET GLEW_158 POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${PROJECT_BINARY_DIR}/External/debug/GLEW_158.lib"
		${PROJECT_BINARY_DIR}/Binaries/win32/)
		
	file(COPY ${PROJECT_SOURCE_DIR}/External/OpenAL32.lib DESTINATION ${PROJECT_BINARY_DIR}/Binaries/win32/)
	file(COPY ${PROJECT_SOURCE_DIR}/External/Ogg/lib/win32/libogg.lib DESTINATION ${PROJECT_BINARY_DIR}/Binaries/win32/)
	file(COPY ${PROJECT_SOURCE_DIR}/External/Vorbis/lib/win32/libvorbis.lib DESTINATION ${PROJECT_BINARY_DIR}/Binaries/win32/)
	file(COPY ${PROJECT_SOURCE_DIR}/External/Vorbis/lib/win32/libvorbisfile.lib DESTINATION ${PROJECT_BINARY_DIR}/Binaries/win32/)
	
elseif( "${OPIFEX_OS}" STREQUAL "OPIFEX_LINUX32" )

	add_custom_command(TARGET GLEW_158 POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${PROJECT_BINARY_DIR}/External/libGLEW_158.a"
		${PROJECT_BINARY_DIR}/Binaries/linux32/)
		
	file(COPY ${PROJECT_SOURCE_DIR}/External/Ogg/lib/linux32/libogg.a DESTINATION ${PROJECT_BINARY_DIR}/Binaries/linux32/)
	file(COPY ${PROJECT_SOURCE_DIR}/External/Vorbis/lib/linux32/libvorbis.a DESTINATION ${PROJECT_BINARY_DIR}/Binaries/linux32/)
	file(COPY ${PROJECT_SOURCE_DIR}/External/Vorbis/lib/linux32/libvorbisfile.a DESTINATION ${PROJECT_BINARY_DIR}/Binaries/linux32/)
		
endif()