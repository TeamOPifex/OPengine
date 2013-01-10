cmake_minimum_required(VERSION 2.8)

add_source_group(\\ "${PROJECT_SOURCE_DIR}/Performance" PERFORMANCE_SRC)

add_library(3_Performance STATIC ${PERFORMANCE_SRC})
SET_TARGET_PROPERTIES(3_Performance PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(3_Performance PROPERTIES 
	OUTPUT_NAME "Performance"
)