cmake_minimum_required(VERSION 2.8)
find_package(OpenGL REQUIRED)

add_subdirectory (external)
include_directories(
	external/glfw-2.7.2/include/
	external/glm-0.9.1/
	external/glew-1.5.8/include/
	.
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	GLFW_272
	GLEW_158
)

add_definitions(
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

add_source_group(\\ "${PROJECT_SOURCE_DIR}/Application" APPLICATION_SRC)

add_executable(7_Application ${APPLICATION_SRC})

target_link_libraries(7_Application
	${ALL_LIBS}
)

SET_TARGET_PROPERTIES(7_Application PROPERTIES LINKER_LANGUAGE C)
set_target_properties(7_Application PROPERTIES 
	OUTPUT_NAME "Application"
)