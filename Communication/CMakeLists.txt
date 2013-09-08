cmake_minimum_required(VERSION 2.8)

#add_source_group(\\ "${PROJECT_SOURCE_DIR}/Communication" COMMUNICATION_SRC)

add_library(6_Communication STATIC ${COMMUNICATION_SRC})
SET_TARGET_PROPERTIES(6_Communication PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(6_Communication PROPERTIES 
	OUTPUT_NAME "Communication"
)