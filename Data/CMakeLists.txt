cmake_minimum_required(VERSION 2.8)

add_source_group(\\ "${PROJECT_SOURCE_DIR}/Data" DATA_SRC)

add_library(2_Data STATIC ${DATA_SRC})
SET_TARGET_PROPERTIES(2_Data PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(2_Data PROPERTIES 
	OUTPUT_NAME "Data"
)