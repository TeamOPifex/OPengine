cmake_minimum_required(VERSION 2.8)

add_source_group(\\ "${PROJECT_SOURCE_DIR}/Core" CORE_SRC)

add_library(1_Core STATIC ${CORE_SRC})
SET_TARGET_PROPERTIES(1_Core PROPERTIES LINKER_LANGUAGE C)
set_target_properties(1_Core PROPERTIES 
	OUTPUT_NAME "Core"
)