cmake_minimum_required(VERSION 2.8)

add_source_group(\\ "${PROJECT_SOURCE_DIR}/Application" APPLICATION_SRC)

add_library(7_Application STATIC ${APPLICATION_SRC})
SET_TARGET_PROPERTIES(7_Application PROPERTIES LINKER_LANGUAGE C)
set_target_properties(7_Application PROPERTIES 
	OUTPUT_NAME "Application"
)