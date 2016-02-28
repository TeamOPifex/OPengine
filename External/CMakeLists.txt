### jansson
SET(JANSSON_WITHOUT_TESTS ON CACHE BOOL "Build JANSSON_WITHOUT_TESTS" FORCE)
SET(JANSSON_EXAMPLES OFF CACHE BOOL "Build JANSSON_EXAMPLES" FORCE)

option(JANSSON_WITHOUT_TESTS "Build shared libraries" OFF)
SET( JANSSON_LIBRARY_PATH "${PROJECT_SOURCE_DIR}/External/jansson" )
add_subdirectory("${JANSSON_LIBRARY_PATH}" ${PROJECT_BINARY_DIR}/jansson)
set_property(TARGET jansson PROPERTY FOLDER "External")

file(GLOB JANSSON_INCLUDES
	"${PROJECT_BINARY_DIR}/jansson/include/*.h"
)
message(STATUS "JANSON INCLUDES ${PROJECT_BINARY_DIR}/jansson/include/ | ${JANSSON_INCLUDES}")

set_binary_output(BINARY_FOLDER)
message(STATUS "Installing to ${BINARY_FOLDER}")
file(COPY ${JANSSON_INCLUDES} DESTINATION ${BINARY_FOLDER})
output_library_from(jansson "/jansson/lib" "jansson")

### GLFW
SET(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build GLFW_BUILD_EXAMPLES" FORCE)
SET(GLFW_INSTALL OFF CACHE BOOL "Build GLFW_BUILD_EXAMPLES" FORCE)
if(${OPIFEX_OS_IOS})
else()

SET( GLFW_LIBRARY_PATH "${PROJECT_SOURCE_DIR}/External/glfw" )
add_subdirectory("${GLFW_LIBRARY_PATH}" ${PROJECT_BINARY_DIR}/GLFW)
set_property(TARGET glfw PROPERTY FOLDER "External")
add_definitions(-DGLEW_STATIC -D_CRT_SECURE_NO_WARNINGS)

output_library_from(glfw "/GLFW/src" "glfw3")

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


endif()


### LodePNG

#########################################
# Include Files
#########################################
add_source_group(include/ "${PROJECT_SOURCE_DIR}/External/LodePNG/include" LODEPNG_SRC)

#########################################
# Source Files
#########################################
add_source_group(src/ "${PROJECT_SOURCE_DIR}/External/LodePNG/src" LODEPNG_SRC)



#########################################
# BUILD
#########################################
add_library( LODEPNG STATIC	${LODEPNG_SRC})

set_property(TARGET LODEPNG PROPERTY FOLDER "External")
set_target_properties(LODEPNG PROPERTIES OUTPUT_NAME "LodePNG")

if(${OPIFEX_OPTION_RELEASE})
	add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)
endif()

populate_binary_directory()

output_library_from(LODEPNG "/External" "LodePNG")





### Mongoose

#########################################
# Include Files
#########################################
add_source_group(include/ "${PROJECT_SOURCE_DIR}/External/Mongoose/include" MONGOOSE_SRC)

#########################################
# Source Files
#########################################
add_source_group(src/ "${PROJECT_SOURCE_DIR}/External/Mongoose/src" MONGOOSE_SRC)

#########################################
# BUILD
#########################################
add_library( MONGOOSE STATIC	${MONGOOSE_SRC})

set_property(TARGET MONGOOSE PROPERTY FOLDER "External")
set_target_properties(MONGOOSE PROPERTIES OUTPUT_NAME "Mongoose")

if(${OPIFEX_OPTION_RELEASE})
	add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)
endif()

populate_binary_directory()

output_library_from(MONGOOSE "/External" "Mongoose")







### Spine

if(${OPIFEX_OPTION_SPINE})
	include_directories("${PROJECT_SOURCE_DIR}/External/Spine/include")

	#########################################
	# Include Files
	#########################################
	add_source_group(include/ "${PROJECT_SOURCE_DIR}/External/Spine/include/spine" SPINE_SRC)

	#########################################
	# Source Files
	#########################################
	add_source_group(src/ "${PROJECT_SOURCE_DIR}/External/Spine/src/spine" SPINE_SRC)

	#########################################
	# BUILD
	#########################################
	add_library( SPINE STATIC	${SPINE_SRC})

	set_source_files_properties(SPINE_SRC PROPERTIES LANGUAGE CXX)

	set_property(TARGET SPINE PROPERTY FOLDER "External")
	set_target_properties(SPINE PROPERTIES OUTPUT_NAME "Spine")

	if(${OPIFEX_OPTION_RELEASE})
		add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)
	endif()

	populate_binary_directory()

	output_library_from(SPINE "/External" "Spine")
endif()
