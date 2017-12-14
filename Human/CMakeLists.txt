#########################################
# HUMAN - LAYER 5
#########################################
cmake_minimum_required(VERSION 2.8)


#########################################
# Include Directories
#########################################
include_directories(
	.
	../
)

add_external_opifex_includes()

#########################################
# Include Files
#########################################
#message_glob(include/ "${PROJECT_SOURCE_DIR}/Human/include/Audio")
add_source_group(include/ "${PROJECT_SOURCE_DIR}/Human/include" HUMAN_SRC)
add_source_group(include/Audio "${PROJECT_SOURCE_DIR}/Human/include/Audio" HUMAN_SRC)
add_source_group(include/Input "${PROJECT_SOURCE_DIR}/Human/include/Input" HUMAN_SRC)
add_source_group(include/Input/Enums "${PROJECT_SOURCE_DIR}/Human/include/Input/Enums" HUMAN_SRC)
add_source_group(include/Rendering "${PROJECT_SOURCE_DIR}/Human/include/Rendering" HUMAN_SRC)
add_source_group(include/Rendering/API "${PROJECT_SOURCE_DIR}/Human/include/Rendering/API" HUMAN_SRC)
add_source_group(include/Rendering/Camera "${PROJECT_SOURCE_DIR}/Human/include/Rendering/Camera" HUMAN_SRC)
add_source_group(include/Rendering/Commands "${PROJECT_SOURCE_DIR}/Human/include/Rendering/Commands" HUMAN_SRC)
add_source_group(include/Rendering/Skinning "${PROJECT_SOURCE_DIR}/Human/include/Rendering/Skinning" HUMAN_SRC)
add_source_group(include/Rendering/Font "${PROJECT_SOURCE_DIR}/Human/include/Rendering/Font" HUMAN_SRC)
add_source_group(include/Rendering/GL "${PROJECT_SOURCE_DIR}/Human/include/Rendering/GL" HUMAN_SRC)
add_source_group(include/Rendering/Primitives "${PROJECT_SOURCE_DIR}/Human/include/Rendering/Primitives" HUMAN_SRC)
add_source_group(include/Rendering/Sprite "${PROJECT_SOURCE_DIR}/Human/include/Rendering/Sprite" HUMAN_SRC)
add_source_group(include/Rendering/UI "${PROJECT_SOURCE_DIR}/Human/include/Rendering/UI" HUMAN_SRC)
add_source_group(include/Rendering/Enums "${PROJECT_SOURCE_DIR}/Human/include/Rendering/Enums" HUMAN_SRC)
add_source_group(include/Systems "${PROJECT_SOURCE_DIR}/Human/include/Systems" HUMAN_SRC)
add_source_group(include/Utilities "${PROJECT_SOURCE_DIR}/Human/include/Utilities" HUMAN_SRC)
add_source_group(include/Platform "${PROJECT_SOURCE_DIR}/Human/include/Platform" HUMAN_SRC)
add_source_group(include/Platform/opengl "${PROJECT_SOURCE_DIR}/Human/include/Platform/opengl" HUMAN_SRC)
add_source_group(include/Platform/opengles "${PROJECT_SOURCE_DIR}/Human/include/Platform/opengles" HUMAN_SRC)
add_source_group(include/Platform/directx "${PROJECT_SOURCE_DIR}/Human/include/Platform/directx" HUMAN_SRC)


#########################################
# Source Files
#########################################
add_source_group(src/ "${PROJECT_SOURCE_DIR}/Human/src" HUMAN_SRC)
add_source_group(src/Audio "${PROJECT_SOURCE_DIR}/Human/src/Audio" HUMAN_SRC)
add_source_group(src/Input "${PROJECT_SOURCE_DIR}/Human/src/Input" HUMAN_SRC)
add_source_group(src/Input "${PROJECT_SOURCE_DIR}/Human/src/Input" HUMAN_SRC)
add_source_group(src/Rendering "${PROJECT_SOURCE_DIR}/Human/src/Rendering" HUMAN_SRC)
add_source_group(src/Rendering/API "${PROJECT_SOURCE_DIR}/Human/src/Rendering/API" HUMAN_SRC)
add_source_group(src/Rendering/Camera "${PROJECT_SOURCE_DIR}/Human/src/Rendering/Camera" HUMAN_SRC)
add_source_group(src/Rendering/Commands "${PROJECT_SOURCE_DIR}/Human/src/Rendering/Commands" HUMAN_SRC)
add_source_group(src/Rendering/Font "${PROJECT_SOURCE_DIR}/Human/src/Rendering/Font" HUMAN_SRC)
add_source_group(src/Rendering/GL "${PROJECT_SOURCE_DIR}/Human/src/Rendering/GL" HUMAN_SRC)
add_source_group(src/Rendering/Primitives "${PROJECT_SOURCE_DIR}/Human/src/Rendering/Primitives" HUMAN_SRC)
add_source_group(src/Rendering/Skinning "${PROJECT_SOURCE_DIR}/Human/src/Rendering/Skinning" HUMAN_SRC)
add_source_group(src/Rendering/Sprite "${PROJECT_SOURCE_DIR}/Human/src/Rendering/Sprite" HUMAN_SRC)
add_source_group(src/Rendering/UI "${PROJECT_SOURCE_DIR}/Human/src/Rendering/UI" HUMAN_SRC)
add_source_group(src/Systems "${PROJECT_SOURCE_DIR}/Human/src/Systems" HUMAN_SRC)
add_source_group(src/Utilities "${PROJECT_SOURCE_DIR}/Human/src/Utilities" HUMAN_SRC)
add_source_group(src/Platform "${PROJECT_SOURCE_DIR}/Human/src/Platform" HUMAN_SRC)
add_source_group(src/Platform/opengl "${PROJECT_SOURCE_DIR}/Human/src/Platform/opengl" HUMAN_SRC)
add_source_group(src/Platform/opengles "${PROJECT_SOURCE_DIR}/Human/src/Platform/opengles" HUMAN_SRC)
add_source_group(src/Platform/directx "${PROJECT_SOURCE_DIR}/Human/src/Platform/directx" HUMAN_SRC)


#########################################
# DEPENDENT LIBRARIES
#########################################
set(ALL_LIBS
	${OPENGL_LIBRARY}
	LODEPNG
	1_Core
	2_Data
	3_Math
	4_Performance
)


#########################################
# BUILD
#########################################
if(${OPIFEX_OPTION_SHARED})
	add_library(5_Human SHARED ${HUMAN_SRC})
else()
	add_library(5_Human STATIC ${HUMAN_SRC})
endif()

if(${OPIFEX_OS_IOS})
	set_xcode_property(5_Human GCC_GENERATE_DEBUGGING_SYMBOLS YES)
endif()
add_opifex_defines()
add_definitions(-DGLEW_STATIC -D_CRT_SECURE_NO_WARNINGS)

target_link_libraries(5_Human ${ALL_LIBS})
if(${OPIFEX_OS_ANDROID})
	target_link_libraries(5_Human GLESv2 EGL)
endif()

set_target_properties(5_Human PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(5_Human PROPERTIES OUTPUT_NAME "Human")


if(${OPIFEX_OPTION_RELEASE})
	add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)
endif()
add_marketplace_defines()

#########################################
# ADDITIONAL FEATURES
#########################################
add_opifex_oculus(5_Human)
add_opifex_myo(5_Human)
add_opifex_oggvorbis(5_Human)

output_library(5_Human "Human")
