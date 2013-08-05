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
)

add_definitions(
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

add_source_group(include/ "${PROJECT_SOURCE_DIR}/Human/include" HUMAN_SRC)
add_source_group(include/Audio "${PROJECT_SOURCE_DIR}/Human/include/Audio" HUMAN_SRC)
add_source_group(include/Input "${PROJECT_SOURCE_DIR}/Human/include/Input" HUMAN_SRC)
add_source_group(include/Math "${PROJECT_SOURCE_DIR}/Human/include/Math" HUMAN_SRC)
add_source_group(include/Input "${PROJECT_SOURCE_DIR}/Human/include/Input" HUMAN_SRC)
add_source_group(include/Rendering "${PROJECT_SOURCE_DIR}/Human/include/Rendering" HUMAN_SRC)
add_source_group(include/Rendering/GL "${PROJECT_SOURCE_DIR}/Human/include/Rendering/GL" HUMAN_SRC)
add_source_group(include/Resources "${PROJECT_SOURCE_DIR}/Human/include/Resources" HUMAN_SRC)
add_source_group(include/Resources/Buffer "${PROJECT_SOURCE_DIR}/Human/include/Resources/Buffer" HUMAN_SRC)
add_source_group(include/Resources/Model "${PROJECT_SOURCE_DIR}/Human/include/Resources/Model" HUMAN_SRC)
add_source_group(include/Resources/Material "${PROJECT_SOURCE_DIR}/Human/include/Resources/Material" HUMAN_SRC)
add_source_group(include/Resources/Material/Shader "${PROJECT_SOURCE_DIR}/Human/include/Resources/Material/Shader" HUMAN_SRC)
add_source_group(include/Resources/Material/Shader/Params "${PROJECT_SOURCE_DIR}/Human/include/Resources/Material/Shader/Params" HUMAN_SRC)
add_source_group(include/Resources/Material/Shader/Attributes "${PROJECT_SOURCE_DIR}/Human/include/Resources/Material/Shader/Attributes" HUMAN_SRC)
add_source_group(include/Resources/Material/State "${PROJECT_SOURCE_DIR}/Human/include/Resources/Material/State" HUMAN_SRC)
add_source_group(include/Resources/Sound "${PROJECT_SOURCE_DIR}/Human/include/Resources/Sound" HUMAN_SRC)
add_source_group(include/Resources/Texture "${PROJECT_SOURCE_DIR}/Human/include/Resources/Texture" HUMAN_SRC)
add_source_group(include/Utilities "${PROJECT_SOURCE_DIR}/Human/include/Utilities" HUMAN_SRC)

add_source_group(src/ "${PROJECT_SOURCE_DIR}/Human/src" HUMAN_SRC)
add_source_group(src/Audio "${PROJECT_SOURCE_DIR}/Human/src/Audio" HUMAN_SRC)
add_source_group(src/Input "${PROJECT_SOURCE_DIR}/Human/src/Input" HUMAN_SRC)
add_source_group(src/Math "${PROJECT_SOURCE_DIR}/Human/src/Math" HUMAN_SRC)
add_source_group(src/Input "${PROJECT_SOURCE_DIR}/Human/src/Input" HUMAN_SRC)
add_source_group(src/Rendering "${PROJECT_SOURCE_DIR}/Human/src/Rendering" HUMAN_SRC)
add_source_group(src/Rendering/GL "${PROJECT_SOURCE_DIR}/Human/src/Rendering/GL" HUMAN_SRC)
add_source_group(src/Resources "${PROJECT_SOURCE_DIR}/Human/src/Resources" HUMAN_SRC)
add_source_group(src/Resources/Buffer "${PROJECT_SOURCE_DIR}/Human/src/Resources/Buffer" HUMAN_SRC)
add_source_group(src/Resources/Model "${PROJECT_SOURCE_DIR}/Human/src/Resources/Model" HUMAN_SRC)
add_source_group(src/Resources/Material "${PROJECT_SOURCE_DIR}/Human/src/Resources/Material" HUMAN_SRC)
add_source_group(src/Resources/Material/Shader "${PROJECT_SOURCE_DIR}/Human/src/Resources/Material/Shader" HUMAN_SRC)
add_source_group(src/Resources/Material/Shader/Params "${PROJECT_SOURCE_DIR}/Human/src/Resources/Material/Shader/Params" HUMAN_SRC)
add_source_group(src/Resources/Material/Shader/Attributes "${PROJECT_SOURCE_DIR}/Human/src/Resources/Material/Shader/Attributes" HUMAN_SRC)
add_source_group(src/Resources/Material/State "${PROJECT_SOURCE_DIR}/Human/src/Resources/Material/State" HUMAN_SRC)
add_source_group(src/Resources/Sound "${PROJECT_SOURCE_DIR}/Human/src/Resources/Sound" HUMAN_SRC)
add_source_group(src/Resources/Texture "${PROJECT_SOURCE_DIR}/Human/src/Resources/Texture" HUMAN_SRC)
add_source_group(src/Utilities "${PROJECT_SOURCE_DIR}/Human/src/Utilities" HUMAN_SRC)

add_library(4_Human STATIC ${HUMAN_SRC})
	
if( "${OPIFEX_OS}" STREQUAL "OPIFEX_WIN32" )

	add_custom_command(TARGET 4_Human POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${PROJECT_SOURCE_DIR}/External/Ogg/lib/win32/ogg.dll"
		${PROJECT_BINARY_DIR}/Binaries/win32/)

	add_custom_command(TARGET 4_Human POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${PROJECT_SOURCE_DIR}/External/Ogg/lib/win32/libogg.dll"
		${PROJECT_BINARY_DIR}/Binaries/win32/)

	add_custom_command(TARGET 4_Human POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${PROJECT_SOURCE_DIR}/External/Vorbis/lib/win32/vorbisfile.dll"
		${PROJECT_BINARY_DIR}/Binaries/win32/)

	add_custom_command(TARGET 4_Human POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${PROJECT_SOURCE_DIR}/External/Vorbis/lib/win32/libvorbis.dll"
		${PROJECT_BINARY_DIR}/Binaries/win32/)
		
	add_custom_command(TARGET 4_Human POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${PROJECT_SOURCE_DIR}/External/Vorbis/lib/win32/libvorbisfile.dll"
		${PROJECT_BINARY_DIR}/Binaries/win32/)
		
	add_custom_command(TARGET 4_Human POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${PROJECT_BINARY_DIR}/Human/Debug/Human.lib"
		${PROJECT_BINARY_DIR}/Binaries/win32/)
	
endif()
	
target_link_libraries(4_Human
	${ALL_LIBS}
	${OGG_LIBRARY}
	${VORBIS_LIBRARY}
	${VORBISFILE_LIBRARY}
)
SET_TARGET_PROPERTIES(4_Human PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(4_Human PROPERTIES 
	OUTPUT_NAME "Human"
)