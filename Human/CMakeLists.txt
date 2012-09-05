cmake_minimum_required(VERSION 2.8)

add_source_group(\\ "${PROJECT_SOURCE_DIR}/Human" HUMAN_SRC)


include_directories(
	../
)

add_library(3_Human STATIC ${HUMAN_SRC})
target_link_libraries(3_Human
	1_Core
)
SET_TARGET_PROPERTIES(3_Human PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(3_Human PROPERTIES 
	OUTPUT_NAME "Human"
)