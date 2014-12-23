#########################################
# HUMAN - LAYER 5
#########################################
cmake_minimum_required(VERSION 2.8)


#########################################
# Include Directories
#########################################
include_directories(
	../External/glfw-3.0.4/include/
	../External/glew-1.9.0/include/
	../External/Phsyx.3.2.1/include/
	../External/GLES2/
	../External/Ogg/include/
	../External/Vorbis/include/
	../External/OpenAL/
	../External/VP8/include/
	.
	../
)


#########################################
# Include Files
#########################################
#message_glob(include/ "${PROJECT_SOURCE_DIR}/Human/include/Audio")
add_source_group(include/ "${PROJECT_SOURCE_DIR}/Human/include" HUMAN_SRC)
add_source_group(include/Audio "${PROJECT_SOURCE_DIR}/Human/include/Audio" HUMAN_SRC)
add_source_group(include/Input "${PROJECT_SOURCE_DIR}/Human/include/Input" HUMAN_SRC)
add_source_group(include/Input "${PROJECT_SOURCE_DIR}/Human/include/Input" HUMAN_SRC)
add_source_group(include/Rendering "${PROJECT_SOURCE_DIR}/Human/include/Rendering" HUMAN_SRC)
add_source_group(include/Rendering/Skinning "${PROJECT_SOURCE_DIR}/Human/include/Rendering/Skinning" HUMAN_SRC)
add_source_group(include/Rendering/Font "${PROJECT_SOURCE_DIR}/Human/include/Rendering/Font" HUMAN_SRC)
add_source_group(include/Rendering/GL "${PROJECT_SOURCE_DIR}/Human/include/Rendering/GL" HUMAN_SRC)
add_source_group(include/Rendering/Primitives "${PROJECT_SOURCE_DIR}/Human/include/Rendering/Primitives" HUMAN_SRC)
add_source_group(include/Rendering/Sprite "${PROJECT_SOURCE_DIR}/Human/include/Rendering/Sprite" HUMAN_SRC)
add_source_group(include/Systems "${PROJECT_SOURCE_DIR}/Human/include/Systems" HUMAN_SRC)
add_source_group(include/Utilities "${PROJECT_SOURCE_DIR}/Human/include/Utilities" HUMAN_SRC)


#########################################
# Source Files
#########################################
add_source_group(src/ "${PROJECT_SOURCE_DIR}/Human/src" HUMAN_SRC)
add_source_group(src/Audio "${PROJECT_SOURCE_DIR}/Human/src/Audio" HUMAN_SRC)
add_source_group(src/Input "${PROJECT_SOURCE_DIR}/Human/src/Input" HUMAN_SRC)
add_source_group(src/Input "${PROJECT_SOURCE_DIR}/Human/src/Input" HUMAN_SRC)
add_source_group(src/Rendering "${PROJECT_SOURCE_DIR}/Human/src/Rendering" HUMAN_SRC)
add_source_group(src/Rendering/Font "${PROJECT_SOURCE_DIR}/Human/src/Rendering/Font" HUMAN_SRC)
add_source_group(src/Rendering/GL "${PROJECT_SOURCE_DIR}/Human/src/Rendering/GL" HUMAN_SRC)
add_source_group(src/Rendering/Primitives "${PROJECT_SOURCE_DIR}/Human/src/Rendering/Primitives" HUMAN_SRC)
add_source_group(src/Rendering/Skinning "${PROJECT_SOURCE_DIR}/Human/src/Rendering/Skinning" HUMAN_SRC)
add_source_group(src/Rendering/Sprite "${PROJECT_SOURCE_DIR}/Human/src/Rendering/Sprite" HUMAN_SRC)
add_source_group(src/Systems "${PROJECT_SOURCE_DIR}/Human/src/Systems" HUMAN_SRC)
add_source_group(src/Utilities "${PROJECT_SOURCE_DIR}/Human/src/Utilities" HUMAN_SRC)


#########################################
# DEPENDENT LIBRARIES
#########################################
set(ALL_LIBS
	${OPENGL_LIBRARY}
	${OPENAL_LIBRARY}
	GLEW_158
	LODEPNG
	1_Core
	2_Data
	3_Math
	4_Performance
	glfw 
	${GLFW_LIBRARIES}
)


#########################################
# BUILD
#########################################
if(${OPIFEX_SHARED})
	add_library(5_Human SHARED ${HUMAN_SRC})
else()
	add_library(5_Human STATIC ${HUMAN_SRC})
endif()
add_definitions(-DGLEW_STATIC -D_CRT_SECURE_NO_WARNINGS)
add_definitions(-D${OPIFEX_OS})

target_link_libraries(5_Human ${ALL_LIBS})

set_target_properties(5_Human PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(5_Human PROPERTIES OUTPUT_NAME "Human")


if(${OPIFEX_RELEASE})

else()
	add_definitions(-D_DEBUG)
endif()
if(${OPIFEX_RELEASE})
	add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)
endif()

#########################################
# ADDITIONAL FEATURES
#########################################
add_opifex_oculus(5_Human)
add_opifex_myo(5_Human)
add_opifex_oggvorbis(5_Human)

output_library(5_Human "Human")
