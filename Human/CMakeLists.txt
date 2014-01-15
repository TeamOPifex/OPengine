cmake_minimum_required(VERSION 2.8)

include_directories(
	../External/glfw-2.7.6/include/
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
	GLFW_276
	GLEW_158
	1_Core
	2_Data
	3_Math
	4_Performance
)

add_definitions(
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

add_source_group(include/ "${PROJECT_SOURCE_DIR}/Human/include" HUMAN_SRC)
add_source_group(include/Audio "${PROJECT_SOURCE_DIR}/Human/include/Audio" HUMAN_SRC)
add_source_group(include/Input "${PROJECT_SOURCE_DIR}/Human/include/Input" HUMAN_SRC)
add_source_group(include/Input "${PROJECT_SOURCE_DIR}/Human/include/Input" HUMAN_SRC)
add_source_group(include/Rendering "${PROJECT_SOURCE_DIR}/Human/include/Rendering" HUMAN_SRC)
add_source_group(include/Rendering/GL "${PROJECT_SOURCE_DIR}/Human/include/Rendering/GL" HUMAN_SRC)
add_source_group(include/Utilities "${PROJECT_SOURCE_DIR}/Human/include/Utilities" HUMAN_SRC)

add_source_group(src/ "${PROJECT_SOURCE_DIR}/Human/src" HUMAN_SRC)
add_source_group(src/Audio "${PROJECT_SOURCE_DIR}/Human/src/Audio" HUMAN_SRC)
add_source_group(src/Input "${PROJECT_SOURCE_DIR}/Human/src/Input" HUMAN_SRC)
add_source_group(src/Input "${PROJECT_SOURCE_DIR}/Human/src/Input" HUMAN_SRC)
add_source_group(src/Rendering "${PROJECT_SOURCE_DIR}/Human/src/Rendering" HUMAN_SRC)
add_source_group(src/Rendering/GL "${PROJECT_SOURCE_DIR}/Human/src/Rendering/GL" HUMAN_SRC)
add_source_group(src/Utilities "${PROJECT_SOURCE_DIR}/Human/src/Utilities" HUMAN_SRC)

add_library(5_Human STATIC ${HUMAN_SRC})
	
if( "${OPIFEX_OS}" STREQUAL "OPIFEX_WIN32" )

	add_custom_command(TARGET 5_Human POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${PROJECT_SOURCE_DIR}/External/Ogg/lib/win32/ogg.dll"
		${PROJECT_BINARY_DIR}/Binaries/win32/)

	add_custom_command(TARGET 5_Human POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${PROJECT_SOURCE_DIR}/External/Ogg/lib/win32/libogg.dll"
		${PROJECT_BINARY_DIR}/Binaries/win32/)

	add_custom_command(TARGET 5_Human POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${PROJECT_SOURCE_DIR}/External/Vorbis/lib/win32/vorbisfile.dll"
		${PROJECT_BINARY_DIR}/Binaries/win32/)

	add_custom_command(TARGET 5_Human POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${PROJECT_SOURCE_DIR}/External/Vorbis/lib/win32/libvorbis.dll"
		${PROJECT_BINARY_DIR}/Binaries/win32/)
		
	add_custom_command(TARGET 5_Human POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${PROJECT_SOURCE_DIR}/External/Vorbis/lib/win32/libvorbisfile.dll"
		${PROJECT_BINARY_DIR}/Binaries/win32/)
		
	add_custom_command(TARGET 5_Human POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${PROJECT_BINARY_DIR}/Human/Debug/Human.lib"
		${PROJECT_BINARY_DIR}/Binaries/win32/)
	
elseif("${OPIFEX_OS}" STREQUAL "OPIFEX_LINUX32")

	add_custom_command(TARGET 5_Human POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${PROJECT_SOURCE_DIR}/External/Ogg/lib/win32/ogg.dll"
		${PROJECT_BINARY_DIR}/Binaries/linux32/)

	add_custom_command(TARGET 5_Human POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${PROJECT_SOURCE_DIR}/External/Ogg/lib/win32/libogg.dll"
		${PROJECT_BINARY_DIR}/Binaries/linux32/)

	add_custom_command(TARGET 5_Human POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${PROJECT_SOURCE_DIR}/External/Vorbis/lib/win32/vorbisfile.dll"
		${PROJECT_BINARY_DIR}/Binaries/linux32/)

	add_custom_command(TARGET 5_Human POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${PROJECT_SOURCE_DIR}/External/Vorbis/lib/win32/libvorbis.dll"
		${PROJECT_BINARY_DIR}/Binaries/linux32/)
		
	add_custom_command(TARGET 5_Human POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${PROJECT_SOURCE_DIR}/External/Vorbis/lib/win32/libvorbisfile.dll"
		${PROJECT_BINARY_DIR}/Binaries/linux32/)
		
	add_custom_command(TARGET 5_Human POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${PROJECT_BINARY_DIR}/Human/libHuman.a"
		${PROJECT_BINARY_DIR}/Binaries/linux32/)

endif()
	
add_definitions(-D${OPIFEX_OS})
target_link_libraries(5_Human
	${ALL_LIBS}
	${OGG_LIBRARY}
	${VORBIS_LIBRARY}
	${VORBISFILE_LIBRARY}
	${PROJECT_SOURCE_DIR}/External/Oculus/lib/win32/libovr.lib
)
SET_TARGET_PROPERTIES(5_Human PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(5_Human PROPERTIES 
	OUTPUT_NAME "Human"
)