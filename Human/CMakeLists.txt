cmake_minimum_required(VERSION 2.8)

include_directories(
	../External/glfw-2.7.2/include/
	../External/glm-0.9.1/
	../External/glew-1.5.8/include/
	../External/Phsyx.3.2.1/include/
	../External/GLES2/
	.
	../
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	GLFW_272
	GLEW_158
	1_Core
	2_Data
)

add_definitions(
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

add_source_group(\\ "${PROJECT_SOURCE_DIR}/Human" HUMAN_SRC)
add_source_group(Math "${PROJECT_SOURCE_DIR}/Human/Math" HUMAN_SRC)
add_source_group(Rendering "${PROJECT_SOURCE_DIR}/Human/Rendering" HUMAN_SRC)
add_source_group(Rendering/GL "${PROJECT_SOURCE_DIR}/Human/Rendering/GL" HUMAN_SRC)
add_source_group(Rendering/GL/ES "${PROJECT_SOURCE_DIR}/Human/Rendering/GL/ES" HUMAN_SRC)

add_source_group(Resources "${PROJECT_SOURCE_DIR}/Human/Resources" HUMAN_SRC)
add_source_group(Resources/Buffer "${PROJECT_SOURCE_DIR}/Human/Resources/Buffer" HUMAN_SRC)
add_source_group(Resources/Material "${PROJECT_SOURCE_DIR}/Human/Resources/Material" HUMAN_SRC)
add_source_group(Resources/Material/Shader "${PROJECT_SOURCE_DIR}/Human/Resources/Material/Shader" HUMAN_SRC)
add_source_group(Resources/Material/State "${PROJECT_SOURCE_DIR}/Human/Resources/Material/State" HUMAN_SRC)
add_source_group(Resources/Sound "${PROJECT_SOURCE_DIR}/Human/Resources/Sound" HUMAN_SRC)
add_source_group(Resources/Texture "${PROJECT_SOURCE_DIR}/Human/Resources/Texture" HUMAN_SRC)

add_library(3_Human STATIC ${HUMAN_SRC})
target_link_libraries(3_Human
	${ALL_LIBS}
)
SET_TARGET_PROPERTIES(3_Human PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(3_Human PROPERTIES 
	OUTPUT_NAME "Human"
)