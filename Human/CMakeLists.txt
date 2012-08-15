cmake_minimum_required(VERSION 2.8)

add_source_group(\\ "${PROJECT_SOURCE_DIR}/Human" HUMAN_SRC)

add_library(3_Human STATIC ${HUMAN_SRC})
SET_TARGET_PROPERTIES(3_Human PROPERTIES LINKER_LANGUAGE C)
set_target_properties(3_Human PROPERTIES 
	OUTPUT_NAME "Human"
)