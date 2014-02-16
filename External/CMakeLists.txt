### GLFW

SET( GLFW_LIBRARY_PATH "${PROJECT_SOURCE_DIR}/External/glfw-3.0.4" )
add_subdirectory("${GLFW_LIBRARY_PATH}" ${PROJECT_BINARY_DIR}/GLFW)
set_property(TARGET glfw PROPERTY FOLDER "External")
add_definitions(-DGLEW_STATIC -D_CRT_SECURE_NO_WARNINGS)

### GLEW

#########################################
# Include Directories
#########################################
include_directories(glew-1.9.0/include/)


#########################################
# Source Files
#########################################
set(GLEW_SOURCE	glew-1.9.0/src/glew.c)


#########################################
# BUILD
#########################################
add_library( GLEW_158 STATIC
	${GLEW_SOURCE}
	${GLEW_INCLUDE}
)

target_link_libraries(GLEW_158 ${OPENGL_LIBRARY})
set_property(TARGET GLEW_158 PROPERTY FOLDER "External")

populate_binary_directory()

output_library_from(GLEW_158 "/External" "GLEW_158")

output_binary(GLEW_158 "/External/Ogg/lib/" "libogg.lib" ${OPIFEX_OS_WINDOWS})
output_binary(GLEW_158 "/External/Vorbis/lib/" "libvorbis.lib" ${OPIFEX_OS_WINDOWS})
output_binary(GLEW_158 "/External/Vorbis/lib/" "libvorbisfile.lib" ${OPIFEX_OS_WINDOWS})