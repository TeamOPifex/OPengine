#########################################
# APPLICATION - LAYER 9
#########################################
cmake_minimum_required(VERSION 2.8)


#########################################
# Include Directories
#########################################
add_opifex_includes()
add_marketplace_addons_projects()

#########################################
# Source Files
#########################################
add_source_group(/ "${PROJECT_SOURCE_DIR}/Application" APPLICATION_SRC)
add_source_group(Examples "${PROJECT_SOURCE_DIR}/Application/Examples" APPLICATION_SRC)
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
    	add_library(9_Application SHARED ${APPLICATION_SRC})
    else()
    	add_library(9_Application STATIC ${APPLICATION_SRC})
    endif()
    output_library(9_Application "Application")
else()
	 add_opifex_executable(9_Application ${APPLICATION_SRC})
endif()

target_link_libraries(9_Application ${ALL_LIBS})

set_target_properties(9_Application PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(9_Application PROPERTIES OUTPUT_NAME "Application")

add_opifex_targets(9_Application)
add_opifex_defines()

#########################################
# ADDITIONAL FEATURES
#########################################

add_opifex_spine(9_Application)
add_opifex_physx(9_Application)
add_opifex_v8(9_Application)
add_opifex_oculus(9_Application)
add_opifex_myo(9_Application)
add_opifex_assets(9_Application)
add_opifex_oggvorbis(9_Application)
add_opifex_steam(9_Application)
add_opifex_fmod(9_Application)
add_opifex_xinput(9_Application)
add_opifex_raknet(9_Application)

message(STATUS "ADDING ${ADDON}")
add_marketplace_addons(9_Application)
