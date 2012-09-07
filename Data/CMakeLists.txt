cmake_minimum_required(VERSION 2.8)

include_directories(
	.
	../
)

add_source_group(\\ "${PROJECT_SOURCE_DIR}/Data" DATA_SRC)
add_source_group(Resources "${PROJECT_SOURCE_DIR}/Data/Resources" DATA_SRC)
add_source_group(Resources/Material "${PROJECT_SOURCE_DIR}/Data/Resources/Material" DATA_SRC)
add_source_group(Resources/Material/Shader "${PROJECT_SOURCE_DIR}/Data/Resources/Material/Shader" DATA_SRC)
add_source_group(Resources/Material/State "${PROJECT_SOURCE_DIR}/Data/Resources/Material/State" DATA_SRC)
add_source_group(Resources/Sound "${PROJECT_SOURCE_DIR}/Data/Resources/Sound" DATA_SRC)
add_source_group(Resources/Texture "${PROJECT_SOURCE_DIR}/Data/Resources/Texture" DATA_SRC)


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