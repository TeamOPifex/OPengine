#########################################
# APPLICATION - LAYER 9
#########################################
cmake_minimum_required(VERSION 2.8)


#########################################
# Include Directories
#########################################
add_opifex_includes()

#########################################
# Source Files
#########################################
add_source_group(/ "${PROJECT_SOURCE_DIR}/Editor" APPLICATION_SRC)
add_asset_dir(APPLICATION_SRC)


#########################################
# DEPENDENT LIBRARIES
#########################################
set(ALL_LIBS
	LODEPNG
	1_Core
	2_Data
	3_Math
	4_Performance
	5_Human
	6_Communication
	7_Pipeline
	8_Scripting
	${ZLIB_LIBRARY}
	${LIBPNG_LIBRARY}
	${CMAKE_THREAD_LIBS_INIT}
  jansson
)


#########################################
# BUILD
#########################################


if(${OPIFEX_OS_IOS})
    if(${OPIFEX_OPTION_SHARED})
    	add_library(9_Editor SHARED ${APPLICATION_SRC})
    else()
    	add_library(9_Editor STATIC ${APPLICATION_SRC})
    endif()
    output_library(9_Editor "Editor")
else()
	 add_opifex_executable(9_Editor ${APPLICATION_SRC})
endif()

target_link_libraries(9_Editor ${ALL_LIBS})

set_target_properties(9_Editor PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(9_Editor PROPERTIES OUTPUT_NAME "Editor")

add_opifex_targets(9_Editor)

add_marketplace_addons(9_Editor "Application/${BINARY_RELEASE_MODE}/" OPIFEX_ASSETS_ADDITIONAL)
add_opifex_defines(OPIFEX_ASSETS_ADDITIONAL)

#########################################
# ADDITIONAL FEATURES
#########################################

add_opifex_spine(9_Editor)
add_opifex_physx(9_Editor)
add_opifex_v8(9_Editor)
add_opifex_oculus(9_Editor)
add_opifex_myo(9_Editor)
add_opifex_assets(9_Editor)
add_opifex_oggvorbis(9_Editor)
add_opifex_steam(9_Editor)
add_opifex_fmod(9_Editor)
add_opifex_xinput(9_Editor)
add_opifex_raknet(9_Editor)