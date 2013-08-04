cmake_minimum_required(VERSION 2.8)

include_directories(
	.
	../
)

add_source_group(\\ "${PROJECT_SOURCE_DIR}/Data" DATA_SRC)
add_source_group(src "${PROJECT_SOURCE_DIR}/Data/src" DATA_SRC)
add_source_group(include "${PROJECT_SOURCE_DIR}/Data/include" DATA_SRC)


set(ALL_LIBS
	1_Core
)
	
add_library(2_Data STATIC ${DATA_SRC})

target_link_libraries(2_Data
	${ALL_LIBS}
)
SET_TARGET_PROPERTIES(2_Data PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(2_Data PROPERTIES 
	OUTPUT_NAME "Data"
)